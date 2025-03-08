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
    playButton.addListener(this);

    addAndMakeVisible(stopButton);
    stopButton.addListener(this);

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);

    addAndMakeVisible(fadeInButton);
    fadeInButton.addListener(this);

    addAndMakeVisible(fadeOutButton);
    fadeOutButton.addListener(this);

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed");
    speedLabel.setReadOnly(true);
    speedLabel.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
    speedLabel.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    speedLabel.setJustification(juce::Justification::centred);

    addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.addListener(this);
    speedSlider.setRange(0.1, 2.0);
    speedSlider.setValue(1.0);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setRange(0.1, 1.0);
    //posSlider.setValue(DJAudioPlayer::getPositionRelative());

    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Volume");
    volumeLabel.setReadOnly(true);
    volumeLabel.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
    volumeLabel.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    volumeLabel.setJustification(juce::Justification::centred);

    addAndMakeVisible(volumeSlider);
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.addListener(this);
    volumeSlider.setRange(0.1, 2.0);
    volumeSlider.setValue(1.0);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    startTimer(500);

    addAndMakeVisible(waveformDisplay);

    addAndMakeVisible(fileNameLabel);
    fileNameLabel.setJustificationType(juce::Justification::centred);
    fileNameLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    fileNameLabel.setFont(juce::Font(14.0f, juce::Font::bold));

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear background

    g.setColour(juce::Colours::grey); // draw outline around DeckGUI
    g.drawRect(getLocalBounds(), 1);

    g.setColour(juce::Colours::black); // draw black "record disk"
    if (getWidth() > getHeight()) {
        double record_radius = getHeight() / 4 - 10;
        g.fillEllipse(
            (getWidth() / 2) - record_radius,
            getHeight() / 8 * 3,
            record_radius * 2,
            record_radius * 2);
    }
    else {
        double record_radius = getWidth() / 4 - 10;
        g.fillEllipse(
            (getWidth() / 2) - record_radius,
            getHeight() / 8 * 5 - record_radius,
            record_radius * 2,
            record_radius * 2);
    }

    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f));
    g.drawText("", getLocalBounds(), juce::Justification::topLeft, true);
}

void DeckGUI::resized() {
    DBG("MainComponent::resized");
    double rowH = getHeight() / 8;
    double rowW = getWidth() / 6;

    fileNameLabel.setBounds(0 * rowW, 0 * rowH, rowW * 6, rowH * 1);
    waveformDisplay.setBounds(0 * rowW, 1 * rowH, getWidth(), rowH * 1);

    speedLabel.setBounds(0 * rowW, 2 * rowH, rowW * 1, rowH * 1);
    posSlider.setBounds(1 * rowW, 2 * rowH, rowW * 4, rowH * 1);
    volumeLabel.setBounds(5 * rowW, 2 * rowH, rowW * 1, rowH * 1);
    speedSlider.setBounds(0 * rowW, 3 * rowH, rowW * 1, rowH * 4);
    volumeSlider.setBounds(5 * rowW, 3 * rowH, rowW * 1, rowH * 4);

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
        if (button == &loadButton) {
            auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles; //configure the dialogue
            fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser) { //launch out of the main thread using lambda function
                juce::File chosenFile = chooser.getResult();
                player->loadURL(juce::URL{ chosenFile });
                waveformDisplay.loadURL(juce::URL{ chosenFile });
                updateFileName(chosenFile.getFileName());
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
        juce::File file{ files[0] };  // Convert the first string to a juce::File
        player->loadURL(juce::URL{ file });
        updateFileName(file.getFileNameWithoutExtension()); // Correct function call
        DBG("DeckGUI::filesDropped, fileLoaded set to true");
    }
}

//cleans file name of file type
void DeckGUI::updateFileName(juce::String fileName) {
    juce::String editedFileName = fileName;
    juce::StringArray replaced_word = { ".mp3", ".wav" };
    for (auto& wordToReplace : replaced_word) {
        int pos = fileName.indexOf(wordToReplace);
        if (pos != -1) { // Ensure the substring exists before replacing
            editedFileName = fileName.replaceSection(pos, wordToReplace.length(), "");
        }
    }
    fileNameLabel.setText("Playing: " + editedFileName, juce::dontSendNotification);
    fileNameLabel.setFont(juce::Font("Arial", 16.0f, juce::Font::bold));
    fileNameLabel.setColour(juce::Label::textColourId, juce::Colours::lightblue);
}

void DeckGUI::loadFile(const juce::String& filePath) {
    juce::File file{ filePath };  // Convert the path string to a juce::File object

    if (file.existsAsFile()) {
        player->loadURL(juce::URL{ file }); // Load the file into the player
        updateFileName(file.getFileNameWithoutExtension()); // Update displayed file name
        DBG("DeckGUI::loadFile - Loaded file: " + filePath);
    }
    else {
        DBG("DeckGUI::loadFile - Error: File does not exist");
    }
}

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}