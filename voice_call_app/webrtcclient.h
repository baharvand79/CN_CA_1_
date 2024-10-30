#ifndef WEBRTCCLIENT_H
#define WEBRTCCLIENT_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebSockets/QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

class WebRTCClient : public QObject {
    Q_OBJECT // Necessary for QObject-derived classes
public:
    WebRTCClient(QObject *parent = nullptr) : QObject(parent) {
        connect(&webSocket, &QWebSocket::connected, this, &WebRTCClient::onConnected);
        connect(&webSocket, &QWebSocket::disconnected, this, &WebRTCClient::onDisconnected);
        connect(&webSocket, &QWebSocket::textMessageReceived, this, &WebRTCClient::onMessageReceived);

        webSocket.open(QUrl("ws://localhost:3000")); // Adjust the URL as necessary
    }

    // Use Q_INVOKABLE to make methods callable from QML
    Q_INVOKABLE void registerClient(const QString &clientId) {
        QJsonObject message;
        message["type"] = "register";
        message["id"] = clientId;
        webSocket.sendTextMessage(QJsonDocument(message).toJson());
    }

    Q_INVOKABLE void sendOffer(const QString &targetId, const QString &offer) {
        QJsonObject message;
        message["type"] = "offer";
        message["targetId"] = targetId;
        message["offer"] = offer;
        webSocket.sendTextMessage(QJsonDocument(message).toJson());
    }

    Q_INVOKABLE void sendAnswer(const QString &targetId, const QString &answer) {
        QJsonObject message;
        message["type"] = "answer";
        message["targetId"] = targetId;
        message["answer"] = answer;
        webSocket.sendTextMessage(QJsonDocument(message).toJson());
    }

    Q_INVOKABLE void sendCandidate(const QString &targetId, const QString &candidate) {
        QJsonObject message;
        message["type"] = "candidate";
        message["targetId"] = targetId;
        message["candidate"] = candidate;
        webSocket.sendTextMessage(QJsonDocument(message).toJson());
    }

private slots:
    void onConnected() {
        qDebug("WebSocket connected");
    }

    void onDisconnected() {
        qDebug("WebSocket disconnected");
    }

    void onMessageReceived(const QString &message) {
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
        QJsonObject json = doc.object();
        QString type = json["type"].toString();

        if (type == "offer") {
            QString offer = json["offer"].toString();
            // Handle the offer (e.g., set local description, create answer)
        } else if (type == "answer") {
            QString answer = json["answer"].toString();
            // Handle the answer (e.g., set remote description)
        } else if (type == "candidate") {
            QString candidate = json["candidate"].toString();
            // Handle the ICE candidate
        }
    }

private:
    QWebSocket webSocket;
};

#endif // WEBRTCCLIENT_H
