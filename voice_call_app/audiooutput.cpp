#include "AudioOutput.h"

AudioOutput::AudioOutput(QObject *parent) : QObject(parent) {
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);

    audioSink = new QAudioSink(format, this);
    outputDevice = audioSink->start();
}

AudioOutput::~AudioOutput() {
    audioSink->stop();
}

void AudioOutput::playAudio(const QByteArray &data) {
    QMutexLocker locker(&mutex);
    outputDevice->write(data);
}
