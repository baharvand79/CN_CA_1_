#include <opus.h>
#include "AudioOutput.h"
#include <QDebug>

const int OPUS_SAMPLE_RATE = 48000;
const int OPUS_CHANNELS = 1;
const int OPUS_FRAME_SIZE = 960;

AudioOutput::AudioOutput(QObject *parent) : QObject(parent) {
    QAudioFormat format;
    format.setSampleRate(OPUS_SAMPLE_RATE);
    format.setChannelCount(OPUS_CHANNELS);
    format.setSampleFormat(QAudioFormat::Int16);

    Q_EMIT debugMessage("AudioOutput initializing...");
    qDebug() << "AudioOutput initializing...";

    audioSink = new QAudioSink(format, this);
    outputDevice = audioSink->start();

    if (!outputDevice) {
        Q_EMIT debugMessage("[AudioOutput] Failed to detect audio output device.");
        qDebug() << "[AudioOutput] Failed to detect audio output device.";
    } else {
        Q_EMIT debugMessage("[AudioOutput] Audio output device detected.");
        qDebug() << "[AudioOutput] Audio output device detected.";
    }

    int error;
    opusDecoder = opus_decoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNELS, &error);
    if (error != OPUS_OK) {
        Q_EMIT debugMessage("[AudioOutput] Failed to initialize Opus decoder.");
        qDebug() << "[AudioOutput] Failed to initialize Opus decoder.";
    } else {
        Q_EMIT debugMessage("[AudioOutput] Opus decoder initialized successfully.");
        qDebug() << "[AudioOutput] Opus decoder initialized successfully.";
    }
}

AudioOutput::~AudioOutput() {
    opus_decoder_destroy(opusDecoder);
    audioSink->stop();
}

void AudioOutput::playAudio(const QByteArray &data) {
    QMutexLocker locker(&mutex);

    QByteArray decodedData = decodeAudio(data);
    if (!decodedData.isEmpty()) {
        qint64 writtenBytes = outputDevice->write(decodedData);
        Q_EMIT debugMessage("[AudioOutput] Playing audio, written bytes: " + QString::number(writtenBytes));
        qDebug() << "[AudioOutput] Playing audio, written bytes:" << writtenBytes;
    } else {
        Q_EMIT debugMessage("[AudioOutput] No data decoded to play.");
        qDebug() << "[AudioOutput] No data decoded to play.";
    }
}

QByteArray AudioOutput::decodeAudio(const QByteArray &input) {
    QByteArray output;
    int maxDecodedSamples = OPUS_FRAME_SIZE;
//    opus_int16 decodedData[maxDecodedSamples];

    std::vector<opus_int16> decodedData(maxDecodedSamples);
    int decodedSamples = opus_decode(opusDecoder, reinterpret_cast<const unsigned char*>(input.data()), input.size(), decodedData.data(), maxDecodedSamples, 0);
    if (decodedSamples > 0) {
        output.append(reinterpret_cast<const char*>(decodedData.data()), decodedSamples * sizeof(int16_t));
        Q_EMIT debugMessage("[AudioOutput] Decoded audio data size: " + QString::number(decodedSamples * sizeof(int16_t)));
        qDebug() << "[AudioOutput] Decoded audio data size:" << decodedSamples * sizeof(int16_t);
    } else {
        Q_EMIT debugMessage("[AudioOutput] Decoding failed, samples decoded: " + QString::number(decodedSamples));
        qDebug() << "[AudioOutput] Decoding failed, samples decoded:" << decodedSamples;
    }
    return output;
}
