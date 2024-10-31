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
      webSocket(new QWebSocket())
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

    connectToSignalingServer();
}

void WebRTC::establishConnection() {
    Q_EMIT debugMessage("[WebRTC] Starting to establish connection.");

    peerConnection->onLocalDescription([this](rtc::Description description) {
        Q_EMIT localDescriptionGenerated(QString::fromStdString(description));
        Q_EMIT debugMessage("[WebRTC] Local SDP generated and emitted.");
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

void WebRTC::registerClient() {
    if (!peerId.isEmpty()) {
        QJsonObject registrationMessage;
        registrationMessage["type"] = "register";
        registrationMessage["id"] = peerId;

        webSocket->sendTextMessage(QJsonDocument(registrationMessage).toJson());
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
}

void WebRTC::onSignalingServerConnected() {
    Q_EMIT debugMessage("[WebRTC] Connected to signaling server.");
    registerClient();
    establishConnection();
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
    Q_EMIT debugMessage("[WebRTC] SDP Offer created.");
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
