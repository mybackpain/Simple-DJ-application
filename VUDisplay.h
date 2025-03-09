/*
  ==============================================================================

    VUDisplay.h
    Created: 10 Mar 2025 1:13:47am
    Author:  JJ

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class VUDisplay : public juce::Component {
public:
    VUDisplay();
    void paint(juce::Graphics& g) override;
    void resized() override;

    void setLevel(float newLevel); // Call this to update the meter

private:
    float level = 0.0f; // Stores the current VU level
};
