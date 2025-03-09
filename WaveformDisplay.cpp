/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 22 Jan 2025 3:41:05pm
    Author:  JJ

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse) :
    audioThumb(1000, formatManagerToUse, cacheToUse){
    audioThumb.addChangeListener(this);
    fileLoaded = false;
    position = 0.0;
}

WaveformDisplay::~WaveformDisplay(){
}

void WaveformDisplay::paint (juce::Graphics& g){
    juce::Colour pennRed(149, 25, 12), berkeleyBlue(18, 53, 91), darkPurple(66, 0, 57);

    g.fillAll (berkeleyBlue);

    g.setColour(juce::Colours::grey); // outline around WaveformDisplay instance
    g.drawRect(getLocalBounds(), 4);

    if (fileLoaded) {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1);
    }
    else {
        g.setColour(juce::Colours::white);
        g.setFont(juce::FontOptions(16.0f));
        g.drawText("File not loaded", getLocalBounds(), juce::Justification::centred, true);
    }
    g.setColour(juce::Colours::lightgreen);
    if (position != 0) {
        if (position >= 0 && position <= getWidth()) {
            g.drawRect(position * getWidth(), 0, getWidth() / 40, getHeight());
        }
    }
}

void WaveformDisplay::resized(){

}

void WaveformDisplay::loadURL(juce::URL audioURL) {
    DBG("WaveformDisplay::loadURL");
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded) {
        DBG("WaveformDisplay::loadURL loaded");
    }
    else {
        DBG("WaveformDisplay::loadURL load fail");
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
    DBG("WaveformDisplay::changeListenerCallback");
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos) {
    if (fileLoaded == true) {
        if (position != pos) {
            position = pos;
            repaint();
        };
    }
}