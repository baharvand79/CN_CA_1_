#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QDebug>

class SignalingServer : public QObject {
    Q_OBJECT

public:
    explicit SignalingServer(quint16 port, QObject *parent = nullptr);
    ~SignalingServer();

signals:
    void messageReceived(const QString &message, QWebSocket *client);

public slots:
    void onNewConnection();
    void onTextMessageReceived(const QString &message);
    void onDisconnected();

private:
    QWebSocketServer *m_server;
    QMap<QWebSocket*, QString> m_clients;
};
