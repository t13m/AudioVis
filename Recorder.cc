//
// Created by Ziye Fan on 30/3/2017.
//

#include "Recorder.hh"
#include <exception>
#include <algorithm>
Recorder * Recorder::_recorderObj = NULL;

void Recorder::initPortaudio() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        throw std::runtime_error("Portaudio not initialized");
    }

    _paParam.device = Pa_GetDefaultInputDevice();
    printf( "Input device # %d.\n", _paParam.device ); /* print input device number */
    printf( "Input LL: %g s\n", Pa_GetDeviceInfo( _paParam.device )->defaultLowInputLatency );
    printf( "Input HL: %g s\n", Pa_GetDeviceInfo( _paParam.device )->defaultHighInputLatency );
    _paParam.channelCount = 1;
    _paParam.sampleFormat = paFloat32;
    _paParam.suggestedLatency = Pa_GetDeviceInfo( _paParam.device )->defaultHighInputLatency;
    _paParam.hostApiSpecificStreamInfo = NULL;
    _recorderObj = this;

    err = Pa_OpenStream(
            &_paStream,
            &_paParam,
            NULL,
            _fs,
            _bufsize,
            paNoFlag,
            callback,
            NULL
    );
    if (err != paNoError) {
        throw std::runtime_error("Portaudio stream is not open");
    }
    err = Pa_StartStream( _paStream );
    if (err != paNoError) {
        throw std::runtime_error("Portaudio stream is not started");
    }
}

Recorder::~Recorder() {
    auto err = Pa_StopStream( _paStream );
    Pa_Terminate();
    delete [] _data;
}

bool Recorder::IsRecording() { return _recording; }

void Recorder::StartRecording() {
    _recording = true;
}

void Recorder::StopRecording() {
    _recording = false;
}
int Recorder::_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    if (_recording) {
        const float *pdata = static_cast<const float *>(inputBuffer);
        std::copy(pdata, pdata + framesPerBuffer, _data);
        _notify(_data);
    }
    return 0;
}
