//#ifndef SIGNALINGSERVER_H
//#define SIGNALINGSERVER_H

//#include <QObject>
//#include <QWebSocketServer>
//#include <QWebSocket>
//#include <QJsonDocument>
//#include <QJsonObject>

//class SignalingServer : public QObject {
//    Q_OBJECT

//public:
//    explicit SignalingServer(quint16 port, QObject *parent = nullptr);
//    ~SignalingServer();

//    void sendLocalDescription(const QString &sdp);
//    void sendLocalCandidate(const QString &candidate);
//    void sendTextMessage(const QString &message);

//signals:
//    void messageReceived(const QString &message);

//private slots:
//    void onNewConnection();
//    void onTextMessageReceived(const QString &message);
//    void onDisconnected();

//private:
//    QWebSocketServer *m_server;
//    QMap<QWebSocket*, QString> m_clients; // Mapping of clients
//};

//#endif // SIGNALINGSERVER_H
