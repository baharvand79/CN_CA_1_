//#include "signalingclient.h"
//#include <QDebug>

//SignalingClient::SignalingClient(const QUrl &url, QObject *parent)
//    : QObject(parent), m_url(url) {
//    connect(&m_webSocket, &QWebSocket::connected, this, &SignalingClient::onConnected);
//    connect(&m_webSocket, &QWebSocket::disconnected, this, &SignalingClient::onDisconnected);
//    m_webSocket.open(m_url); // Start the connection
//}

//void SignalingClient::sendMessage(const QString &message) {
//    if (m_webSocket.isValid()) {
//        m_webSocket.sendTextMessage(message);
//        qDebug() << "[SignalingClient] Sent message:" << message;
//    } else {
//        qDebug() << "[SignalingClient] WebSocket is not connected.";
//    }
//}

//void SignalingClient::sendMessage(MessageType type, const QString &content) {
//    QJsonObject message = createMessage(type, content);
//    m_webSocket.sendTextMessage(QJsonDocument(message).toJson());
//}


//void SignalingClient::onConnected() {
//    qDebug() << "[SignalingClient] Connected to server.";
//    sendMessage("Hello from client!"); // Send initial message to server
//    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &SignalingClient::onTextMessageReceived);
//}

////void SignalingClient::onTextMessageReceived(const QString &message) {
////    qDebug() << "[SignalingClient] Message received from server:" << message;
////}

//void SignalingClient::onTextMessageReceived(const QString &message) {
//    emit messageReceived(message); // Emit the signal when a message is received

//    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
//    QJsonObject jsonObj = doc.object();

//    MessageType type = static_cast<MessageType>(jsonObj["type"].toInt());
//    QString content = jsonObj["content"].toString();

//    switch (type) {
//        case MessageType::Offer:
//            m_webRTC->setRemoteDescription(content);
//            break;
//        case MessageType::Answer:
//            m_webRTC->setRemoteDescription(content);
//            break;
//        case MessageType::Candidate:
//            m_webRTC->addRemoteCandidate(content);
//            break;
//        case MessageType::ChatMessage:
//            qDebug() << "[SignalingClient] Chat message received:" << content;
//            break;
//        default:
//            break;
//    }
//}



//void SignalingClient::onDisconnected() {
//    qDebug() << "[SignalingClient] Disconnected from server.";
//}


//// Define a simple JSON structure for messages
//QJsonObject SignalingClient::createMessage(MessageType type, const QString& content) {
//    QJsonObject msg;
//    msg["type"] = static_cast<int>(type);
//    msg["content"] = content;
//    return msg;
//}
