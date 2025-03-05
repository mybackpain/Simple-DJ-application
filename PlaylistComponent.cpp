/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 24 Jan 2025 12:44:29pm
    Author:  JJ

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(){
    
    /*trackTitles.push_back("Track 1");*/

    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    tableComponent.getHeader().addColumn("", 2, 200);
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent(){
}

void PlaylistComponent::paint (juce::Graphics& g){

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized(){
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
    trackTitleEditor.setBounds(0, 0, getWidth() - 20, 30);
}

int PlaylistComponent::getNumRows() {
    return trackTitles.size();
}
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::orange);
    }
    else {
        g.fillAll(juce::Colours::darkgrey);
    }
}
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.drawText(trackTitles[rowNumber],2,0,width-4,height-2, juce::Justification::centredLeft,true);
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) {
    if (columnId == 1) { // controls text box
        auto* textEditor = dynamic_cast<juce::TextEditor*>(existingComponentToUpdate);

        if (textEditor == nullptr) {
            textEditor = new juce::TextEditor();
            textEditor->setText(trackTitles[rowNumber]);  // create placeholding text
            textEditor->setMultiLine(false);
            textEditor->setReturnKeyStartsNewLine(false);
            textEditor->setScrollbarsShown(false);
            textEditor->setCaretVisible(true);
            textEditor->onTextChange = [this, rowNumber, textEditor]() {
                if (rowNumber >= 0 && rowNumber < static_cast<int>(trackTitles.size())) {
                    trackTitles[rowNumber] = textEditor->getText().toStdString();
                }
            };
        }
        return textEditor;
    }
    if (columnId == 2) { // controls play button
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton{ "Play" };
            juce::String id{ std::to_string(rowNumber) };
            btn->juce::Component::setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button) {
    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked" << trackTitles[id]);
}
