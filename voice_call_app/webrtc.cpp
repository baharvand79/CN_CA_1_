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
    rtc::Configuration config;

    // STUN Server (Google's public STUN server)
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");

    // TURN Server
    config.iceServers.emplace_back("turn:openrelayproject:openrelayproject@openrelay.metered.ca:80");

    peerConnection = std::make_shared<rtc::PeerConnection>(config);

    // Initialize the audio track
    rtc::Description::Audio media("Audio", rtc::Description::Direction::SendRecv);
    audioTrack = peerConnection->addTrack(media);

    peerConnection->onLocalDescription([this](rtc::Description description) {
        emit localDescriptionGenerated(QString::fromStdString(description));
        qDebug() << "[WebRTC] Local SDP generated\n";
    });

    peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
        QString candidateStr = QString::fromStdString(candidate);
        emit localCandidateGenerated(candidateStr);
        qDebug() << "[WebRTC] Local ICE candidate generated\n";

        // Check the type of candidate (host, srflx, relay)
            if (candidateStr.contains("typ host")) {
                qDebug() << "[WebRTC] Local host candidate generated: " << candidateStr << "\n";
            } else if (candidateStr.contains("typ srflx")) {
                qDebug() << "[WebRTC] Server reflexive (srflx) candidate generated: " << candidateStr << "\n";
            } else if (candidateStr.contains("typ relay")) {
                qDebug() << "[WebRTC] Relay (TURN) candidate generated: " << candidateStr;
            } else {
                qDebug() << "[WebRTC] Unknown type of ICE candidate: " << candidateStr << "\n";
            }
    });

    // Setup callback for handling incoming tracks
    peerConnection->onTrack([this](std::shared_ptr<rtc::Track> track) {
        track->onFrame([this](rtc::binary data, rtc::FrameInfo frameInfo) {
            emit audioReceived(QByteArray(reinterpret_cast<const char*>(data.data()), data.size()));
            qDebug() <<"[WebRTC] Audio frame received, size: " + QString::number(data.size()) + "\n";
            Q_UNUSED(frameInfo); // Use this if you want to silence the warning
        });
    });

    // Register state change callback
    peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
        qDebug() <<("[WebRTC] PeerConnection state changed: " + QString::fromStdString(peerConnectionStateToString(state)) + "\n");

        // Debugging for TURN interaction: Check if we're falling back to the TURN server
                if (state == rtc::PeerConnection::State::Failed) {
                    qDebug() << "[WebRTC] PeerConnection failed: TURN server might be needed.\n";
                }


    });

    // ICE gathering state change
    peerConnection->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            emit gatheringCompleted();
            qDebug() <<"[WebRTC] ICE gathering completed\n";
        }
    });
}




void WebRTC::createOffer() {
    peerConnection->setLocalDescription(rtc::Description::Type::Offer);
    qDebug() << "[WebRTC] SDP Offer created \n";
}

void WebRTC::setRemoteDescription(const QString& sdp) {
    peerConnection->setRemoteDescription(rtc::Description(sdp.toStdString()));
    qDebug() << "[WebRTC] Remote SDP set \n";
}

void WebRTC::addRemoteCandidate(const QString& candidate) {
    peerConnection->addRemoteCandidate(rtc::Candidate(candidate.toStdString()));
    qDebug() << "[WebRTC] Remote ICE candidate set \n";
}


