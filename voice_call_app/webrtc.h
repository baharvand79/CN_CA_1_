#ifndef WEBRTC_H
#define WEBRTC_H

#include <QObject>
#include <QWebSocket>
#include <memory>
#include <string>
#include "rtc/rtc.hpp"

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

public: Q_SIGNALS:
//    void peerIdGenerated(const QString &peerId);
    void localDescriptionGenerated(const QString &sdp);
    void localCandidateGenerated(const QString &candidate);
    void audioReceived(const QByteArray &data);
    void gatheringCompleted();
    void debugMessage(QString message);
    void clientIsRegistered();



public Q_SLOTS:
    void onSignalingServerConnected();
    void onSignalingServerDisconnected();
    void onSignalingMessageReceived(const QString &message);
    void check();
    void sendOffer();

private:
    std::shared_ptr<rtc::PeerConnection> peerConnection;
    std::shared_ptr<rtc::Track> audioTrack;
    std::shared_ptr<QWebSocket> webSocket; // WebSocket for signaling
    rtc::Description::Audio audio;
    rtc::Configuration config;
    QString peerId;
    QString targetId;
    bool peerIsOfferer;
    QString localDescription_string;
    rtc::Description localSDP;
    bool isClientRegistered = false; // Track client registration state
    bool isLocalDescriptionGenerated = false;

};

#endif // WEBRTC_H
