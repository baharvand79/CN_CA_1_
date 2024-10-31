//#ifndef WEBRTC_H
//#define WEBRTC_H

//#include "rtc/rtc.hpp"
//#include <QObject>
//#include <QDebug>
//#include "audioinput.h"
//#include "audiooutput.h"
//#include <opus/include/opus.h>

//class WebRTC : public QObject {
//    Q_OBJECT

//    public:
//        WebRTC();

//        void createOffer();
//        void setRemoteDescription(const QString& sdp);
//        void addRemoteCandidate(const QString& candidate);
//        void startAudioCapture();
//        void stopAudioCapture();
//        void sendAudio(const QByteArray &data);



//    signals:
//        void localDescriptionGenerated(const QString& sdp);
//        void localCandidateGenerated(const QString& candidate);
//        void audioReceived(const QByteArray& audioData);
//        void gatheringCompleted();

//    private:
//        std::shared_ptr<rtc::PeerConnection> peerConnection;
//        std::shared_ptr<rtc::Track> audioTrack;

//        AudioInput audioInput;    // AudioInput for capturing audio
//        AudioOutput audioOutput;  // AudioOutput for playing audio

//        void handleAudioFrame(const QByteArray &audioData);

//};
//#endif // WEBRTC_H


#ifndef WEBRTC_H
#define WEBRTC_H

#include <QObject>
#include <QWebSocket>
#include <memory>
#include <string>
#include "rtc/rtc.hpp" // Include the WebRTC peer connection header

class WebRTC : public QObject {
    Q_OBJECT

public:
    explicit WebRTC(QObject *parent = nullptr);

    Q_INVOKABLE void setId(QString id);
    Q_INVOKABLE void init();
    Q_INVOKABLE void registerClient();
    Q_INVOKABLE void connectToSignalingServer();
    Q_INVOKABLE void createOffer();
    Q_INVOKABLE void establishConnection();
    Q_INVOKABLE void setRemoteDescription(const QString& sdp);
    Q_INVOKABLE void addRemoteCandidate(const QString& candidate);

public: Q_SIGNALS:
//    void peerIdGenerated(const QString &peerId);
    void localDescriptionGenerated(const QString &sdp);
    void localCandidateGenerated(const QString &candidate);
    void audioReceived(const QByteArray &data);
    void gatheringCompleted();
    void debugMessage(QString message);

public Q_SLOTS:
    void onSignalingServerConnected();
    void onSignalingServerDisconnected();
    void onSignalingMessageReceived(const QString &message);

private:
//    void connectToSignalingServer();
//    void sendMessageToSignalingServer(const QString &type, const rtc::Description &description);
//    void sendMessageToSignalingServer(const QString &type, const rtc::Candidate &candidate);

//    QString peerId;
    std::shared_ptr<rtc::PeerConnection> peerConnection;
    std::shared_ptr<rtc::Track> audioTrack;
    std::shared_ptr<QWebSocket> webSocket; // WebSocket for signaling
    /// adding new
//    rtc::Description m_localDescription;
    rtc::Description::Audio audio;
    rtc::Configuration config;
    QString peerId;
    bool peerIsOfferer;
    QString localDescription_string;

};

#endif // WEBRTC_H
