#include "webrtc.h"
#include "qdebug.h"


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
        emit localCandidateGenerated(QString::fromStdString(candidate));
        qDebug() << "Local ICE candidate generated\n";
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
    });

    // ICE gathering state change
    peerConnection->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            emit gatheringCompleted();
            qDebug() <<"ICE gathering completed\n";
        }
    });
}
