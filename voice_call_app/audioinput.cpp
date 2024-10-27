#include <opus.h> // Include Opus header
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
    audioDevice = audioSource->start();

    // Opus encoder initialization
    int error;
    opusEncoder = opus_encoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNELS, OPUS_APPLICATION_VOIP, &error);

    connect(audioDevice, &QIODevice::readyRead, this, [this]() {
        QByteArray audioData = audioDevice->readAll();
        QByteArray encodedData = encodeAudio(audioData);
        emit audioCaptured(encodedData);
    });
}

AudioInput::~AudioInput() {
    opus_encoder_destroy(opusEncoder);
    audioSource->stop();
}

QByteArray AudioInput::encodeAudio(const QByteArray &input) {
    QByteArray output;
    int maxEncodedBytes = 4000; // Adjust based on Opus encoding specs
    unsigned char encodedData[maxEncodedBytes];

    int frameSize = input.size() / (OPUS_CHANNELS * sizeof(int16_t)); // Number of samples
    int encodedBytes = opus_encode(opusEncoder, reinterpret_cast<const opus_int16*>(input.constData()), frameSize, encodedData, maxEncodedBytes);

    if (encodedBytes > 0) {
        output.append(reinterpret_cast<const char*>(encodedData), encodedBytes);
    }

    return output;
}

qint64 AudioInput::readData(char *data, qint64 maxlen) {
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return 0; // No data to read
}

qint64 AudioInput::writeData(const char *data, qint64 len) {
    emit audioCaptured(QByteArray(data, len)); // Emit captured audio data
    return len;
}

void AudioInput::startCapture() {
    audioSource->start(); // Start audio capture
}
