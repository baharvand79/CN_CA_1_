//#include <QGuiApplication>
//#include <QQmlApplicationEngine>


//int main(int argc, char *argv[])
//{
//#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//#endif
//    QGuiApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    const QUrl url(QStringLiteral("qrc:/main.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);
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


#include <QCoreApplication>
#include "webrtc.h"

int main(int argc, char *argv[]){
    QCoreApplication app(argc, argv);

    WebRTC *webrtcInstance = new WebRTC();

       QString localId = "localPeer";
       bool isOfferer = true;
       webrtcInstance->init(localId, isOfferer);

       // Add a peer to connect to
       QString peerId = "peer1";
       webrtcInstance->addPeer(peerId);

       return app.exec();
}
