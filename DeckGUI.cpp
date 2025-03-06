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
    addAndMakeVisible(fadeInButton);
    addAndMakeVisible(fadeOutButton);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    fadeInButton.addListener(this);
    fadeOutButton.addListener(this);

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
    double rowW = getWidth() / 6;

    waveformDisplay.setBounds(0 * rowW, 1 * rowH, rowW * 6, rowH * 1);
    speedSlider.setBounds(0 * rowW, 2 * rowH, rowW * 3, rowH * 1);
    posSlider.setBounds(3 * rowW, 2 * rowH, rowW * 3, rowH * 1);
    volumeSlider.setBounds(0 * rowW, 4 * rowH, rowW * 6, rowH * 1);
    fadeInButton.setBounds(0 * rowW + rowW / 6, 7 * rowH, rowW * 1, rowH * 1);
    playButton.setBounds(1 * rowW + rowW / 6 * 2, 7 * rowH, rowW * 1, rowH * 1);
    stopButton.setBounds(2 * rowW + rowW / 6 * 3, 7 * rowH, rowW * 1, rowH * 1);
    fadeOutButton.setBounds(3 * rowW + rowW / 6 * 4, 7 * rowH, rowW * 1, rowH * 1);
    loadButton.setBounds(4 * rowW + rowW / 6 * 5, 7 * rowH, rowW * 1, rowH * 1);
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
    if (button == &fadeInButton) {
        DBG("MainComponent::buttonClicked fadeInButton");
        player->fadeIn();
    }
    if (button == &fadeOutButton) {
        DBG("MainComponent::buttonClicked fadeOutButton");
        player->fadeOut();
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
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}