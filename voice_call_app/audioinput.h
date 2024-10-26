#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QIODevice>
#include <QAudioSource>
#include <opus.h>
#include <QDebug>

class AudioInput : public QIODevice {
    Q_OBJECT

public:
    explicit AudioInput(QObject *parent = nullptr);
    ~AudioInput();

protected:
    qint64 writeData(const char *data, qint64 len) override;
    qint64 readData(char *data, qint64 len) override;

private:
    OpusEncoder *encoder;
    QAudioSource *audioSource;
    int error;
};

#endif // AUDIOINPUT_H
