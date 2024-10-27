#ifndef WEBRTC_H
#define WEBRTC_H

#include "rtc/rtc.hpp"
#include <QObject>
#include <QDebug>
#include "audioinput.h"
#include "audiooutput.h"
#include <opus/include/opus.h>

class WebRTC : public QObject {
    Q_OBJECT

    public:
        WebRTC();

        void createOffer();
        void setRemoteDescription(const QString& sdp);
        void addRemoteCandidate(const QString& candidate);
        void startAudioCapture();
        void stopAudioCapture();
        void sendAudio(const QByteArray &data);



    signals:
        void localDescriptionGenerated(const QString& sdp);
        void localCandidateGenerated(const QString& candidate);
        void audioReceived(const QByteArray& audioData);
        void gatheringCompleted();

    private:
        std::shared_ptr<rtc::PeerConnection> peerConnection;
        std::shared_ptr<rtc::Track> audioTrack;

        AudioInput audioInput;    // AudioInput for capturing audio
        AudioOutput audioOutput;  // AudioOutput for playing audio

        void handleAudioFrame(const QByteArray &audioData);

};
#endif // WEBRTC_H
