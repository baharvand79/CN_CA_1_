#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QIODevice>
#include <QAudioFormat>
#include <QAudioSink>
#include <QMediaDevices>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>
#include <QDebug>
#include <opus/include/opus.h>

class AudioOutput : public QObject {
    Q_OBJECT

public:
    explicit AudioOutput(QObject *parent = nullptr);
    void addData(const QByteArray &data);
    void play();

signals:
    void newPacket();

private:
    QAudioSink *audioSink;
    QAudioFormat audioFormat;
    QQueue<QByteArray> audioQueue;
    QMutex mutex;
    OpusDecoder *opusDecoder;

    void processAudio(const QByteArray &data);
};

#endif // AUDIOOUTPUT_H
