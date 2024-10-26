#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "webrtc.cpp"
#include "signalingserver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qDebug() << "The program is running.\n";

    // Start the signaling server
    SignalingServer *signalingServer = new SignalingServer(12345);
    qDebug() << "[MAIN] Signaling server started on port 12345";


    WebRTC *webRTC = new WebRTC();

    // Connect signals to slots for logging or handling UI updates
    QObject::connect(webRTC, &WebRTC::localDescriptionGenerated,
                     [](const QString &sdp) {
                         qDebug()  << "[MAIN] Local SDP: " << sdp.toStdString() << "\n";
                     });

    QObject::connect(webRTC, &WebRTC::localCandidateGenerated,
                     [](const QString &candidate) {
                         qDebug()  << "[MAIN] Local Candidate: " << candidate.toStdString()<< "\n";
                     });

    QObject::connect(webRTC, &WebRTC::audioReceived,
                     [](const QByteArray &audioData) {
                         qDebug() << "[MAIN] Audio data received: " << audioData.size() << " bytes."<< "\n";
                     });

    QObject::connect(webRTC, &WebRTC::gatheringCompleted,
                     []() {
                         qDebug()  << "[MAIN] Gathering of ICE candidates completed."<< "\n";
                     });

    webRTC -> createOffer();

    return app.exec();

}
