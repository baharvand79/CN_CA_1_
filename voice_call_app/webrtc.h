#ifndef WEBRTC_H
#define WEBRTC_H

#include "rtc/rtc.hpp"
#include <QObject>
#include <QDebug>
#include <QAudioInput>
#include <QAudioOutput>
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

    signals:
        void localDescriptionGenerated(const QString& sdp);
        void localCandidateGenerated(const QString& candidate);
        void audioReceived(const QByteArray& audioData);
        void gatheringCompleted();

    private:
        std::shared_ptr<rtc::PeerConnection> peerConnection;
        std::shared_ptr<rtc::Track> audioTrack;

        QAudioInput *audioInput;
        QAudioOutput *audioOutput;
        QIODevice *audioIODevice;

        OpusEncoder *encoder;
        OpusDecoder *decoder;

        void handleAudioFrame(const QByteArray &audioData);

};
#endif // WEBRTC_H
