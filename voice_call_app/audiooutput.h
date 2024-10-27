#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioSink>
#include <QMutex>
#include <opus.h>

class AudioOutput : public QObject {
    Q_OBJECT

public:
    explicit AudioOutput(QObject *parent = nullptr);
    ~AudioOutput() override;

    void playAudio(const QByteArray &data);

private:
    QByteArray decodeAudio(const QByteArray &input);

    QAudioSink *audioSink;
    QIODevice *outputDevice;
    QMutex mutex;
    OpusDecoder *opusDecoder;
};

#endif // AUDIOOUTPUT_H