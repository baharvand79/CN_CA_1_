#include <opus.h>
#include "AudioInput.h"
#include <QDebug>

const int OPUS_SAMPLE_RATE = 48000;
const int OPUS_CHANNELS = 2;

AudioInput::AudioInput(QObject *parent) : QIODevice(parent) {
    Q_EMIT debugMessage("[AudioInput] Initializing...");
    qDebug() << "[AudioInput] Initializing...";

    QAudioFormat format;
    format.setSampleRate(OPUS_SAMPLE_RATE);
    format.setChannelCount(OPUS_CHANNELS);
    format.setSampleFormat(QAudioFormat::Int16);

    audioSource = new QAudioSource(format, this);

    int error;
    opusEncoder = opus_encoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNELS, OPUS_APPLICATION_AUDIO, &error);
    if (error != OPUS_OK) {
        Q_EMIT debugMessage("[AudioInput] Failed to initialize Opus encoder");
        qDebug() << "[AudioInput] Failed to initialize Opus encoder";
    } else {
        opus_encoder_ctl(opusEncoder, OPUS_SET_BITRATE(128000));
        opus_encoder_ctl(opusEncoder, OPUS_SET_COMPLEXITY(10));
        opus_encoder_ctl(opusEncoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
        Q_EMIT debugMessage("[AudioInput] Opus encoder initialized successfully");
        qDebug() << "[AudioInput] Opus encoder initialized successfully";
    }
}

AudioInput::~AudioInput() {
    opus_encoder_destroy(opusEncoder);
    audioSource->stop();
}

void AudioInput::stopCapture(){
    audioSource->stop();
}
void AudioInput::startCapture() {
    Q_EMIT debugMessage("[AudioInput] Starting capture...");
    qDebug() << "[AudioInput] Starting capture...";

    audioDevice = audioSource->start(); // Start capturing audio
    if (!audioDevice) {
        Q_EMIT debugMessage("[AudioInput] Failed to start audio source.");
        qDebug() << "[AudioInput] Failed to start audio source.";
        return;
    }

    // Connect readyRead to the slot or lambda function to handle incoming audio data
    connect(audioDevice, &QIODevice::readyRead, this, [this]() {
        QByteArray audioData = audioDevice->readAll();
        Q_EMIT debugMessage("[AudioInput] Captured raw audio data size: " + QString::number(audioData.size()));
        qDebug() << "[AudioInput] Captured raw audio data size:" << audioData.size();

        QByteArray encodedData = encodeAudio(audioData);
        Q_EMIT debugMessage("[AudioInput] Captured processed audio data size: " + QString::number(encodedData.size()));
        qDebug() << "[AudioInput] Captured processed audio data size:" << encodedData.size();

        if (encodedData.size() > 0) {
            Q_EMIT audioCaptured(encodedData);
        }
    });
}

qint64 AudioInput::readData(char *data, qint64 maxlen) {
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return 0;
}

qint64 AudioInput::writeData(const char *data, qint64 len) {
    Q_EMIT audioCaptured(QByteArray(data, len));
    return len;
}

QByteArray AudioInput::encodeAudio(const QByteArray &input) {
    QByteArray output;
    int maxEncodedBytes = 4000;
    unsigned char encodedData[maxEncodedBytes];

    // Frame size calculation based on input data size
    int frameSize = input.size() / (OPUS_CHANNELS * sizeof(int16_t));

    int encodedBytes = opus_encode(opusEncoder, reinterpret_cast<const opus_int16*>(input.constData()), frameSize, encodedData, maxEncodedBytes);
    if (encodedBytes > 0) {
        output.append(reinterpret_cast<const char*>(encodedData), encodedBytes);
    }
    return output;
}

