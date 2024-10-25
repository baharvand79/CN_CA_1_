#ifndef WEBRTC_H
#define WEBRTC_H

#include "rtc/rtc.hpp"
#include <QObject>
#include <QDebug>
class WebRTC : public QObject {
    Q_OBJECT

    public:
        WebRTC();

    signals:
        void localDescriptionGenerated(const QString& sdp);
        void localCandidateGenerated(const QString& candidate);
        void audioReceived(const QByteArray& audioData);
        void gatheringCompleted();

    private:
        std::shared_ptr<rtc::PeerConnection> peerConnection;
        std::shared_ptr<rtc::Track> audioTrack;

};

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

#endif // WEBRTC_H
