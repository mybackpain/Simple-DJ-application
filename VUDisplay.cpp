/*
  ==============================================================================

    VUDisplay.cpp
    Created: 10 Mar 2025 1:13:47am
    Author:  JJ

  ==============================================================================
*/

#include "VUDisplay.h"

VUDisplay::VUDisplay() {
}

void VUDisplay::paint(juce::Graphics& g) {
    juce::Colour pennRed(149, 25, 12), berkeleyBlue(18, 53, 91), darkPurple(66, 0, 57);
    g.fillAll(berkeleyBlue);

    g.setColour(juce::Colours::grey); // outline around VUDisplay instance
    g.drawRect(getLocalBounds(), 4);

    float amplifiedLevel = level * 5.0f;
    amplifiedLevel = juce::jlimit(0.0f, 1.0f, amplifiedLevel); // map to limit
    float barHeight = getHeight() * amplifiedLevel;
    g.setColour(darkPurple);
    float rectHeight = juce::jmax(0.0f, barHeight - 5);
    g.fillRect(5.0f, getHeight() - rectHeight - 5.0f, static_cast<float>(getWidth()) - 10.0f, rectHeight);
}

void VUDisplay::resized() {
}

void VUDisplay::setLevel(float newLevel) { // "map" values between 0 and 1
    level = juce::jlimit(0.0f, 1.0f, newLevel);
    repaint();
}