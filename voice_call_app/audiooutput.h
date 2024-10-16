#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QAudioSink>
#include <QIODevice>
#include <QMutex>

class AudioOutput : public QObject {
    Q_OBJECT
public:
    explicit AudioOutput(QObject *parent = nullptr);
    ~AudioOutput();

public slots:
    void playAudio(const QByteArray &data);

private:
    QAudioSink *audioSink;
    QIODevice *outputDevice;
    QMutex mutex;
};

#endif // AUDIOOUTPUT_H
