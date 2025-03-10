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
    juce::Colour pennRed(149, 25, 12), berkeleyBlue(18, 53, 91), darkPurple(66, 0, 57);

    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setRange(0.000001, 1.0);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    posSlider.setColour(juce::Slider::backgroundColourId, berkeleyBlue.brighter(0.2f));
    posSlider.setColour(juce::Slider::trackColourId, berkeleyBlue.brighter(0.2f));
    posSlider.setColour(juce::Slider::thumbColourId, darkPurple.brighter(0.4f));

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed");
    speedLabel.setReadOnly(true);
    speedLabel.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
    speedLabel.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    speedLabel.setJustification(juce::Justification::centred);

    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Volume");
    volumeLabel.setReadOnly(true);
    volumeLabel.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
    volumeLabel.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    volumeLabel.setJustification(juce::Justification::centred);

    addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.addListener(this);
    speedSlider.setRange(0.0001, 2.0);
    speedSlider.setValue(1.0);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 35, 20);
    speedSlider.setColour(juce::Slider::backgroundColourId, berkeleyBlue.brighter(0.2f));
    speedSlider.setColour(juce::Slider::trackColourId, berkeleyBlue.brighter(0.2f));
    speedSlider.setColour(juce::Slider::thumbColourId, darkPurple.brighter(0.4f));

    addAndMakeVisible(volumeSlider);
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.addListener(this);
    volumeSlider.setRange(0.0001, 2.0);
    volumeSlider.setValue(1.0);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 35, 20);
    volumeSlider.setColour(juce::Slider::backgroundColourId, berkeleyBlue.brighter(0.2f));
    volumeSlider.setColour(juce::Slider::trackColourId, berkeleyBlue.brighter(0.2f));
    volumeSlider.setColour(juce::Slider::thumbColourId, darkPurple.brighter(0.4f));

    addAndMakeVisible(playButton);
    playButton.setColour(juce::TextButton::buttonColourId, darkPurple);
    playButton.addListener(this);

    addAndMakeVisible(stopButton);
    stopButton.setColour(juce::TextButton::buttonColourId, darkPurple);
    stopButton.addListener(this);

    addAndMakeVisible(loadButton);
    loadButton.setColour(juce::TextButton::buttonColourId, darkPurple);
    loadButton.addListener(this);

    addAndMakeVisible(fadeInButton);
    fadeInButton.setColour(juce::TextButton::buttonColourId, darkPurple);
    fadeInButton.addListener(this);

    addAndMakeVisible(fadeOutButton);
    fadeOutButton.setColour(juce::TextButton::buttonColourId, darkPurple);
    fadeOutButton.addListener(this);

    addAndMakeVisible(fileNameLabel);
    fileNameLabel.setJustificationType(juce::Justification::centred);
    fileNameLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    fileNameLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    
    juce::File recordImageFile1 = juce::File::getCurrentWorkingDirectory().getChildFile("recordCover1.jpg");
    if (recordImageFile1.existsAsFile()) {
        recordCover1 = juce::ImageFileFormat::loadFrom(recordImageFile1);
        DBG("Record image 1 loaded");
    }

    startTimer(500); //update interval for waveformDisplay
    addAndMakeVisible(waveformDisplay);

    addAndMakeVisible(vuDisplay);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(juce::Graphics& g) {
    juce::Colour pennRed(149, 25, 12), berkeleyBlue(18, 53, 91), darkPurple(66, 0, 57);

    g.fillAll(berkeleyBlue);

    g.setColour(juce::Colours::grey); // create outline around DeckGUI instance
    g.drawRect(getLocalBounds(), 4);

    g.setColour(juce::Colours::black); // draw black "record disk"
    double recordRadius = getHeight() / 4 - 10;
    double recordCenterX = getWidth() / 2.0;
    double recordCenterY;
    if (getWidth() > getHeight()) {
        recordCenterY = getHeight() / 8 * 3 + recordRadius + 10;
    }
    else {
        recordCenterY = getHeight() / 8 * 5 + 2;
    }
    g.fillEllipse(recordCenterX - recordRadius, recordCenterY - recordRadius, recordRadius * 2, recordRadius * 2);

    if (recordCover1.isValid()) { // draw record label image
        double labelRadius = recordRadius * 0.5; 

        juce::Path circularMask;
        circularMask.addEllipse(recordCenterX - labelRadius, recordCenterY - labelRadius, labelRadius * 2, labelRadius * 2);
        g.reduceClipRegion(circularMask); 

        g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
        juce::Rectangle<float> labelArea(recordCenterX - labelRadius, recordCenterY - labelRadius, labelRadius * 2, labelRadius * 2);
        g.drawImage(recordCover1, labelArea, juce::RectanglePlacement::centred);
    }
    else {
        g.setColour(juce::Colours::red);
        g.drawText("No Image Loaded", getLocalBounds(), juce::Justification::centred);
    }
}

void DeckGUI::resized() {
    DBG("MainComponent::resized");
    double rowH = getHeight() / 8;
    double rowW = getWidth() / 6;

    fileNameLabel.setBounds(0 * rowW, 0 * rowH, getWidth(), rowH * 1);
    waveformDisplay.setBounds(0 * rowW, 1 * rowH, rowW * 4, rowH * 1);
    vuDisplay.setBounds(4 * rowW, 1 * rowH, rowW * 2, rowH * 1);

    posSlider.setBounds(1 * rowW, 2 * rowH, rowW * 4, rowH * 1);
    speedLabel.setBounds(0 * rowW, 3 * rowH, rowW * 1, rowH * 1);
    volumeLabel.setBounds(5 * rowW, 3 * rowH, rowW * 1, rowH * 1);
    speedSlider.setBounds(0 * rowW, 4 * rowH, rowW * 1, rowH * 3);
    volumeSlider.setBounds(5 * rowW, 4 * rowH, rowW * 1, rowH * 3);

    fadeInButton.setBounds(0 * rowW + rowW / 6, 7 * rowH, rowW * 1, rowH * 1 - 10);
    playButton.setBounds(1 * rowW + rowW / 6 * 2, 7 * rowH, rowW * 1, rowH * 1 - 10);
    stopButton.setBounds(2 * rowW + rowW / 6 * 3, 7 * rowH, rowW * 1, rowH * 1 - 10);
    fadeOutButton.setBounds(3 * rowW + rowW / 6 * 4, 7 * rowH, rowW * 1, rowH * 1 - 10);
    loadButton.setBounds(4 * rowW + rowW / 6 * 5, 7 * rowH, rowW * 1, rowH * 1 - 10);
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
        juce::File file{ files[0] };  //convert string juce::File
        player->loadURL(juce::URL{ file });
        updateFileName(file.getFileNameWithoutExtension());
        DBG("DeckGUI::filesDropped, fileLoaded set to true");
    }
}

//cleans file name for display at top of DeckGUI
void DeckGUI::updateFileName(juce::String fileName) {
    juce::String editedFileName = fileName;
    juce::StringArray replacedWord = { ".mp3", ".wav" };
    for (auto& wordToReplace : replacedWord) {
        int pos = fileName.indexOf(wordToReplace);
        if (pos != -1) { // Ensure the substring exists before replacing
            editedFileName = fileName.replaceSection(pos, wordToReplace.length(), "");
        }
    }
    fileNameLabel.setText("Playing: " + editedFileName, juce::dontSendNotification);
    fileNameLabel.setFont(juce::Font("Arial", 16.0f, juce::Font::bold));
    fileNameLabel.setColour(juce::Label::textColourId, juce::Colours::white);
}

void DeckGUI::loadFile(const juce::String& filePath) {
    juce::File file{ filePath };  // convert string to file object

    if (file.existsAsFile()) {
        player->loadURL(juce::URL{ file });
        updateFileName(file.getFileNameWithoutExtension());
        waveformDisplay.loadURL(juce::URL{ file });
        DBG("DeckGUI::loadFile - Loaded file: " + filePath);
    }
    else {
        DBG("DeckGUI::loadFile - Error: File does not exist");
    }
}

void DeckGUI::timerCallback() { //update waveformDisplay
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGUI::updateVUMeter(float level) {
    vuDisplay.setLevel(level);  // update VUDisplay 
}