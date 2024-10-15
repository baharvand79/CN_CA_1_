#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QIODevice>
#include <QAudioSource>
#include <QDebug>
#include <QObject>
#include <opus/include/opus.h>

class AudioInput: public QIODevice
{
    Q_OBJECT
public:
    AudioInput(QObject *parent = nullptr);

private:
    OpusEncoder *encoder;
    QAudioSource *audioSource;
    int error;
};

#endif // AUDIOINPUT_H
