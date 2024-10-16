#include "AudioInput.h"

AudioInput::AudioInput(QObject *parent) : QIODevice(parent) {
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);

    audioSource = new QAudioSource(format, this);
    audioDevice = audioSource->start();

    connect(audioDevice, &QIODevice::readyRead, this, [this]() {
        QByteArray audioData = audioDevice->readAll();
        emit audioCaptured(audioData);
    });
}

AudioInput::~AudioInput() {
    audioSource->stop();
}

qint64 AudioInput::readData(char *data, qint64 maxlen) {
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return 0;
}

qint64 AudioInput::writeData(const char *data, qint64 len) {
    emit audioCaptured(QByteArray(data, len));
    return len;
}

void AudioInput::startCapture() {
    audioSource->start();
}
