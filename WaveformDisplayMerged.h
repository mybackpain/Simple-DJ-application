/*
  ==============================================================================

    WaveformDisplayMerged.h
    Created: 5 Mar 2025 9:33:55pm
    Author:  JJ

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MergedWaveformDisplay : public juce::Component, public juce::ChangeListener {
public:
    MergedWaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    ~MergedWaveformDisplay() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void loadURL1(juce::URL audioURL);
    void loadURL2(juce::URL audioURL);
    void setPositionRelative(double pos1, double pos2);

    void updatePosition(double pos);

private:
    juce::AudioThumbnail audioThumb1;
    juce::AudioThumbnail audioThumb2;

    bool fileLoaded1;
    bool fileLoaded2;

    double position1 = 0.0;
    double position2 = 0.0;
    bool firstUpdate = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MergedWaveformDisplay)
};