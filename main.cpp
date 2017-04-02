#include "config.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Recorder.hh"
#include "FFT.hh"
#include "F0Estimator.hh"

using namespace std;

const int frameLength = 1024;
float audioFrame[1024];
float audioFrameFFT[2048];
float audioFramePower[1024];
GFFT<10, float> FFT;
float freq = 0, qfreq = 0;
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
    float p = F0Estimator::EstimatePeriod(audioFrame, frameLength, 32, 320, qfreq);
    if (qfreq > 0.8)
        freq = 16000.0 / p;
}

int main(int argc, char* argv[]) {

    /* Code adapted from the SFML 2 "Window" example */

    cout << "Version " << AudioVis_VERSION_MAJOR << "." << AudioVis_VERSION_MINOR << endl;

    sf::RenderWindow window(sf::VideoMode(1024, 600), "myproject");
    window.setFramerateLimit(20);

    Recorder recorder(16000, 1024, getAudioFrame);
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
        std::cout << "Current Frequency: " << freq << " || Q: " << qfreq << std::endl;
        window.draw(waveform);
        window.draw(spectrum);
        window.display();
    }

    recorder.StopRecording();
}