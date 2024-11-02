#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioSource>
#include <opus.h>

class AudioInput : public QIODevice {
    Q_OBJECT

public:
    explicit AudioInput(QObject *parent = nullptr);
    ~AudioInput() override;
    Q_INVOKABLE void startCapture();

public: Q_SIGNALS:
    void audioCaptured(const QByteArray &data);
    void debugMessage(const QString &message);

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:
    QByteArray encodeAudio(const QByteArray &input);

    QAudioSource *audioSource;
    QIODevice *audioDevice;
    OpusEncoder *opusEncoder;
};

#endif // AUDIOINPUT_H
