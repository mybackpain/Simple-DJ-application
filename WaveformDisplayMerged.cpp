/*
  ==============================================================================

    WaveformDisplayMerged.cpp
    Created: 5 Mar 2025 9:33:55pm
    Author:  JJ

  ==============================================================================
*/

#include "WaveformDisplayMerged.h"

// Constructor
MergedWaveformDisplay::MergedWaveformDisplay(
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse)
    : audioThumb1(1000, formatManagerToUse, cacheToUse),
    audioThumb2(1000, formatManagerToUse, cacheToUse) {

    audioThumb1.addChangeListener(this);
    audioThumb2.addChangeListener(this);

    fileLoaded1 = false;
    fileLoaded2 = false;

    position1 = 0.0;
    position2 = 0.0;
}

// Destructor
MergedWaveformDisplay::~MergedWaveformDisplay() {}

// Paint function
void MergedWaveformDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    if (fileLoaded1) {
        g.setColour(juce::Colours::blue);
        audioThumb1.drawChannel(g, getLocalBounds(), 0, audioThumb1.getTotalLength(), 0, 1.0f);
    }

    if (fileLoaded2) {
        g.setColour(juce::Colours::red);
        audioThumb2.drawChannel(g, getLocalBounds(), 0, audioThumb2.getTotalLength(), 0, 1.0f);
    }

    // Draw position markers
    g.setColour(juce::Colours::green);
    if (fileLoaded1 && position1 >= 0 && position1 <= getWidth()) {
        g.drawRect(position1 * getWidth(), 0, 2, getHeight());
    }

    g.setColour(juce::Colours::yellow);
    if (fileLoaded2 && position2 >= 0 && position2 <= getWidth()) {
        g.drawRect(position2 * getWidth(), 0, 2, getHeight());
    }
}

void MergedWaveformDisplay::resized() {}

void MergedWaveformDisplay::loadURL1(juce::URL audioURL) {
    DBG("MergedWaveformDisplay::loadURL1");
    audioThumb1.clear();
    fileLoaded1 = audioThumb1.setSource(new juce::URLInputSource(audioURL));
    repaint();
    if (fileLoaded1) {
        DBG("MergedWaveformDisplay::loadURL1 loaded");
    }
    else {
        DBG("MergedWaveformDisplay::loadURL1 load fail");
    }
}

// Load second track
void MergedWaveformDisplay::loadURL2(juce::URL audioURL) {
    DBG("MergedWaveformDisplay::loadURL2");
    audioThumb2.clear();
    fileLoaded2 = audioThumb2.setSource(new juce::URLInputSource(audioURL));
    repaint();
    if (fileLoaded2) {
        DBG("MergedWaveformDisplay::loadURL2 loaded");
    }
    else {
        DBG("MergedWaveformDisplay::loadURL2 load fail");
    }
}

void MergedWaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
    DBG("WaveformDisplay::changeListenerCallback");
    repaint();
}

void MergedWaveformDisplay::setPositionRelative(double pos1, double pos2) {
    if (fileLoaded1 == true) {
        if (position1 != pos1) {
            position1 = pos1;
            repaint();
        };
    }
    if (fileLoaded2 == true) {
        if (position2 != pos1) {
            position2 = pos2;
            repaint();
        };
    }
    repaint();
}

void MergedWaveformDisplay::updatePosition(double pos) {
    if (firstUpdate) {
        position1 = pos;
    }
    else {
        position2 = pos;
    }
    firstUpdate = !firstUpdate;
    if (position1 != position2) {
        repaint();
    }
}
