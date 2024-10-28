#include "signalingserver.h"
#include <QMap>
SignalingServer::SignalingServer(quint16 port, QObject *parent)
    : QObject(parent), m_server(new QWebSocketServer(QStringLiteral("Signaling Server"), QWebSocketServer::NonSecureMode, this)) {

    if (m_server->listen(QHostAddress::Any, port)) {
        connect(m_server, &QWebSocketServer::newConnection, this, &SignalingServer::onNewConnection);
        qDebug() << "[SignalingServer] Server is listening on port" << port;
    } else {
        qDebug() << "[SignalingServer] Failed to start server!";
    }
}

SignalingServer::~SignalingServer() {
    m_server->close();
    for (auto client : m_clients.keys()) {
        delete client;
    }
    m_clients.clear();
    qDebug() << "[SignalingServer] Server and clients cleaned up.";
}


void SignalingServer::onNewConnection() {
    QWebSocket *client = m_server->nextPendingConnection();
    connect(client, &QWebSocket::textMessageReceived, this, &SignalingServer::onTextMessageReceived);
    connect(client, &QWebSocket::disconnected, this, &SignalingServer::onDisconnected);
    m_clients[client] = "";
    qDebug() << "[SignalingServer] New client connected.";
}

//void SignalingServer::onTextMessageReceived(const QString &message) {
//    QWebSocket *senderClient = qobject_cast<QWebSocket *>(sender());
//    if (senderClient) {
//        emit messageReceived(message, senderClient);
//        // Broadcast message to other clients
//        for (QWebSocket *client : m_clients.keys()) {
//            if (client != senderClient) {
//                client->sendTextMessage(message);
//                qDebug() << "[SignalingServer] Forwarded message to client:" << client;
//            }
//        }
//    }
//}

void SignalingServer::onTextMessageReceived(const QString &message) {
    QWebSocket *senderClient = qobject_cast<QWebSocket *>(sender());
    if (senderClient) {
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
        QJsonObject jsonObj = doc.object();

        // Forward messages to other clients based on type
        for (QWebSocket *client : m_clients.keys()) {
            if (client != senderClient) {
                client->sendTextMessage(message);
            }
        }
    }
}


void SignalingServer::onDisconnected() {
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        m_clients.remove(client);
        client->deleteLater();
        qDebug() << "[SignalingServer] Client disconnected.";
    }
}

void SignalingServer::sendTextMessage(const QString &message) {
    for (QWebSocket *client : m_clients.keys()) {
        client->sendTextMessage(message);
        qDebug() << "[SignalingServer] Sent test message to client:" << client << " Message:" << message;
    }
}
