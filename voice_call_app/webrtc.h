#ifndef WEBRTC_H
#define WEBRTC_H

#include "rtc/rtc.hpp"
#include <QObject>
#include <QDebug>
class WebRTC : public QObject {
    Q_OBJECT

    public:
        WebRTC();
        void init();
        void createOffer();
        void setRemoteDescription(const QString& sdp);
        void addRemoteCandidate(const QString& candidate);

    signals:
        void localDescriptionGenerated(const QString& sdp);
        void localCandidateGenerated(const QString& candidate);
        void audioReceived(const QByteArray& audioData);
        void gatheringCompleted();

    private:
        std::shared_ptr<rtc::PeerConnection> peerConnection;
        std::shared_ptr<rtc::Track> audioTrack;

};
#endif // WEBRTC_H
