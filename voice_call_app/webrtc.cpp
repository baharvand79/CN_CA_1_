//#include "webrtc.h"
//#include <QDebug>
//#include <QUuid> // Include QUuid for UUID generation

//// Convert peer connection state to string representation
//std::string peerConnectionStateToString(rtc::PeerConnection::State state) {
//    switch (state) {
//        case rtc::PeerConnection::State::New: return "New";
//        case rtc::PeerConnection::State::Connecting: return "Connecting";
//        case rtc::PeerConnection::State::Connected: return "Connected";
//        case rtc::PeerConnection::State::Disconnected: return "Disconnected";
//        case rtc::PeerConnection::State::Failed: return "Failed";
//        case rtc::PeerConnection::State::Closed: return "Closed";
//        default: return "Unknown State";
//    }
//}

//WebRTC::WebRTC(QObject *parent) : QObject(parent) {
//    // Generate a unique peer ID
//    peerId = QUuid::createUuid().toString(); // Generates a UUID string
//    emit peerIdGenerated(peerId); // Emit the generated peer ID
//    qDebug() << "[WebRTC] Created Peer ID:" << peerId; // Output the generated Peer ID

//    rtc::Configuration config;
//    config.iceServers.emplace_back("stun:stun.l.google.com:19302"); // STUN Server
//    config.iceServers.emplace_back("turn:openrelayproject:openrelayproject@openrelay.metered.ca:80"); // TURN Server

//    peerConnection = std::make_shared<rtc::PeerConnection>(config);

//    // Add audio track
//    audioTrack = peerConnection->addTrack(rtc::Description::Audio("Audio", rtc::Description::Direction::SendRecv));

//    // Handle local description
//    peerConnection->onLocalDescription([this](rtc::Description description) {
//        emit localDescriptionGenerated(QString::fromStdString(description));
//        qDebug() << "[WebRTC] Local SDP generated for Peer ID:" << peerId; // Include Peer ID
//    });

//    // Handle local ICE candidates
//    peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
//        emit localCandidateGenerated(QString::fromStdString(candidate));
//        qDebug() << "[WebRTC] Local ICE candidate generated for Peer ID:" << peerId
//                 << ":" << QString::fromStdString(candidate); // Include Peer ID
//    });

//    // Handle incoming audio frames
//    peerConnection->onTrack([this](std::shared_ptr<rtc::Track> track) {
//        track->onFrame([this](rtc::binary data, rtc::FrameInfo) {
//            emit audioReceived(QByteArray(reinterpret_cast<const char*>(data.data()), data.size()));
//            qDebug() << "[WebRTC] Audio frame received for Peer ID:" << peerId
//                     << ", size:" << data.size(); // Include Peer ID
//        });
//    });

//    // Monitor connection state changes
//    peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
//        qDebug() << "[WebRTC] PeerConnection state changed for Peer ID:" << peerId
//                 << ":" << QString::fromStdString(peerConnectionStateToString(state)); // Include Peer ID
//    });

//    // Monitor ICE gathering completion
//    peerConnection->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
//        if (state == rtc::PeerConnection::GatheringState::Complete) {
//            emit gatheringCompleted();
//            qDebug() << "[WebRTC] ICE gathering completed for Peer ID:" << peerId; // Include Peer ID
//        }
//    });
//}

//void WebRTC::createOffer() {
//    peerConnection->setLocalDescription(rtc::Description::Type::Offer);
//    qDebug() << "[WebRTC] Offer created for Peer ID:" << peerId; // Include Peer ID
//}

//void WebRTC::setRemoteDescription(const QString &sdp) {
//    peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString()));
//    qDebug() << "[WebRTC] Remote SDP set for Peer ID:" << peerId; // Include Peer ID
//}

//void WebRTC::addRemoteCandidate(const QString &candidate) {
//    peerConnection->addRemoteCandidate(rtc::Candidate(candidate.toStdString()));
//    qDebug() << "[WebRTC] Remote ICE candidate added for Peer ID:" << peerId; // Include Peer ID
//}

//void WebRTC::sendAudio(const QByteArray &data) {
//    if (audioTrack) {
//        const std::byte *byteData = reinterpret_cast<const std::byte *>(data.data());
//        audioTrack->send(byteData, data.size());
//        qDebug() << "[WebRTC] Audio data sent for Peer ID:" << peerId
//                 << ", size:" << data.size(); // Include Peer ID
//    }
//}


#include "webrtc.h"
#include <QDebug>
#include <QUuid> // Include QUuid for UUID generation
#include <QWebSocket> // Include QWebSocket for signaling
#include <QWebSocketServer> // Include QWebSocketServer if needed
#include <QJsonDocument>
#include <QJsonObject>

// Convert peer connection state to string representation
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

WebRTC::WebRTC(QObject *parent) : QObject(parent), webSocket(new QWebSocket()) {
    // Generate a unique peer ID
    peerId = QUuid::createUuid().toString(); // Generates a UUID string
    emit peerIdGenerated(peerId); // Emit the generated peer ID
    qDebug() << "[WebRTC] Created Peer ID:" << peerId; // Output the generated Peer ID

    rtc::Configuration config;
    config.iceServers.emplace_back("stun:stun.l.google.com:19302"); // STUN Server
    config.iceServers.emplace_back("turn:openrelayproject:openrelayproject@openrelay.metered.ca:80"); // TURN Server

    peerConnection = std::make_shared<rtc::PeerConnection>(config);

    // Add audio track
    audioTrack = peerConnection->addTrack(rtc::Description::Audio("Audio", rtc::Description::Direction::SendRecv));

    // Connect to signaling server
    connectToSignalingServer();

    // Handle local description
    peerConnection->onLocalDescription([this](rtc::Description description) {
        emit localDescriptionGenerated(QString::fromStdString(description));
        qDebug() << "[WebRTC] Local SDP generated for Peer ID:" << peerId; // Include Peer ID
        // Send local description to the signaling server
        sendMessageToSignalingServer("offer", description);
    });

    // Handle local ICE candidates
    peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
        emit localCandidateGenerated(QString::fromStdString(candidate));
        qDebug() << "[WebRTC] Local ICE candidate generated for Peer ID:" << peerId
                 << ":" << QString::fromStdString(candidate); // Include Peer ID
        // Send local candidate to the signaling server
        sendMessageToSignalingServer("ice-candidate", candidate);
    });

    // Handle incoming audio frames
    peerConnection->onTrack([this](std::shared_ptr<rtc::Track> track) {
        track->onFrame([this](rtc::binary data, rtc::FrameInfo) {
            emit audioReceived(QByteArray(reinterpret_cast<const char*>(data.data()), data.size()));
            qDebug() << "[WebRTC] Audio frame received for Peer ID:" << peerId
                     << ", size:" << data.size(); // Include Peer ID
        });
    });

    // Monitor connection state changes
    peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
        qDebug() << "[WebRTC] PeerConnection state changed for Peer ID:" << peerId
                 << ":" << QString::fromStdString(peerConnectionStateToString(state)); // Include Peer ID
    });

    // Monitor ICE gathering completion
    peerConnection->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            emit gatheringCompleted();
            qDebug() << "[WebRTC] ICE gathering completed for Peer ID:" << peerId; // Include Peer ID
        }
    });
}

void WebRTC::connectToSignalingServer() {
    webSocket->open(QUrl("ws://localhost:3000")); // Connect to signaling server

    connect(webSocket, &QWebSocket::connected, this, &WebRTC::onSignalingServerConnected);
    connect(webSocket, &QWebSocket::disconnected, this, &WebRTC::onSignalingServerDisconnected);
    connect(webSocket, &QWebSocket::textMessageReceived, this, &WebRTC::onSignalingMessageReceived);
}

void WebRTC::onSignalingServerConnected() {
    qDebug() << "[WebRTC] Connected to signaling server.";
}

void WebRTC::onSignalingServerDisconnected() {
    qDebug() << "[WebRTC] Disconnected from signaling server.";
}

void WebRTC::onSignalingMessageReceived(const QString &message) {
    qDebug() << "[WebRTC] Message received from signaling server:" << message;

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj = doc.object();
    QString type = jsonObj["type"].toString();

    if (type == "offer") {
        QString sdp = jsonObj["sdp"].toString();
        setRemoteDescription(sdp);
    } else if (type == "answer") {
        QString sdp = jsonObj["sdp"].toString();
        setRemoteDescription(sdp);
    } else if (type == "ice-candidate") {
        QString candidate = jsonObj["candidate"].toString();
        addRemoteCandidate(candidate);
    }
}

void WebRTC::sendMessageToSignalingServer(const QString &type, const rtc::Description &description) {
    QJsonObject jsonObj;
    jsonObj["type"] = type;
    jsonObj["sdp"] = QString::fromStdString(description);
    webSocket->sendTextMessage(QJsonDocument(jsonObj).toJson());
}

void WebRTC::sendMessageToSignalingServer(const QString &type, const rtc::Candidate &candidate) {
    QJsonObject jsonObj;
    jsonObj["type"] = type;
    jsonObj["candidate"] = QString::fromStdString(candidate);
    webSocket->sendTextMessage(QJsonDocument(jsonObj).toJson());
}

void WebRTC::createOffer() {
    peerConnection->setLocalDescription(rtc::Description::Type::Offer);
    qDebug() << "[WebRTC] Offer created for Peer ID:" << peerId; // Include Peer ID
}

void WebRTC::setRemoteDescription(const QString &sdp) {
    peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString()));
    qDebug() << "[WebRTC] Remote SDP set for Peer ID:" << peerId; // Include Peer ID
}

void WebRTC::addRemoteCandidate(const QString &candidate) {
    peerConnection->addRemoteCandidate(rtc::Candidate(candidate.toStdString()));
    qDebug() << "[WebRTC] Remote ICE candidate added for Peer ID:" << peerId; // Include Peer ID
}

void WebRTC::sendAudio(const QByteArray &data) {
    if (audioTrack) {
        const std::byte *byteData = reinterpret_cast<const std::byte *>(data.data());
        audioTrack->send(byteData, data.size());
        qDebug() << "[WebRTC] Audio data sent for Peer ID:" << peerId
                 << ", size:" << data.size(); // Include Peer ID
    }
}

