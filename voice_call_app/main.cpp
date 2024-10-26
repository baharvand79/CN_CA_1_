#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "webrtc.cpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qDebug() << "The program is running.\n";

    WebRTC *webRTC = new WebRTC();
    webRTC->init();
    return app.exec();

}
