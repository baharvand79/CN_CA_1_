#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QAudioSource>
#include <QIODevice>
#include <QObject>
#include <opus.h>

class AudioInput : public QIODevice {
    Q_OBJECT
public:
    explicit AudioInput(QObject *parent = nullptr);
    ~AudioInput();

    void startCapture();
    void stopCapture();

signals:
    void audioCaptured(const QByteArray &data);

protected:
    // QIODevice reimplementation
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:
    QByteArray encodeAudio(const QByteArray &input);
    QAudioSource *audioSource;
    QIODevice *audioDevice;
    OpusEncoder *opusEncoder;
};

#endif // AUDIOINPUT_H
