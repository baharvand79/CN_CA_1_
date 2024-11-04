#ifndef WEBRTC_H
#define WEBRTC_H

#include <QObject>
#include <QWebSocket>
#include <memory>
#include <string>
#include "audioinput.h"
#include "rtc/rtc.hpp"
#include <QtEndian>
#include <QJsonDocument>
#include <cstdint>
#include <stdint.h>

#pragma pack(push, 1) // Ensure no padding between fields
struct RTPHeader {
    uint8_t first = 0b10000000;
    uint8_t marker : 1;
    uint8_t payloadType : 7;
    uint16_t sequenceNumber;
    uint32_t timestamp;
    uint32_t ssrc;
};
#pragma pack(pop)

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
    Q_INVOKABLE void addRemoteCandidate(const QString& candidate);
    Q_INVOKABLE void setTargetId(QString id);
    Q_INVOKABLE void checkWebSocketState();
    Q_INVOKABLE void createAnswer();
    Q_INVOKABLE bool getIsOfferer();
    Q_INVOKABLE RTPHeader createRTPHeader(uint16_t sequenceNumber);


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
    void audioDataReceived(const QByteArray &data);



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
    void sendTrack(const QByteArray &audioData);

private:
    std::shared_ptr<rtc::PeerConnection> peerConnection;
    std::shared_ptr<rtc::Track> audioTrack;
    std::shared_ptr<QWebSocket> webSocket;
    rtc::Description::Audio audio;
    rtc::Configuration config;
    QString peerId;
    QString targetId;
    bool peerIsOfferer;
     QString localSDP;
    QString remoteSDP;
    bool isClientRegistered = false;
    bool isLocalDescriptionGenerated = false;
    QStringList localIceCandidates;
    QStringList remoteIceCandidates;
    bool isSettingRemoteDescription = false;

//    const QByteArray &buffer;
//    inline uint32_t getCurrentTimeStamp(){
//        auto now = std::chrono::steady_clock::now();
//        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

//        return static_cast<uint32_t>(ms);
//    }
    inline uint32_t getCurrentTimeStamp() {
        auto now = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        uint32_t samples = static_cast<uint32_t>(ms * 48); // Adjust for 48kHz sample rate
        return samples;
    }

//    static inline uint16_t                              m_sequenceNumber = 0;

    uint16_t sequenceNumber = 0;
    uint32_t timestamp = 0;
    uint32_t ssrc = 2;
    uint32_t payloadType = 111;
    Q_PROPERTY(bool peerIsOfferer READ getPeerIsOfferer WRITE setPeerIsOfferer RESET resetPeerIsOfferer NOTIFY peerIsOffererChanged)
};

#endif // WEBRTC_H
