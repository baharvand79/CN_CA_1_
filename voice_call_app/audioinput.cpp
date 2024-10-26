#include "audioinput.h"

AudioInput::AudioInput(QObject *parent)
    : QIODevice(parent), encoder(nullptr), audioSource(nullptr), error(0) {

    // Initialize Opus encoder
    encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, &error);

    if (error != OPUS_OK) {
        qCritical() << "Failed to create Opus encoder:" << opus_strerror(error);
        return; // Exit if the encoder creation failed
    } else {
        qDebug() << "Opus encoder is created.";
    }

    // Initialize audio source
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16); // Ensure this matches Opus expected input

    audioSource = new QAudioSource(format, this);
    if (!audioSource) {
        qCritical() << "Failed to initialize audio source.";
        return; // Exit if the audio source fails to initialize
    }

    audioSource->start(this); // Start capturing audio
    this->open(QIODevice::WriteOnly); // Open this QIODevice in WriteOnly mode
    qDebug() << "Audio source initialized successfully.";
}

AudioInput::~AudioInput() {
    if (encoder) {
        opus_encoder_destroy(encoder); // Clean up Opus encoder
    }
    if (audioSource) {
        audioSource->stop(); // Stop the audio source before destruction
    }
}

qint64 AudioInput::writeData(const char *data, qint64 len) {
    const int maxPacketSize = 4000; // Max size of encoded packet
    unsigned char encodedData[maxPacketSize];

    int encodedBytes = opus_encode(encoder, reinterpret_cast<const opus_int16*>(data), len / 2, encodedData, maxPacketSize);

    if (encodedBytes < 0) {
        qDebug() << "Opus encoding failed with error:" << encodedBytes;
        return -1; // Return an error if encoding fails
    }

    qDebug() << "Encoded" << encodedBytes << "bytes of audio data.";

    // Here you can emit the encoded data as a signal to the rest of your application
    emit audioCaptured(QByteArray(reinterpret_cast<const char*>(encodedData), encodedBytes));

    return len; // Return the number of bytes processed
}

qint64 AudioInput::readData(char *data, qint64 len) {
    Q_UNUSED(data);
    Q_UNUSED(len);
    return 0;
}
