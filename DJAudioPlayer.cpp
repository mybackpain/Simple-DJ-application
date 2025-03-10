/*
  ==============================================================================

    DJAudioFolder.cpp
    Created: 16 Jan 2025 2:00:56pm
    Author:  JJ

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager, DeckGUI& _deckGUI) : formatManager(_formatManager), deckGUI(_deckGUI) {
    DBG("DJAudioPlayer::DJAudioPlayer initialized.");
}

DJAudioPlayer::~DJAudioPlayer() {
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    resampleSource.getNextAudioBlock(bufferToFill);

    if (bufferToFill.buffer->getNumChannels() > 0) {
        float rms = bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.numSamples);

        juce::MessageManager::callAsync([this, rms]() {
            deckGUI.updateVUMeter(rms);
            });
    }
}

void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL) {
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}
void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain > 2) {
        DBG("DJAudioPlayer::setGain set between 0 and 1");
    }
    else {
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio > 2) {
        DBG("DJAudioPlayer::setSpeed set between 0 and 10");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0 || pos > 1) {
        DBG("DJAudioPlayer::setPositionRelative set pos between 0 and 1");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start() {
    transportSource.start();
    DBG("DJAudioPlayer::start");
}
void DJAudioPlayer::stop() {
    transportSource.stop();
    DBG("DJAudioPlayer::stop");
}

double DJAudioPlayer::getPositionRelative() {
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

//start playing music, increase gain from 0 to 1
void DJAudioPlayer::fadeIn() {
    currentGain = 0.0;
    fadeState = FADE_IN;
    transportSource.start();
    startTimer(100);
}

//stop playing music, increase gain from 0 to 1
void DJAudioPlayer::fadeOut() {
    fadeState = FADE_OUT;
    startTimer(100);
}

void DJAudioPlayer::timerCallback() {
    double gain_change = 0.05;
    double play_volume = 1.0;

    //fade in or stop
    if (fadeState == FADE_IN) {
        if (currentGain < play_volume) {
            currentGain += gain_change;
            if (currentGain > play_volume) currentGain = play_volume;
            setGain(currentGain);
        }
        else {
            fadeState = NONE;
            stopTimer();
        }
    }
    //fade out or stop
    else if (fadeState == FADE_OUT) {
        if (currentGain > 0.0) {
            currentGain -= gain_change;
            if (currentGain < 0.0) currentGain = 0.0;
            setGain(currentGain);
        }
        else {
            fadeState = NONE;
            stopTimer();
            transportSource.stop();
        }
    }
}

void DJAudioPlayer::processBlock(juce::AudioBuffer<float>& buffer) {
    float rms = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    DBG("Processing audio. RMS Level: " << rms); 
    deckGUI.updateVUMeter(rms);
}