#ifndef SIGNALINGCLIENT_H
#define SIGNALINGCLIENT_H

#include <QObject>
#include <QWebSocket>

class SignalingClient : public QObject {
    Q_OBJECT
public:
    explicit SignalingClient(const QUrl &url, QObject *parent = nullptr);

public slots:
    void sendMessage(const QString &message);

private slots:
    void onConnected();
    void onTextMessageReceived(const QString &message);
    void onDisconnected();

private:
    QWebSocket m_webSocket;
    QUrl m_url;
};

#endif // SIGNALINGCLIENT_H
