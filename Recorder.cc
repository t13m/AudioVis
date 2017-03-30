//
// Created by Ziye Fan on 30/3/2017.
//

#include "Recorder.hh"
#include <exception>
#include <algorithm>
Recorder * Recorder::_recorderObj = NULL;

void Recorder::initPortaudio() {
    _recorderObj = this;
    _audio = new RtAudio();
    _rtIParam.deviceId = _audio->getDefaultInputDevice();
    _rtIParam.nChannels = 2;
    _rtIParam.firstChannel = 0;

    try {
        _audio->openStream(NULL, &_rtIParam, RTAUDIO_FLOAT32, _fs, &_bufsize, &callback);
    } catch (RtAudioError &error) {
        error.printMessage();
    }
    _audio->startStream();
}

Recorder::~Recorder() {
    _audio->stopStream();
    delete _audio;
    delete [] _data;
}

bool Recorder::IsRecording() { return _recording; }

void Recorder::StartRecording() {
    _recording = true;
}

void Recorder::StopRecording() {
    _recording = false;
}
int Recorder::_callback(void *outputBuffer, void *inputBuffer, unsigned int framesPerBuffer,
                        double timeInfo, RtAudioStreamStatus statusFlags, void *userData) {
    if (_recording) {
        const float *pdata = static_cast<const float *>(inputBuffer);
        std::copy(pdata, pdata + framesPerBuffer, _data);
        _notify(_data);
    }
    return 0;
}
