//#ifndef SIGNALINGCLIENT_H
//#define SIGNALINGCLIENT_H

//#include <QObject>
//#include <QWebSocket>
//#include <QJsonObject>
//#include <QJsonDocument>
//#include "webrtc.h"
//enum class MessageType {
//    Offer,
//    Answer,
//    Candidate,
//    ChatMessage
//};


//class SignalingClient : public QObject {
//    Q_OBJECT
//public:
//    explicit SignalingClient(const QUrl &url, QObject *parent = nullptr);
//    void sendMessage(MessageType type, const QString &content);
//    QJsonObject createMessage(MessageType type, const QString& content);

//public slots:
//    void sendMessage(const QString &message);

//signals:
//    void messageReceived(const QString &message);

//private slots:
//    void onConnected();
//    void onTextMessageReceived(const QString &message);
//    void onDisconnected();

//private:
//    QWebSocket m_webSocket;
//    QUrl m_url;
//    WebRTC *m_webRTC;
//};



//#endif // SIGNALINGCLIENT_H
