#include "AudioInput.h"

// Constants for Opus encoding
const int OPUS_SAMPLE_RATE = 48000;
const int OPUS_CHANNELS = 2;
const int OPUS_FRAME_SIZE = 960; // for 20ms frames at 48kHz

AudioInput::AudioInput(QObject *parent) : QIODevice(parent) {
    QAudioFormat format;
    format.setSampleRate(OPUS_SAMPLE_RATE);
    format.setChannelCount(OPUS_CHANNELS);
    format.setSampleFormat(QAudioFormat::Int16);

    audioSource = new QAudioSource(format, this);

    // Opus encoder initialization
    int error;
    opusEncoder = opus_encoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNELS, OPUS_APPLICATION_VOIP, &error);

    connect(audioSource, &QAudioSource::stateChanged, this, [this]() {
        if (audioSource->state() == QAudio::ActiveState) {
            audioDevice = audioSource->start();
            connect(audioDevice, &QIODevice::readyRead, this, [this]() {
                QByteArray audioData = audioDevice->readAll();
                QByteArray encodedData = encodeAudio(audioData);
                emit audioCaptured(encodedData);
            });
        } else {
            // Handle stopping of audio capture if necessary
            audioDevice = nullptr;
        }
    });
}

AudioInput::~AudioInput() {
    opus_encoder_destroy(opusEncoder);
    audioSource->stop();
}

void AudioInput::startCapture() {
    if (audioSource) {
        audioSource->start();
    }
}

void AudioInput::stopCapture() {
    if (audioSource) {
        audioSource->stop(); // Stop audio capture
        // Optionally: disconnect audioDevice if you don't need it anymore
        if (audioDevice) {
            audioDevice->disconnect(); // Clean up
            audioDevice = nullptr;
        }
    }
}

QByteArray AudioInput::encodeAudio(const QByteArray &input) {
    QByteArray output;
    int maxEncodedBytes = 4000;
    unsigned char encodedData[maxEncodedBytes];

    int frameSize = input.size() / (OPUS_CHANNELS * sizeof(int16_t));
    int encodedBytes = opus_encode(opusEncoder, reinterpret_cast<const opus_int16*>(input.constData()), frameSize, encodedData, maxEncodedBytes);

    if (encodedBytes > 0) {
        output.append(reinterpret_cast<const char*>(encodedData), encodedBytes);
    }

    return output;
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
