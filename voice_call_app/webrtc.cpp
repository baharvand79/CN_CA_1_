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
    Q_EMIT debugMessage("[WebRTC] Constructor called and initialized.");
}

void WebRTC::setId(QString id) {
    peerId = id;
    Q_EMIT debugMessage("[WebRTC] Peer Id is set to: " + peerId);
}

void WebRTC::setTargetId(QString id) {
    targetId = id;
    Q_EMIT debugMessage("[WebRTC] Target Id is set to: " + peerId);
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

    callOnRun();
    connectToSignalingServer();
}

void WebRTC::callOnRun() {
    Q_EMIT debugMessage("[WebRTC] The callOnRun started.");

    peerConnection->onLocalDescription([this](rtc::Description description) {
        Q_EMIT debugMessage("[WebRTC] Creating SDP offer.");
        Q_EMIT debugMessage("here in callOnRun$$$$$$$$$$$");
        Q_EMIT localDescriptionGenerated(QString::fromStdString(description));
        isLocalDescriptionGenerated = true;
        Q_EMIT debugMessage("[WebRTC] Local SDP generated and emitted.");
        Q_EMIT debugMessage("The generated SDP is: "+QString::fromStdString(description));
        localSDP = description;
    });

    peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
        QString candidateStr = QString::fromStdString(candidate);
        Q_EMIT localCandidateGenerated(candidateStr);
        Q_EMIT debugMessage("[WebRTC] Local candidate generated and emitted: " + candidateStr);

    });

    peerConnection->onTrack([this](std::shared_ptr<rtc::Track> track) {
        track->onFrame([this](rtc::binary data, rtc::FrameInfo) {
            Q_EMIT audioReceived(QByteArray(reinterpret_cast<const char*>(data.data()), data.size()));
            Q_EMIT debugMessage("[WebRTC] Audio frame received. Size: " + QString::number(data.size()));
        });
    });

    peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
        Q_EMIT debugMessage("[WebRTC] PeerConnection state changed: " + QString::fromStdString(peerConnectionStateToString(state)));
        if (state == rtc::PeerConnection::State::Failed) {
            Q_EMIT debugMessage("[WebRTC] Connection failed; TURN server might be required.");
        }
    });

    peerConnection->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            Q_EMIT gatheringCompleted();
            Q_EMIT debugMessage("[WebRTC] ICE gathering complete.");
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
    Q_EMIT debugMessage("[WebRTC] WebSocket state: " + stateString);
}

void WebRTC::registerClient() {
    if (!peerId.isEmpty()) {
        QJsonObject registrationMessage;
        registrationMessage["type"] = "register";
        registrationMessage["id"] = peerId;

        webSocket->sendTextMessage(QJsonDocument(registrationMessage).toJson());
        Q_EMIT debugMessage("#######register######");
        checkWebSocketState();
        Q_EMIT clientIsRegistered();
        isClientRegistered = true;
        Q_EMIT debugMessage("[WebRTC] Sent registration message for ID: " + peerId);

    } else {
        Q_EMIT debugMessage("[WebRTC] Peer ID is empty; registration skipped.");
    }
}

void WebRTC::connectToSignalingServer() {
    Q_EMIT debugMessage("[WebRTC] Attempting to connect to signaling server.");
    webSocket->open(QUrl("ws://localhost:3000"));

    connect(webSocket.get(), &QWebSocket::connected, this, &WebRTC::onSignalingServerConnected);
    connect(webSocket.get(), &QWebSocket::disconnected, this, &WebRTC::onSignalingServerDisconnected);
    connect(webSocket.get(), &QWebSocket::textMessageReceived, this, &WebRTC::onSignalingMessageReceived);
    connect(this,&WebRTC::localDescriptionGenerated, this, &WebRTC::sendOffer);
}

void WebRTC::sendOffer() {
    Q_EMIT debugMessage("[WebRTC] Sending offer to signaling server.");

    connect(this, &WebRTC::clientIsRegistered, this, &WebRTC::check);
//    check();
//    // Create the offer JSON message
//    QJsonObject offerMessage;
//    offerMessage["type"] = "offer";
//    offerMessage["sdp"] = localSDP;

//    // Send the offer via WebSocket
//    if (webSocket->state() == QAbstractSocket::ConnectedState) {
//        webSocket->sendTextMessage(QJsonDocument(offerMessage).toJson());
//        Q_EMIT debugMessage("[WebRTC] SDP Offer sent: " + localSDP);
//    } else {
//        Q_EMIT debugMessage("[WebRTC] WebSocket not connected; cannot send offer.");
//    }
}

void WebRTC::check(){
    Q_EMIT debugMessage("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%check is on%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
    // Create JSON message to send to server
    QJsonObject offerMessage;
    offerMessage["type"] = "offer";
    offerMessage["sdp"] = QString::fromStdString(localSDP);

    Q_EMIT debugMessage("#######check######");
    checkWebSocketState();
    // Send the offer to the signaling server
    if (webSocket->state() == QAbstractSocket::ConnectedState) {
        webSocket->sendTextMessage(QJsonDocument(offerMessage).toJson());
        Q_EMIT debugMessage("[WebRTC] SDP Offer sent to signaling server: " + QString::fromStdString(localSDP));
    } else {
        Q_EMIT debugMessage("[WebRTC] WebSocket not connected; cannot send offer.");
    }
}
void WebRTC::onSignalingServerConnected() {
    Q_EMIT debugMessage("[WebRTC] Connected to signaling server.");
    registerClient();
}

void WebRTC::onSignalingServerDisconnected() {
    Q_EMIT debugMessage("[WebRTC] Disconnected from signaling server.");
}

void WebRTC::onSignalingMessageReceived(const QString &message) {
    Q_EMIT debugMessage("[WebRTC] Message received from signaling server: " + message);

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj = doc.object();
    QString type = jsonObj["type"].toString();

    if (type == "offer" || type == "answer") {
        QString sdp = jsonObj["sdp"].toString();
        peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString(), type.toStdString()));
        Q_EMIT debugMessage("[WebRTC] Remote SDP set for " + type + ".");
    } else if (type == "ice-candidate") {
        QString candidate = jsonObj["candidate"].toString();
        peerConnection->addRemoteCandidate(rtc::Candidate(candidate.toStdString()));
        Q_EMIT debugMessage("[WebRTC] Remote ICE candidate added: " + candidate);
    } else {
        Q_EMIT debugMessage("[WebRTC] Unrecognized signaling message type: " + type);
    }
}

void WebRTC::createOffer() {
    Q_EMIT debugMessage("[WebRTC] Creating SDP offer.");
    peerConnection->setLocalDescription(rtc::Description::Type::Offer);
}


void WebRTC::setRemoteDescription(const QString& sdp) {
    Q_EMIT debugMessage("[WebRTC] Setting remote SDP.");
    peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString()));
    Q_EMIT debugMessage("[WebRTC] Remote SDP set successfully.");
}

void WebRTC::addRemoteCandidate(const QString& candidate) {
    Q_EMIT debugMessage("[WebRTC] Adding remote ICE candidate.");
    peerConnection->addRemoteCandidate(rtc::Candidate(candidate.toStdString()));
    Q_EMIT debugMessage("[WebRTC] Remote ICE candidate set successfully.");
}
