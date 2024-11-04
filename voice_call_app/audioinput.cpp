//#include <opus.h> // Include Opus header
//#include "AudioInput.h"
//#include "webrtc.h"


#include <opus.h>
#include "AudioInput.h"
#include "audioinput.h"
#include <QDebug>
//// Constants for Opus encoding
//const int OPUS_SAMPLE_RATE = 48000;
//const int OPUS_CHANNELS = 2;
//const int OPUS_FRAME_SIZE = 960; // for 20ms frames at 48kHz

//AudioInput::AudioInput(QObject *parent) : QIODevice(parent) {
//    QAudioFormat format;
//    qDebug()<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&";
//    format.setSampleRate(OPUS_SAMPLE_RATE);
//    format.setChannelCount(OPUS_CHANNELS);
//    format.setSampleFormat(QAudioFormat::Int16);

//    audioSource = new QAudioSource(format, this);
//    audioDevice = audioSource->start();

//    int error;
//    opusEncoder = opus_encoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNELS, OPUS_APPLICATION_VOIP, &error);

//    Q_EMIT debugMessage("[AudioInput] Constructor.");
//    connect(audioDevice, &QIODevice::readyRead, this, [this]() {
//        QByteArray audioData = audioDevice->readAll();
//        QByteArray encodedData = encodeAudio(audioData);
////        Q_EMIT debugMessage("[AudioInput] Audio captured emmited in constructor.");
//        Q_EMIT audioCaptured(encodedData);
//    });
//}

AudioInput::~AudioInput() {
    opus_encoder_destroy(opusEncoder);
    audioSource->stop();
}

//QByteArray AudioInput::encodeAudio(const QByteArray &input) {
////    Q_EMIT debugMessage("[AudioInput] Encoding audio.");
//    QByteArray output;
//    int maxEncodedBytes = 4000;
//    unsigned char encodedData[maxEncodedBytes];

//    int frameSize = input.size() / (OPUS_CHANNELS * sizeof(int16_t));
//    int encodedBytes = opus_encode(opusEncoder, reinterpret_cast<const opus_int16*>(input.constData()), frameSize, encodedData, maxEncodedBytes);

//    if (encodedBytes > 0) {
//        output.append(reinterpret_cast<const char*>(encodedData), encodedBytes);
//    }

//    return output;
//}

qint64 AudioInput::readData(char *data, qint64 maxlen) {
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return 0;
}

qint64 AudioInput::writeData(const char *data, qint64 len) {
//    Q_EMIT debugMessage("[AudioInput] Audio captured emmited in writeData.");

    Q_EMIT audioCaptured(QByteArray(data, len));
    return len;
}

void AudioInput::startCapture() {
    Q_EMIT debugMessage("[AudioInput] Start capture.");
    qDebug()<<"[AudioInput] Start capture.";
    audioSource->start();
}


const int OPUS_SAMPLE_RATE = 48000;
const int OPUS_CHANNELS = 2;
const int OPUS_FRAME_SIZE = 960; // 20ms frames at 48kHz, balanced for quality

AudioInput::AudioInput(QObject *parent) : QIODevice(parent) {
    qDebug() << "[AudioInput]";
    QAudioFormat format;
    format.setSampleRate(OPUS_SAMPLE_RATE);
    format.setChannelCount(OPUS_CHANNELS);
    format.setSampleFormat(QAudioFormat::Int16);

    audioSource = new QAudioSource(format, this);
    audioDevice = audioSource->start();

    int error;
    opusEncoder = opus_encoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNELS, OPUS_APPLICATION_AUDIO, &error);
    if (error != OPUS_OK) {
        qDebug() << "[AudioInput] Failed to initialize Opus encoder";
    } else {
        opus_encoder_ctl(opusEncoder, OPUS_SET_BITRATE(64000)); // 64kbps for improved quality
        opus_encoder_ctl(opusEncoder, OPUS_SET_COMPLEXITY(10)); // Max complexity
    }

    connect(audioDevice, &QIODevice::readyRead, this, [this]() {
        QByteArray audioData = audioDevice->readAll();
        qDebug() << "[AudioInput] Captured raw audio data size:" << audioData.size();
        QByteArray encodedData = encodeAudio(audioData);
        qDebug() <<"[AudioInput] data is captured.";
//        Q_EMIT audioCaptured(encodedData);
        qDebug() << "[AudioInput] Captured processed audio data size:" << encodedData.size();

        Q_EMIT audioCaptured(encodedData);

    });
}

// Encoder function
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

