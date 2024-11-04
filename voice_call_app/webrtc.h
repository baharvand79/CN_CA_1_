#ifndef WEBRTC_H
#define WEBRTC_H

#include <QObject>
#include <QWebSocket>
#include <memory>
#include <string>
#include "audioinput.h"
#include "audiooutput.h"
#include "rtc/rtc.hpp"
#include <QtEndian>
#include <QJsonDocument>
#include <cstdint>
#include <stdint.h>
class WebRTC : public QObject {
    Q_OBJECT

public:
    explicit WebRTC(QObject *parent = nullptr);

    Q_INVOKABLE void setId(QString id);
    Q_INVOKABLE void init();
    Q_INVOKABLE void registerClient();
    Q_INVOKABLE void connectToSignalingServer();
    Q_INVOKABLE void createOffer();
    Q_INVOKABLE void callOnRun();
    Q_INVOKABLE void setRemoteDescription(const QString& sdp);
    Q_INVOKABLE void addRemoteCandidate(const QString& candidate);
    Q_INVOKABLE void setTargetId(QString id);
    Q_INVOKABLE void checkWebSocketState();
    Q_INVOKABLE void createAnswer();
    AudioInput *audioInput;
    AudioOutput *audioOutput;



public:  bool getPeerIsOfferer() const;
    void setPeerIsOfferer(bool newPeerIsOfferer);
    void resetPeerIsOfferer();

Q_SIGNALS:
    void localDescriptionGenerated(const QString &sdp);
    void localCandidateGenerated(const QString &candidate);
    void audioReceived(const QByteArray &data);
    void gatheringCompleted();
    void debugMessage(QString message);
    void clientIsRegistered();
    void answerIsReady();
    void peerIsConnected();
//    void incommingPacket(peerId, packet, packet.size());



    void peerIsOffererChanged();

public Q_SLOTS:
    void onSignalingServerConnected();
    void onSignalingServerDisconnected();
    void onSignalingMessageReceived(const QString &message);
//    void sendOfferHelper();
    void sendOffer();
    void sendAnswer();
    void onAudioCaptured(const QByteArray &audioData);
//    void onPeerIsConnected();
    void sendTrack(const QString &peerId, const QByteArray &buffer);

private:
    std::shared_ptr<rtc::PeerConnection> peerConnection;
    std::shared_ptr<rtc::Track> audioTrack;
    std::shared_ptr<QWebSocket> webSocket; // WebSocket for signaling
    rtc::Description::Audio audio;
    rtc::Configuration config;
    QString peerId;
    QString targetId;
    bool peerIsOfferer;
     QString localSDP;
    QString remoteSDP;
    bool isClientRegistered = false; // Track client registration state
    bool isLocalDescriptionGenerated = false;
    QStringList localIceCandidates; // Store local ICE candidates
    QStringList remoteIceCandidates; // Store remote ICE candidates
    bool isSettingRemoteDescription = false;

//    const QByteArray &buffer;
    inline uint32_t getCurrentTimeStamp(){
        auto now = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        return static_cast<uint32_t>(ms);
    }
    static inline uint16_t                              m_sequenceNumber = 0;
    QMap<QString, std::shared_ptr<rtc::DataChannel>> datachannels;
    Q_PROPERTY(bool peerIsOfferer READ getPeerIsOfferer WRITE setPeerIsOfferer RESET resetPeerIsOfferer NOTIFY peerIsOffererChanged)
};

#endif // WEBRTC_H
