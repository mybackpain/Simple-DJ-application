/*
  ==============================================================================

    DeckGUI.cpp
    Created: 17 Jan 2025 12:23:57pm
    Author:  JJ

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================

//pass _player variable into player variable class
DeckGUI::DeckGUI(
    DJAudioPlayer* _player,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse) :
    player(_player),
    waveformDisplay(formatManagerToUse, cacheToUse)
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    volumeSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volumeSlider.setRange(0.1, 1.0);
    speedSlider.setRange(0.1, 2.0);
    posSlider.setRange(0.1, 1.0);

    startTimer(500);

    addAndMakeVisible(waveformDisplay);
}


DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g) {
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("DeckGUI", getLocalBounds(), juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized() {
    DBG("MainComponent::resized");

    double rowH = getHeight() / 8;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    volumeSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) {
        DBG("MainComponent::buttonClicked playButton");
        player->start();
    }
    if (button == &stopButton) {
        DBG("MainComponent::buttonClicked stopButton");
        player->stop();
    }
    if (button == &loadButton) {
        DBG("MainComponent::buttonClicked loadButton");
        juce::FileChooser chooser{ "Select file" };
        if (button == &loadButton){
            auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles; //configure the dialogue
            fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser){ //launch out of the main thread using lambda function
                    juce::File chosenFile = chooser.getResult();
                    player->loadURL(juce::URL{ chosenFile });
                    waveformDisplay.loadURL(juce::URL{ chosenFile });
                });
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volumeSlider) {
        player->setGain(slider->getValue());
        std::string output = "MainComponent::sliderValueChanged volumeSlider " + std::to_string(slider->getValue());
        DBG(output);
    }
    if (slider == &speedSlider) {
        player->setSpeed(slider->getValue());
        std::string output = "MainComponent::sliderValueChanged speedSlider " + std::to_string(slider->getValue());
        DBG(output);
    }
    if (slider == &posSlider) {
        player->setPositionRelative(slider->getValue());
        std::string output = "MainComponent::sliderValueChanged posSlider " + std::to_string(slider->getValue());
        DBG(output);
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files) {
    DBG("File dragged");
    return true;
}
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y) {
    DBG("File dropped");
    if (files.size() == 1) {
        player->loadURL(juce::URL{ juce::File{files[0]} });
        DBG("DeckGUI::timerCallback, fileLoaded set to true");
    }
}

void DeckGUI::timerCallback() {
    DBG("DeckGUI::timerCallback, fileLoaded = true");
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}