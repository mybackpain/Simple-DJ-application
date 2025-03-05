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

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    if (fileLoaded) {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1);
    }
    else {
        g.setColour(juce::Colours::orange);
        g.setFont(juce::FontOptions(20.0f));
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