#include "webrtc.h"
#include <QDebug>


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
        qDebug() << "Local SDP generated";
    });

    peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
        QString candidateStr = QString::fromStdString(candidate);
        emit localCandidateGenerated(candidateStr);
        qDebug() << "Local ICE candidate generated\n";

        // Check the type of candidate (host, srflx, relay)
            if (candidateStr.contains("typ host")) {
                qDebug() << "Local host candidate generated: " << candidateStr;
            } else if (candidateStr.contains("typ srflx")) {
                qDebug() << "Server reflexive (srflx) candidate generated: " << candidateStr;
            } else if (candidateStr.contains("typ relay")) {
                qDebug() << "Relay (TURN) candidate generated: " << candidateStr;
            } else {
                qDebug() << "Unknown type of ICE candidate: " << candidateStr;
            }
    });

    // Setup callback for handling incoming tracks
    peerConnection->onTrack([this](std::shared_ptr<rtc::Track> track) {
        track->onFrame([this](rtc::binary data, rtc::FrameInfo frameInfo) {
            emit audioReceived(QByteArray(reinterpret_cast<const char*>(data.data()), data.size()));
            qDebug() <<"Audio frame received, size: " + QString::number(data.size()) + "\n";
            Q_UNUSED(frameInfo); // Use this if you want to silence the warning
        });
    });

    // Register state change callback
    peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
        qDebug() <<("PeerConnection state changed: " + QString::fromStdString(peerConnectionStateToString(state)));

        // Debugging for TURN interaction: Check if we're falling back to the TURN server
                if (state == rtc::PeerConnection::State::Failed) {
                    qDebug() << "PeerConnection failed: TURN server might be needed.";
                }

    });

    // ICE gathering state change
    peerConnection->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            emit gatheringCompleted();
            qDebug() <<"ICE gathering completed\n";
        }
    });
}

std::string WebRTC::peerConnectionStateToString(rtc::PeerConnection::State state) {
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
