/*
  ==============================================================================

    DJAudioFolder.h
    Created: 16 Jan 2025 2:00:56pm
    Author:  JJ

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource, public juce::Timer {
public:

    //constructor and destructor
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();

    void fadeIn();
    void fadeOut();

    double getPositionRelative();

private:

    juce::AudioFormatManager& formatManager; //first layer, overall audio manager, registers audio formats, creates parsers for known audio formats
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource; //second layer, wrap reader into audio source, reads audio file, downloads data into numerical array
    juce::AudioTransportSource transportSource; //third layer, wraps AudioFormatReaderSource, allows control of audio playback (backwards forwards etc)

    juce::ResamplingAudioSource resampleSource{&transportSource,false,2 };

    enum FadeState { NONE, FADE_IN, FADE_OUT };
    FadeState fadeState = NONE;


    double currentGain = 0.0;
    void timerCallback();
};