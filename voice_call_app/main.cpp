#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "webrtc.h"
#include "signalingserver.h"
#include "signalingclient.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qDebug() << "Starting WebRTC application.";

    // Initialize signaling server
    SignalingServer signalingServer(12345);
    qDebug() << "[MAIN] Signaling server started on port 12345.";

    // Initialize WebRTC instance
    WebRTC webRTC;

    // Connect WebRTC signals to slots
    QObject::connect(&webRTC, &WebRTC::localDescriptionGenerated, [](const QString &sdp) {
        qDebug() << "[MAIN] Local SDP generated:" << sdp;
    });
    QObject::connect(&webRTC, &WebRTC::localCandidateGenerated, [](const QString &candidate) {
        qDebug() << "[MAIN] Local ICE candidate generated:" << candidate;
    });
    QObject::connect(&webRTC, &WebRTC::audioReceived, [](const QByteArray &audioData) {
        qDebug() << "[MAIN] Received audio data of size:" << audioData.size() << "bytes.";
    });
    QObject::connect(&webRTC, &WebRTC::gatheringCompleted, []() {
        qDebug() << "[MAIN] ICE candidate gathering completed.";
    });

    // Set up signaling client to connect to signaling server
    QUrl serverUrl("ws://localhost:12345");
    SignalingClient client(serverUrl, &webRTC);
    qDebug() << "[MAIN] Signaling client attempting to connect to" << serverUrl.toString();

    // Handle messages received by the signaling client
    QObject::connect(&client, &SignalingClient::messageReceived, &webRTC, [](const QString &message) {
        qDebug() << "[MAIN] Message received from signaling server:" << message;
    });

    // Create initial SDP offer
    webRTC.createOffer();

    // Send a test message to connected clients
    signalingServer.sendTestMessage("Welcome to the WebRTC signaling server!");

    return app.exec();
}



//#include <QCoreApplication>
//#include "AudioInput.h"
//#include "AudioOutput.h"

//int main(int argc, char *argv[]) {
//    QCoreApplication a(argc, argv);

//    AudioInput audioInput;
//    AudioOutput audioOutput;

//    QObject::connect(&audioInput, &AudioInput::audioCaptured, &audioOutput, &AudioOutput::playAudio);

//    audioInput.startCapture();

//    return a.exec();
//}


//#include <QCoreApplication>
//#include "webrtc.h"
//#include "signalingserver.h"

//int main(int argc, char *argv[]) {
//    QCoreApplication a(argc, argv);

//    WebRTC webRTC; // Create WebRTC instance
//    SignalingServer signalingServer(12345); // Start signaling server on port 12345

//    // Connect signaling server signals to WebRTC slots
//     QObject::connect(&signalingServer, &SignalingServer::messageReceived,
//                      &webRTC, &WebRTC::setRemoteDescription);

//     QObject::connect(&signalingServer, &SignalingServer::messageReceived,
//                      &webRTC, &WebRTC::addRemoteCandidate);

//     // Setup other connections if needed
//     QObject::connect(&webRTC, &WebRTC::localDescriptionGenerated,
//                      &signalingServer, &SignalingServer::sendLocalDescription);

//     QObject::connect(&webRTC, &WebRTC::localCandidateGenerated,
//                      &signalingServer, &SignalingServer::sendLocalCandidate);

//     QObject::connect(&webRTC, &WebRTC::audioReceived,
//                      [&webRTC](const QByteArray &audioData) {

//                      });

//     webRTC.startAudioCapture();

//    return a.exec();
//}
