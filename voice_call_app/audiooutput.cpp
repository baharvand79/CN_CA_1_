#include "audiooutput.h"
#include "opus/include/opus.h"

AudioOutput::AudioOutput(QObject *parent) : QObject(parent) {

    audioFormat.setSampleRate(48000);
    audioFormat.setChannelCount(2);
    audioFormat.setSampleFormat(QAudioFormat::Int16);

    audioSink = new QAudioSink(audioFormat, this);

    connect(this, &AudioOutput::newPacket, this, &AudioOutput::play);

    int error;
    opusDecoder = opus_decoder_create(48000, 2, &error);
    if (error != OPUS_OK) {
        qWarning() << "Failed to create Opus decoder:" << opus_strerror(error);
    }

    audioSink->start();
}

void AudioOutput::addData(const QByteArray &data) {
    QMutexLocker locker(&mutex);
    audioQueue.enqueue(data);
    emit newPacket();
}

void AudioOutput::play() {
    QMutexLocker locker(&mutex);

    if (!audioQueue.isEmpty()) {
        QByteArray data = audioQueue.dequeue();
        processAudio(data);
    }
}

void AudioOutput::processAudio(const QByteArray &data) {
    std::vector<int16_t> decodedAudio(data.size());
    int frameSize = opus_decode(opusDecoder, reinterpret_cast<const unsigned char*>(data.constData()),
                                 data.size(), decodedAudio.data(), decodedAudio.size(), 0);

    if (frameSize < 0) {
        qWarning() << "Opus decoding error:" << opus_strerror(frameSize);
        return;
    }

    QIODevice *device = audioSink->start();
    device->write(reinterpret_cast<const char*>(decodedAudio.data()), frameSize * sizeof(int16_t));

}
