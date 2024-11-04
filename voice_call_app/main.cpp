#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include "webrtc.h"
#include "audioinput.h"
#include "audiooutput.h"
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(u"qrc:/Main.qml"_qs);
    qmlRegisterType<WebRTC>("WebRTC", 1, 0, "WebRTC");
    qmlRegisterType<AudioInput>("AudioInput", 1, 0, "AudioInput");
    qmlRegisterType<AudioOutput>("AudioOutput", 1, 0, "AudioOutput");
    qRegisterMetaType<QByteArray>("QByteArray");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() {QCoreApplication::exit(-1);}, Qt::QueuedConnection);
    engine.load(url);

//    QCoreApplication app(argc, argv);

//    AudioInput audioInput;
//    AudioOutput audioOutput;

//    QObject::connect(&audioInput, &AudioInput::audioCaptured, &audioOutput, &AudioOutput::playAudio);

//    audioInput.startCapture();


    return app.exec();
}

//#include "main.moc"

