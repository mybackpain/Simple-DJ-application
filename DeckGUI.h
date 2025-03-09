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
#include "VUDisplay.h"

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

    void buttonClicked(juce::Button*) override;// button listener
    void sliderValueChanged(juce::Slider*) override; // slider listener

    bool isInterestedInFileDrag(const juce::StringArray& files) override; // drag and drop
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void timerCallback() override; //updates waveform display

    void loadFile(const juce::String& filePath); // loading music file

private:

    ///////////////////////////////////////////////////////////// buttons, sliders, labels, images

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

    juce::Image recordCover1;

    ///////////////////////////////////////////////////////////// reference pointers and instances

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;

    ///////////////////////////////////////////////////////////// file import

    juce::FileChooser fChooser{ "Select a file" };
    juce::Label fileNameLabel;
    void updateFileName(const juce::String fileName);

    ///////////////////////////////////////////////////////////// VU

    VUDisplay vuDisplay;
    void updateVUMeter(float level);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};