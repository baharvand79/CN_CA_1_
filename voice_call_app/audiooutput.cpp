#include <opus.h> // Include Opus header
#include "AudioOutput.h"
#include "qdebug.h"

const int OPUS_SAMPLE_RATE = 48000;
const int OPUS_CHANNELS = 2;
const int OPUS_FRAME_SIZE = 960; // for 20ms frames at 48kHz
AudioOutput::AudioOutput(QObject *parent) : QObject(parent) {
    QAudioFormat format;
    format.setSampleRate(OPUS_SAMPLE_RATE);
    format.setChannelCount(OPUS_CHANNELS);
    format.setSampleFormat(QAudioFormat::Int16);

    qDebug() << "AudioOutput";

    audioSink = new QAudioSink(format, this);
    outputDevice = audioSink->start();

    if (!outputDevice) {
        Q_EMIT debugMessage("[AudioOutput] Failed to detect audio output device.");
    } else {
        Q_EMIT debugMessage("[AudioOutput] Audio output device detected.");
    }

    int error;
    opusDecoder = opus_decoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNELS, &error);
    if (error != OPUS_OK) {
        Q_EMIT debugMessage("[AudioOutput] Failed to initialize Opus decoder.");
    } else {
        Q_EMIT debugMessage("[AudioOutput] Opus decoder initialized successfully.");
    }
}


AudioOutput::~AudioOutput() {
    opus_decoder_destroy(opusDecoder);
    audioSink->stop();
}

void AudioOutput::playAudio(const QByteArray &data) {
    QMutexLocker locker(&mutex);

    QByteArray decodedData = decodeAudio(data);
    outputDevice->write(decodedData);
//    Q_EMIT debugMessage("[AudioOutput] Play audio.");
}

QByteArray AudioOutput::decodeAudio(const QByteArray &input) {
    QByteArray output;
    int maxDecodedSamples = OPUS_FRAME_SIZE * OPUS_CHANNELS;
    opus_int16 decodedData[maxDecodedSamples];

    int decodedSamples = opus_decode(opusDecoder, reinterpret_cast<const unsigned char*>(input.constData()), input.size(), decodedData, maxDecodedSamples, 0);
//    Q_EMIT debugMessage("[AudioOutput] Decode audio.");

    if (decodedSamples > 0) {
        output.append(reinterpret_cast<const char*>(decodedData), decodedSamples * sizeof(int16_t));
    }

    return output;
}
