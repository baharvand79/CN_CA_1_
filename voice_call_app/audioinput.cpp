#include "audioinput.h"

AudioInput::AudioInput(QObject *parent)
    : QIODevice(parent)
{
    // init steps for opus
    encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP,  &error);

    if (error != OPUS_OK){
        qCritical() << "Failed to create Opus encoder: " << opus_strerror(error);
    } else {
        qDebug() << "Opus encoder is created.";
    }

    // init source
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    audioSource = new QAudioSource(format, this);
    if (!this->isOpen()) {
        qDebug() << "Audio source initialized successfully.";
        this->open(QIODevice::WriteOnly);
    } else {
        qCritical() << "Failed to initialize audio source.";
    }

    audioSource->start(this);
}

qint64 AudioInput::writeData(const char *data, qint64 len)
{
    const int maxPacketSize = 4000;
    unsigned char encodedData[maxPacketSize];

    int encodedBytes = opus_encode(encoder, reinterpret_cast<const opus_int16*>(data), len / 2, encodedData, maxPacketSize);

    if (encodedBytes < 0) {
        qDebug() << "Opus encoding failed with error:" << encodedBytes;
        return -1;
    }

    qDebug() << "Encoded" << encodedBytes << "bytes of audio data.";

    return len;
}
