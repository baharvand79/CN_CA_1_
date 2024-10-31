#include "webrtc.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

std::string peerConnectionStateToString(rtc::PeerConnection::State state) {
    switch (state) {
        case rtc::PeerConnection::State::New: return "New";
        case rtc::PeerConnection::State::Connecting: return "Connecting";
        case rtc::PeerConnection::State::Connected: return "Connected";
        case rtc::PeerConnection::State::Disconnected: return "Disconnected";
        case rtc::PeerConnection::State::Failed: return "Failed";
        case rtc::PeerConnection::State::Closed: return "Closed";
        default: return "Unknown State";
    }
}

WebRTC::WebRTC(QObject *parent)
    : QObject{parent},
      webSocket(new QWebSocket()),
      localSDP("")
{
    Q_EMIT debugMessage("[WebRTC] Constructor called: Initialization completed.");
}

void WebRTC::setId(QString id) {
    peerId = id;
    Q_EMIT debugMessage("[WebRTC] Peer ID set to: " + peerId);
}

void WebRTC::setTargetId(QString id) {
    targetId = id;
    Q_EMIT debugMessage("[WebRTC] Target ID set to: " + targetId);
}

void WebRTC::init() {
    rtc::InitLogger(rtc::LogLevel::Debug);
    Q_EMIT debugMessage("[WebRTC] Initialization started.");

    config.iceServers.emplace_back("stun:stun.l.google.com:19302");
    config.iceServers.emplace_back("turn:openrelayproject:openrelayproject@openrelay.metered.ca:80");

    peerConnection = std::make_shared<rtc::PeerConnection>(config);
    Q_EMIT debugMessage("[WebRTC] ICE servers configured and peer connection created.");

    audioTrack = peerConnection->addTrack(rtc::Description::Audio("Audio", rtc::Description::Direction::SendRecv));
    Q_EMIT debugMessage("[WebRTC] Audio track added to peer connection.");
}

void WebRTC::callOnRun() {
    Q_EMIT debugMessage("[WebRTC] callOnRun initiated.");

    //if (!isSettingRemoteDescription){
        peerConnection->onLocalDescription([this](rtc::Description description) {
            Q_EMIT debugMessage("[WebRTC] Local SDP offer being created.");
            Q_EMIT localDescriptionGenerated(QString::fromStdString(description));
            isLocalDescriptionGenerated = true;
            localSDP = QString::fromStdString(description);
            Q_EMIT debugMessage("[WebRTC] Local SDP generated and emitted\n"+localSDP);
        });
    //}



    peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
        QString candidateStr = QString::fromStdString(candidate);
        localIceCandidates.append(candidateStr);
        Q_EMIT localCandidateGenerated(candidateStr);
        Q_EMIT debugMessage("[WebRTC] Local ICE candidate generated and emitted: " + candidateStr);
    });

    peerConnection->onTrack([this](std::shared_ptr<rtc::Track> track) {
        track->onFrame([this](rtc::binary data, rtc::FrameInfo) {
            Q_EMIT audioReceived(QByteArray(reinterpret_cast<const char*>(data.data()), data.size()));
            Q_EMIT debugMessage("[WebRTC] Audio frame received. Size: " + QString::number(data.size()) + " bytes.");
        });
    });

    peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
        QString stateStr = QString::fromStdString(peerConnectionStateToString(state));
        Q_EMIT debugMessage("[WebRTC] PeerConnection state changed: " + stateStr);
        if (state == rtc::PeerConnection::State::Failed) {
            Q_EMIT debugMessage("[WebRTC] Warning: Connection failed; a TURN server might be required.");
        }
    });

    peerConnection->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            Q_EMIT gatheringCompleted();
            Q_EMIT debugMessage("[WebRTC] ICE gathering completed successfully.");
        }
    });
}

// Function to convert WebSocket state to string
QString websocketStateToString(QAbstractSocket::SocketState state) {
    switch (state) {
        case QAbstractSocket::UnconnectedState: return "UnconnectedState";
        case QAbstractSocket::HostLookupState: return "HostLookupState";
        case QAbstractSocket::ConnectingState: return "ConnectingState";
        case QAbstractSocket::ConnectedState: return "ConnectedState";
        case QAbstractSocket::BoundState: return "BoundState";
        case QAbstractSocket::ListeningState: return "ListeningState";
        case QAbstractSocket::ClosingState: return "ClosingState";
        default: return "Unknown State";
    }
}

// Example usage in your WebRTC class
void WebRTC::checkWebSocketState() {
    QString stateString = websocketStateToString(webSocket->state());
    Q_EMIT debugMessage("[WebRTC] Current WebSocket state: " + stateString);
}

void WebRTC::registerClient() {
    if (!peerId.isEmpty()) {
        QJsonObject registrationMessage;
        registrationMessage["type"] = "register";
        registrationMessage["id"] = peerId;

        webSocket->sendTextMessage(QJsonDocument(registrationMessage).toJson());
        Q_EMIT debugMessage("[WebRTC] Registration message sent for Peer ID: " + peerId);
        checkWebSocketState();
        Q_EMIT clientIsRegistered();
        isClientRegistered = true;
    } else {
        Q_EMIT debugMessage("[WebRTC] Error: Peer ID is empty; registration skipped.");
    }
}

void WebRTC::connectToSignalingServer() {
    Q_EMIT debugMessage("[WebRTC] Attempting to connect to signaling server at ws://localhost:3000.");
    webSocket->open(QUrl("ws://localhost:3000"));

    connect(webSocket.get(), &QWebSocket::connected, this, &WebRTC::onSignalingServerConnected);
    connect(webSocket.get(), &QWebSocket::disconnected, this, &WebRTC::onSignalingServerDisconnected);
    connect(webSocket.get(), &QWebSocket::textMessageReceived, this, &WebRTC::onSignalingMessageReceived);
}


void WebRTC::sendOffer() {
    QJsonObject offerMessage;
    offerMessage["type"] = "offer";
    offerMessage["id"] = peerId;
    offerMessage["targetId"] = targetId;
    offerMessage["sdp"] = localSDP;

    if (webSocket->state() == QAbstractSocket::ConnectedState) {
        webSocket->sendTextMessage(QJsonDocument(offerMessage).toJson());
        Q_EMIT debugMessage("[WebRTC] SDP Offer sent to signaling server: " + localSDP);
    } else {
        Q_EMIT debugMessage("[WebRTC] Error: WebSocket not connected; cannot send offer.");
    }
}



void WebRTC::onSignalingServerConnected() {
    Q_EMIT debugMessage("[WebRTC] Successfully connected to signaling server.");
    registerClient();
}

void WebRTC::onSignalingServerDisconnected() {
    Q_EMIT debugMessage("[WebRTC] Disconnected from signaling server.");
}

void WebRTC::onSignalingMessageReceived(const QString &message) {
    Q_EMIT debugMessage("[WebRTC] Signaling message received from server: " + message);

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj = doc.object();
    QString type = jsonObj["type"].toString();

    if (type == "offer") {
        QString sdp = jsonObj["sdp"].toString();
                remoteSDP = sdp; // Store remote SDP
                Q_EMIT debugMessage("[WebRTC] Received offer with SDP: " + sdp);

                // Set remote description with the received offer
                peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString(), type.toStdString()));
                Q_EMIT debugMessage("[WebRTC] Remote SDP set for offer.");
                // Create an answer after setting the remote SDP
                //createAnswer();
    } else if (type == "answer") {
        QString sdp = jsonObj["sdp"].toString();
        remoteSDP = sdp;
        isSettingRemoteDescription = true;
        peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString(), type.toStdString()));
        Q_EMIT debugMessage("[WebRTC] Remote SDP set for answer.");
    } else if (type == "ice-candidate") {
        QString candidate = jsonObj["candidate"].toString();
        remoteIceCandidates.append(candidate);
        peerConnection->addRemoteCandidate(rtc::Candidate(candidate.toStdString()));
        Q_EMIT debugMessage("[WebRTC] Remote ICE candidate added: " + candidate);
    } else {
        Q_EMIT debugMessage("[WebRTC] Warning: Unrecognized signaling message type: " + type);
    }
}

void WebRTC::createAnswer() {
//    Q_EMIT debugMessage("[WebRTC] Creating new SDP answer.");
//    peerConnection->setLocalDescription(rtc::Description::Type::Answer);


}

void WebRTC::createOffer() {
    Q_EMIT debugMessage("[WebRTC] Creating new SDP offer.");
    peerConnection->setLocalDescription(rtc::Description::Type::Offer);
}


void WebRTC::setRemoteDescription(const QString& sdp) {
//    Q_EMIT debugMessage("[WebRTC] Setting remote SDP.");
//    peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString()));
//    Q_EMIT debugMessage("[WebRTC] Remote SDP set.");
}

void WebRTC::addRemoteCandidate(const QString& candidate) {
    Q_EMIT debugMessage("[WebRTC] Adding remote ICE candidate: " + candidate);
    peerConnection->addRemoteCandidate(rtc::Candidate(candidate.toStdString()));
    Q_EMIT debugMessage("[WebRTC] Remote ICE candidate added.");
}

