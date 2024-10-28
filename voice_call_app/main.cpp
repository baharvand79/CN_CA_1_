//#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include <QDebug>
//#include "AudioInput.h"
//#include "AudioOutput.h"
//#include "webrtc.h"
//#include "signalingserver.h"
//#include "signalingclient.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QWebSocket>
#include <QDebug> // Include qDebug for console output
#include "signalingserver.h"
#include "webrtc.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    quint16 signalingPort = 12345; // Set the port for the signaling server

    // Create the signaling server
    SignalingServer signalingServer(signalingPort);
    qDebug() << "Signaling server started on port:" << signalingPort;

    // Create a list to hold WebRTC peers
    QList<WebRTC*> peers;

    // Setup peers
    for (int i = 0; i < 2; ++i) {
        WebRTC *peer = new WebRTC(&signalingServer); // Pass the signaling server to each peer
        peers.append(peer);

        // Connect signals to handle local description generation
        QObject::connect(peer, &WebRTC::localDescriptionGenerated, [&signalingServer, i](const QString &sdp) {
            QJsonObject jsonObj;
            jsonObj["peerIndex"] = i;
            jsonObj["offer"] = sdp;
            signalingServer.sendTextMessage(QJsonDocument(jsonObj).toJson(QJsonDocument::Compact));
            qDebug() << "Peer ID:" << i << "generated offer:" << sdp; // Output the generated offer
        });

        // Connect signals to handle local candidate generation
        QObject::connect(peer, &WebRTC::localCandidateGenerated, [&signalingServer, i](const QString &candidate) {
            QJsonObject jsonObj;
            jsonObj["peerIndex"] = i;
            jsonObj["candidate"] = candidate;
            signalingServer.sendTextMessage(QJsonDocument(jsonObj).toJson(QJsonDocument::Compact));
            qDebug() << "Peer ID:" << i << "generated candidate:" << candidate; // Output the generated candidate
        });

        // Create the initial offer for the peer
        peer->createOffer(); // Initiate the offer for each peer
    }

    // Connect to the signaling server's messageReceived signal
    QObject::connect(&signalingServer, &SignalingServer::messageReceived, [&peers](const QString &message) {
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
        QJsonObject jsonObj = doc.object();

        // Process offers and candidates received from the signaling server
        if (jsonObj.contains("offer")) {
            int peerIndex = jsonObj["peerIndex"].toInt();
            if (peerIndex >= 0 && peerIndex < peers.size()) {
                QString offer = jsonObj["offer"].toString();
                peers[peerIndex]->setRemoteDescription(offer);
                qDebug() << "Received offer for Peer ID:" << peerIndex << "SDP:" << offer; // Output the received offer
            }
        } else if (jsonObj.contains("candidate")) {
            int peerIndex = jsonObj["peerIndex"].toInt();
            if (peerIndex >= 0 && peerIndex < peers.size()) {
                QString candidate = jsonObj["candidate"].toString();
                peers[peerIndex]->addRemoteCandidate(candidate);
                qDebug() << "Received candidate for Peer ID:" << peerIndex << "Candidate:" << candidate; // Output the received candidate
            }
        }
    });

    // Start the application event loop
    return app.exec();
}

//int main(int argc, char *argv[]) {

//    QGuiApplication app(argc, argv);

    // qDebug() << "Starting WebRTC application.";

    // // Initialize signaling server
    // SignalingServer signalingServer(12345);
    // qDebug() << "[MAIN] Signaling server started on port 12345.";

    // // Initialize WebRTC instance
    // WebRTC webRTC;

    // // Connect WebRTC signals to slots
    // QObject::connect(&webRTC, &WebRTC::localDescriptionGenerated, [](const QString &sdp) {
    //     qDebug() << "[MAIN] Local SDP generated:" << sdp;
    // });
    // QObject::connect(&webRTC, &WebRTC::localCandidateGenerated, [](const QString &candidate) {
    //     qDebug() << "[MAIN] Local ICE candidate generated:" << candidate;
    // });
    // QObject::connect(&webRTC, &WebRTC::audioReceived, [](const QByteArray &audioData) {
    //     qDebug() << "[MAIN] Received audio data of size:" << audioData.size() << "bytes.";
    // });
    // QObject::connect(&webRTC, &WebRTC::gatheringCompleted, []() {
    //     qDebug() << "[MAIN] ICE candidate gathering completed.";
    // });

    // // Set up signaling client to connect to signaling server
    // QUrl serverUrl("ws://localhost:12345");
    // SignalingClient client(serverUrl, &webRTC);
    // qDebug() << "[MAIN] Signaling client attempting to connect to" << serverUrl.toString();

    // // Handle messages received by the signaling client
    // QObject::connect(&client, &SignalingClient::messageReceived, &webRTC, [](const QString &message) {
    //     qDebug() << "[MAIN] Message received from signaling server:" << message;
    // });

    // // Create initial SDP offer
    // webRTC.createOffer();

    // // Send a test message to connected clients
    // signalingServer.sendTestMessage("Welcome to the WebRTC signaling server!");

//    qmlRegisterType<AudioInput>("Audio", 1, 0, "AudioInput");
//    qmlRegisterType<AudioOutput>("Audio", 1, 0, "AudioOutput");
//    qmlRegisterType<WebRTC>("WebRTC", 1, 0, "RTC");


//    QQmlApplicationEngine engine;
//    const QUrl url(u"qrc:/Main.qml"_qs);

//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() {QCoreApplication::exit(-1);}, Qt::QueuedConnection);
//    engine.load(url);

//    return app.exec();
//}



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
