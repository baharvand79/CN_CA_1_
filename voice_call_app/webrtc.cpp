#include "webrtc.h"
#include "qdebug.h"

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

WebRTC::WebRTC() {

//    AudioInput* audioInput = new AudioInput(this);
//    AudioOutput* audioOutput = new AudioOutput(this);

//    connect(audioInput, &AudioInput::audioCaptured, this, [this](const QByteArray& encodedData) {
//        if (peerConnection) {
//            sendAudio(encodedData);
//            qDebug() << "[WebRTC] Audio data sent, size:" << encodedData.size();
//        }
//    });

    rtc::Configuration config;
    config.iceServers.emplace_back("stun:stun.l.google.com:19302"); // STUN Server
    config.iceServers.emplace_back("turn:openrelayproject:openrelayproject@openrelay.metered.ca:80"); // TURN Server

    peerConnection = std::make_shared<rtc::PeerConnection>(config);

    // Add audio track
    audioTrack = peerConnection->addTrack(rtc::Description::Audio("Audio", rtc::Description::Direction::SendRecv));

    // Handle local description
    peerConnection->onLocalDescription([this](rtc::Description description) {
        emit localDescriptionGenerated(QString::fromStdString(description));
        qDebug() << "[WebRTC] Local SDP generated.";
    });

    // Handle ICE candidates
    peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
        QString candidateStr = QString::fromStdString(candidate);
        emit localCandidateGenerated(candidateStr);
        if (candidateStr.contains("typ host")) {
            qDebug() << "[WebRTC] Host candidate generated:" << candidateStr;
        } else if (candidateStr.contains("typ srflx")) {
            qDebug() << "[WebRTC] Server reflexive candidate generated:" << candidateStr;
        } else if (candidateStr.contains("typ relay")) {
            qDebug() << "[WebRTC] Relay (TURN) candidate generated:" << candidateStr;
        } else {
            qDebug() << "[WebRTC] Unknown candidate type:" << candidateStr;
        }
    });

    // Handle incoming audio frames
    peerConnection->onTrack([this](std::shared_ptr<rtc::Track> track) {
        track->onFrame([this](rtc::binary data, rtc::FrameInfo) {
            emit audioReceived(QByteArray(reinterpret_cast<const char*>(data.data()), data.size()));
            qDebug() << "[WebRTC] Audio frame received, size:" << data.size();
        });
    });

    // Monitor connection state changes
    peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
        qDebug() << "[WebRTC] PeerConnection state changed:" << QString::fromStdString(peerConnectionStateToString(state));
        if (state == rtc::PeerConnection::State::Failed) {
            qDebug() << "[WebRTC] PeerConnection failed: TURN server might be needed.";
        }
    });

    // Monitor ICE gathering completion
    peerConnection->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            emit gatheringCompleted();
            qDebug() << "[WebRTC] ICE gathering completed.";
        }
    });
}

void WebRTC::createOffer() {
    peerConnection->setLocalDescription(rtc::Description::Type::Offer);
    qDebug() << "[WebRTC] SDP Offer created.";
}

void WebRTC::setRemoteDescription(const QString& sdp) {
    peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString()));
    qDebug() << "[WebRTC] Remote SDP set.";
}

void WebRTC::addRemoteCandidate(const QString& candidate) {
    peerConnection->addRemoteCandidate(rtc::Candidate(candidate.toStdString()));
    qDebug() << "[WebRTC] Remote ICE candidate set.";
}

void WebRTC::sendAudio(const QByteArray &data) {
    // Assuming you have a track set up to send audio
    if (audioTrack) {
        const std::byte* byteData = reinterpret_cast<const std::byte*>(data.data());
        audioTrack->send(byteData, data.size());
        qDebug() << "[WebRTC] Audio data sent, size:" << data.size();
    }
}
