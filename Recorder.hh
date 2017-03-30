//
// Created by Ziye Fan on 30/3/2017.
//

#ifndef AUDIOVIS_RECORDER_HH
#define AUDIOVIS_RECORDER_HH

#include <functional>
#include <thread>
#include "RtAudio.h"

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
    unsigned int _fs, _bufsize;
    float *_data;

    RtAudio *_audio;
    RtAudio::StreamParameters _rtIParam, _rtOParam;
    RtAudio::StreamOptions _rtOptions;

    bool _recording;
    std::function<void(void*)> _notify;

    void initPortaudio();


    static int callback(void *outputBuffer, void *inputBuffer, unsigned int framesPerBuffer,
                        double timeInfo, RtAudioStreamStatus statusFlags, void *userData) {
        return _recorderObj->_callback(outputBuffer, inputBuffer, framesPerBuffer,
                                       timeInfo, statusFlags, userData);
    }
    int _callback(void *outputBuffer, void *inputBuffer, unsigned int framesPerBuffer,
                  double timeInfo, RtAudioStreamStatus statusFlags, void *userData);

};


#endif //AUDIOVIS_RECORDER_HH
