#include "webrtc.h"
#include <QDebug>

WebRTC::WebRTC()
    : socketClient() // Initialize socketClient here
{
    rtc::Configuration config;

    // STUN Server (Google's public STUN server)
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");

    // TURN Server
    config.iceServers.emplace_back("turn:openrelayproject:openrelayproject@openrelay.metered.ca:80");

    peerConnection = std::make_shared<rtc::PeerConnection>(config);

    // Initialize the audio track
    //rtc::Description::Audio media("Audio", rtc::Description::Direction::SendRecv);
    rtc::Description media(rtc::Description::Audio, rtc::Description::Direction::SendRecv);

    audioTrack = peerConnection->addTrack(media);

    // Connect the signaling client
    socketClient.connect("http://localhost:3000");

    peerConnection->onLocalDescription([this](rtc::Description description) {
        QString sdp = QString::fromStdString(description);
        socketClient.socket()->emit("offer", sdp.toStdString());
        emit localDescriptionGenerated(sdp);
        qDebug() << "Local SDP generated and sent via signaling";
    });

    peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
        QString candidateStr = QString::fromStdString(candidate);
        socketClient.socket()->emit("candidate", candidateStr.toStdString());
        emit localCandidateGenerated(candidateStr);
        qDebug() << "Local ICE candidate generated\n";
    });

    // Listen for remote SDP offers
    socketClient.socket()->on("offer", [&](sio::event& event) {
        QString remoteOffer = QString::fromStdString(event.get_message()->get_string());
        peerConnection->setRemoteDescription(rtc::Description(remoteOffer.toStdString(), "offer"));
        qDebug() << "Received remote SDP offer";

        // Prepare to create an answer
        qDebug() << "Preparing to create an answer...";
    });

    // Setup callback for handling incoming tracks
    peerConnection->onTrack([this](std::shared_ptr<rtc::Track> track) {
        track->onFrame([this](rtc::binary data, rtc::FrameInfo frameInfo) {
            emit audioReceived(QByteArray(reinterpret_cast<const char*>(data.data()), data.size()));
            qDebug() << "Audio frame received, size: " + QString::number(data.size()) + "\n";
            Q_UNUSED(frameInfo); // Use this if you want to silence the warning
        });
    });

    // Listen for remote SDP answers
    socketClient.socket()->on("answer", [&](sio::event& event) {
        QString remoteAnswer = QString::fromStdString(event.get_message()->get_string());
        peerConnection->setRemoteDescription(rtc::Description(remoteAnswer.toStdString(), "answer"));
        qDebug() << "Received remote SDP answer";
    });

    // Listen for remote ICE candidates
    socketClient.socket()->on("candidate", [&](sio::event& event) {
        QString remoteCandidate = QString::fromStdString(event.get_message()->get_string());
        peerConnection->addRemoteCandidate(rtc::Candidate(remoteCandidate.toStdString()));
        qDebug() << "Received remote ICE candidate";
    });

    // Register state change callback
    peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
        qDebug() << "PeerConnection state changed: " + QString::fromStdString(peerConnectionStateToString(state));
        if (state == rtc::PeerConnection::State::Failed) {
            qDebug() << "PeerConnection failed: TURN server might be needed.";
        }
    });

    // ICE gathering state change
    peerConnection->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            emit gatheringCompleted();
            qDebug() << "ICE gathering completed\n";
        }
    });
}
