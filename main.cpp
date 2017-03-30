
#include "config.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Recorder.hh"
#include "FFT.hh"

using namespace std;

const int frameLength = 1024;
float audioFrame[1024];
float audioFrameFFT[2048];
float audioFramePower[1024];
GFFT<10, float> FFT;
static void getAudioFrame(void* data) {
    copy(static_cast<float*>(data), static_cast<float*>(data) + frameLength, audioFrame);

    for (int i = 0; i < frameLength; i ++) {
        audioFrameFFT[i * 2] = audioFrame[i];
        audioFrameFFT[i * 2 + 1] = 0.0f;
    }
    FFT.fft(audioFrameFFT);
    for (int i = 0; i < frameLength; i ++)
        audioFramePower[i] = audioFrameFFT[2 * i] * audioFrameFFT[2 * i]
                             + audioFrameFFT[2 * i + 1] * audioFrameFFT[2 * i + 1];

}

int main(int argc, char* argv[]) {

    /* Code adapted from the SFML 2 "Window" example */

    cout << "Version " << AudioVis_VERSION_MAJOR << "." << AudioVis_VERSION_MINOR << endl;

    sf::RenderWindow window(sf::VideoMode(1024, 600), "myproject");
    window.setFramerateLimit(20);

    Recorder recorder(44100, 1024, getAudioFrame);
    recorder.StartRecording();


    sf::VertexArray waveform(sf::LinesStrip, 1024);
    sf::VertexArray spectrum(sf::Lines, 1024);
    while (window.isOpen()) {
        sf::Event Event;
        while (window.pollEvent(Event)) {
            if (Event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        for (int i = 0; i < 1024; i ++) {
            waveform[i].position = sf::Vector2f(i, 100 + 300 * audioFrame[i]);
            waveform[i].color = sf::Color::Green;

        }
        for (int i = 0; i < 512; i ++) {
            spectrum[i*2].position = sf::Vector2f(i*2, 500 + 1);
            spectrum[i*2+1].position = sf::Vector2f(i*2, 500 - audioFramePower[i]);

            spectrum[i*2].color = sf::Color::White;
            spectrum[i*2+1].color = sf::Color::White;

        }

        window.draw(waveform);
        window.draw(spectrum);
        window.display();
    }

    recorder.StopRecording();
}















/*

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
            double streamTime, RtAudioStreamStatus status, void *userData )
{
    if ( status )
        std::cout << "Stream overflow detected!" << std::endl;
    // Do something with the data in the "inputBuffer" buffer.
    return 0;
}
int main()
{
    RtAudio adc;
    if ( adc.getDeviceCount() < 1 ) {
        std::cout << "\nNo audio devices found!\n";
        exit( 0 );
    }
    RtAudio::StreamParameters parameters;
    parameters.deviceId = adc.getDefaultInputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 256; // 256 sample frames
    try {
        adc.openStream( NULL, &parameters, RTAUDIO_SINT16,
                        sampleRate, &bufferFrames, &record );
        adc.startStream();
    }
    catch ( RtAudioError& e ) {
        e.printMessage();
        exit( 0 );
    }

    char input;
    std::cout << "\nRecording ... press <enter> to quit.\n";
    std::cin.get( input );
    try {
        // Stop the stream
        adc.stopStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
    }
    if ( adc.isStreamOpen() ) adc.closeStream();
    return 0;
}

*/