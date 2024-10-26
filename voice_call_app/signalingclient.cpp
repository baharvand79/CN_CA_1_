#include "signalingclient.h"
#include <QDebug>

SignalingClient::SignalingClient(const QUrl &url, QObject *parent)
    : QObject(parent), m_url(url) {
    connect(&m_webSocket, &QWebSocket::connected, this, &SignalingClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &SignalingClient::onDisconnected);
    m_webSocket.open(m_url); // Start the connection
}

void SignalingClient::sendMessage(const QString &message) {
    if (m_webSocket.isValid()) {
        m_webSocket.sendTextMessage(message);
        qDebug() << "[SignalingClient] Sent message:" << message;
    } else {
        qDebug() << "[SignalingClient] WebSocket is not connected.";
    }
}

void SignalingClient::onConnected() {
    qDebug() << "[SignalingClient] Connected to server.";
    sendMessage("Hello from client!"); // Send initial message to server
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &SignalingClient::onTextMessageReceived);
}

void SignalingClient::onTextMessageReceived(const QString &message) {
    qDebug() << "[SignalingClient] Message received from server:" << message;
}

void SignalingClient::onDisconnected() {
    qDebug() << "[SignalingClient] Disconnected from server.";
}
