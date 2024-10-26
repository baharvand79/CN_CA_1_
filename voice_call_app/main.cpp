#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "webrtc.h"
#include "signalingserver.h"
#include "signalingclient.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qDebug() << "The program is running.\n";

    // Start the signaling server
    SignalingServer *signalingServer = new SignalingServer(12345);
    qDebug() << "[MAIN] Signaling server started on port 12345";

    // Create WebRTC instance
    WebRTC *webRTC = new WebRTC();

    // Connect WebRTC signals to slots for logging or handling UI updates
    QObject::connect(webRTC, &WebRTC::localDescriptionGenerated,
                     [](const QString &sdp) {
                         qDebug() << "[MAIN] Local SDP:" << sdp;
                     });

    QObject::connect(webRTC, &WebRTC::localCandidateGenerated,
                     [](const QString &candidate) {
                         qDebug() << "[MAIN] Local Candidate:" << candidate;
                     });

    QObject::connect(webRTC, &WebRTC::audioReceived,
                     [](const QByteArray &audioData) {
                         qDebug() << "[MAIN] Audio data received:" << audioData.size() << " bytes.";
                     });

    QObject::connect(webRTC, &WebRTC::gatheringCompleted,
                     []() {
                         qDebug() << "[MAIN] Gathering of ICE candidates completed.";
                     });

    // Create a signaling client and connect to the server
    QUrl serverUrl(QStringLiteral("ws://localhost:12345"));
    SignalingClient *client = new SignalingClient(serverUrl, webRTC);
    qDebug() << "[MAIN] Signaling client started and attempting to connect to" << serverUrl.toString();

    // Connect client signals to handle incoming messages
    QObject::connect(client, &SignalingClient::messageReceived,
                     [webRTC](const QString &message) {
                         // Handle incoming messages
                         qDebug() << "[MAIN] Message received from client:" << message;
                     });

    // Create an SDP offer
    webRTC->createOffer();

    // Send a test message to all connected clients (optional)
    signalingServer->sendTestMessage("Hello, this is a test message from the server.");

    // Start the application event loop
    return app.exec();
}
