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
PlaylistComponent::PlaylistComponent(DeckGUI* deck1, DeckGUI* deck2) : deckGUI1(deck1), deckGUI2(deck2) {

    tableComponent.getHeader().addColumn("Track Title / Description", 1, 400);
    tableComponent.getHeader().addColumn("", 2, 200);
    tableComponent.getHeader().addColumn("", 3, 200);
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent() {
}

void PlaylistComponent::paint(juce::Graphics& g) {

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f));
    g.drawText("Drag files here to add them to your playlist", getLocalBounds(), juce::Justification::centredTop, true);   // draw some placeholder text
}

void PlaylistComponent::resized() {
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
    g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height - 2, juce::Justification::centredLeft, true);
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) {
    if (columnId == 1) { // controls text box
        auto* textEditor = dynamic_cast<juce::TextEditor*>(existingComponentToUpdate);

        if (textEditor == nullptr) {
            textEditor = new juce::TextEditor();
            textEditor->setText(trackTitles[rowNumber]);  // create placeholding text
            textEditor->setJustification(juce::Justification::left);
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
    if (columnId == 2) { // buttons to queue tracks
        auto* btn = dynamic_cast<juce::TextButton*>(existingComponentToUpdate);
        if (btn == nullptr) {
            btn = new juce::TextButton("Push to Deck 1");
            juce::String id = juce::String(rowNumber) + "-" + juce::String(columnId);
            btn->setComponentID(id);
            btn->addListener(this);
        }
        return btn;
    }
    if (columnId == 3) { // buttons to queue tracks
        auto* btn = dynamic_cast<juce::TextButton*>(existingComponentToUpdate);
        if (btn == nullptr) {
            btn = new juce::TextButton("Push to Deck 2");
            juce::String id = juce::String(rowNumber) + "-" + juce::String(columnId);
            btn->setComponentID(id);
            btn->addListener(this);
        }
        return btn;
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button) {
    juce::String id = button->getComponentID(); 
    DBG("Button clicked: " + id);

    
    juce::StringArray tokens;
    tokens.addTokens(id, "-", ""); //first token is the row number, second token is the position of stored file address in fileLocation array

    if (tokens.size() < 2) {
        DBG("PlaylistComponent::buttonClicked - Invalid button ID format");
        return;
    }

    int fileIndex = tokens[0].getIntValue();
    int deckNumber = tokens[1].getIntValue() - 1; //column 2 = deck 1, column 3 = deck 2
    DBG("deckNum: " << deckNumber << " address pos in array: " << fileIndex);

    if (deckNumber < 1 || deckNumber > globalFileQueue.size()) {
        DBG("PlaylistComponent::buttonClicked - Invalid deck number");
        return;
    }

    // Get the file path from the stored playlist
    if (fileIndex >= 0 && fileIndex < fileLocation.size()) {
        juce::String filePath = fileLocation[fileIndex];

        // Store the file path in the correct deck queue
        globalFileQueue.getReference(deckNumber - 1).add(filePath);
        DBG("PlaylistComponent::buttonClicked - Added file to Deck " + juce::String(deckNumber));

        if (deckNumber == 1) {
            deckGUI1->loadFile(filePath);
        }
        else if (deckNumber == 2) {
            deckGUI2->loadFile(filePath);
        }
    }
    else {
        DBG("PlaylistComponent::buttonClicked - Invalid file index");
    }
}


bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files) {
    DBG("PlaylistComponent::isInterestedInFileDrag");
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y) {
    for (const auto& filePath : files) {
        juce::File file(filePath);
        trackTitles.push_back(file.getFullPathName());
        fileLocation.push_back(file.getFullPathName());
    }
    tableComponent.updateContent();
    DBG("PlaylistComponent::filesDropped");
}