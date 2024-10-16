#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QAudioSource>
#include <QIODevice>
#include <QObject>

class AudioInput : public QIODevice {
    Q_OBJECT
public:
    AudioInput(QObject *parent = nullptr);
    ~AudioInput();

    void startCapture();

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

signals:
    void audioCaptured(const QByteArray &data);

private:
    QAudioSource *audioSource;
    QIODevice *audioDevice;
};

#endif // AUDIOINPUT_H
