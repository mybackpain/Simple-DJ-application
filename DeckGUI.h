/*
  ==============================================================================

    DeckGUI.h
    Created: 17 Jan 2025 12:23:57pm
    Author:  JJ

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    //button listener
    void buttonClicked(juce::Button*) override;
    //slider listener
    void sliderValueChanged(juce::Slider*) override;

    //drag and drop
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void timerCallback() override;

    void loadFile(const juce::String& filePath);

private:

    juce::Label fileNameLabel;
    void updateFileName(const juce::String fileName);

    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };
    juce::TextButton fadeInButton{ "FADE IN" };
    juce::TextButton fadeOutButton{ "FADE OUT" };

    juce::Slider volumeSlider{};
    juce::Slider speedSlider{};
    juce::Slider posSlider{};

    juce::TextEditor speedLabel;
    juce::TextEditor volumeLabel;

    DJAudioPlayer* player;

    WaveformDisplay waveformDisplay;

    juce::FileChooser fChooser{ "Select a file" };

    juce::Image recordCover1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};