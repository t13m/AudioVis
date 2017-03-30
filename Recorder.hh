//
// Created by Ziye Fan on 30/3/2017.
//

#ifndef AUDIOVIS_RECORDER_HH
#define AUDIOVIS_RECORDER_HH

#include <functional>
#include <thread>
#include "portaudio.h"

class Recorder {
public:
    Recorder(int fs, int bufsize, std::function<void(void*)> notify):
        _fs(fs), _bufsize(bufsize), _notify(notify){
        _data = new float[bufsize];

        initPortaudio();

    }
    Recorder(const Recorder&) = delete;

    ~Recorder();

    void StartRecording();
    void StopRecording();
    bool IsRecording();

    static Recorder* _recorderObj;

private:
    int _fs, _bufsize;
    float *_data;

    PaStream *_paStream;
    PaStreamParameters _paParam;

    bool _recording;
    std::function<void(void*)> _notify;

    void initPortaudio();


    static int callback(const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlags,
                    void *userData ) {
        return _recorderObj->_callback(inputBuffer, outputBuffer, framesPerBuffer,
                                       timeInfo, statusFlags, userData);
    }
    int _callback(const void *inputBuffer, void *outputBuffer,
                 unsigned long framesPerBuffer,
                 const PaStreamCallbackTimeInfo* timeInfo,
                 PaStreamCallbackFlags statusFlags,
                 void *userData );

};


#endif //AUDIOVIS_RECORDER_HH
