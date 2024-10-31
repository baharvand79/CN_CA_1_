#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QObject>
#include "webrtc.h"
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(u"qrc:/Main.qml"_qs);
    qmlRegisterType<WebRTC>("WebRTC", 1, 0, "WebRTC");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() {QCoreApplication::exit(-1);}, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

//#include "main.moc"
