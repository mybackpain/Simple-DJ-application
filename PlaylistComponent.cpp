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
    tableComponent.getHeader().addColumn("Track Title / Description", 1, 350);
    tableComponent.getHeader().addColumn("", 2, 150);
    tableComponent.getHeader().addColumn("", 3, 150);
    tableComponent.getHeader().addColumn("", 4, 150);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    loadPlaylist();
}

PlaylistComponent::~PlaylistComponent() {
}

void PlaylistComponent::paint(juce::Graphics& g) {
    juce::Colour pennRed(149, 25, 12), berkeleyBlue(18, 53, 91), darkPurple(66, 0, 57);

    g.fillAll(berkeleyBlue);
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(pennRed);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
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
    if (columnId == 2) { // send to deck 1
        auto* btn = dynamic_cast<juce::TextButton*>(existingComponentToUpdate);
        if (btn == nullptr) {
            btn = new juce::TextButton("Push to Deck 1");
            juce::String id = juce::String(rowNumber) + "-" + juce::String(columnId);
            btn->setComponentID(id);
            btn->addListener(this);
        }
        return btn;
    }
    if (columnId == 3) { // send to deck 2
        auto* btn = dynamic_cast<juce::TextButton*>(existingComponentToUpdate);
        if (btn == nullptr) {
            btn = new juce::TextButton("Push to Deck 2");
            juce::String id = juce::String(rowNumber) + "-" + juce::String(columnId);
            btn->setComponentID(id);
            btn->addListener(this);
        }
        return btn;
    }
    if (columnId == 4) { // delete
        auto* btn = dynamic_cast<juce::TextButton*>(existingComponentToUpdate);
        if (btn == nullptr) {
            btn = new juce::TextButton("Remove");
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

    int tableRow = tokens[0].getIntValue(); // rows
    int tableColumn = tokens[1].getIntValue() - 1; //column 2 = deck 1, column 3 = deck 2, column 4 = remove button
    DBG("tableRow: " << tableRow << " tableColumn: " << tableColumn);

    if (tableRow < 0 || tableRow >= fileLocation.size()) {
        DBG("PlaylistComponent::buttonClicked - Invalid file index: " << tableRow);
        return;
    }

    if (tableRow >= 0 && tableRow < fileLocation.size()) {
        juce::String filePath = fileLocation[tableRow];

        globalFileQueue.getReference(tableColumn - 1).add(filePath);
        DBG("PlaylistComponent::buttonClicked - Added file to Deck " + juce::String(tableColumn));

        if (tableColumn == 1) {
            deckGUI1->loadFile(filePath);
        }
        if (tableColumn == 2) {
            deckGUI2->loadFile(filePath);
        }
    }
    if (tableColumn == 3) { // "Remove" button
        DBG("Removing file at index: " << tableRow);
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
    DBG("PlaylistComponent::filesDropped " << fileLocation.size() << " files stored");
}

void PlaylistComponent::savePlaylist() {
    juce::File playlistFile(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("playlist.json"));

    juce::DynamicObject::Ptr jsonObject = new juce::DynamicObject();
    juce::Array<juce::var> tracksArray;

    for (const auto& file : fileLocation) {
        tracksArray.add(file);
    }

    jsonObject->setProperty("tracks", tracksArray);
    juce::var jsonData = juce::var(jsonObject.get());

    playlistFile.replaceWithText(juce::JSON::toString(jsonData));
}

void PlaylistComponent::loadPlaylist() {
    juce::File playlistFile(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("playlist.json"));

    if (playlistFile.existsAsFile()) {
        juce::var jsonData = juce::JSON::parse(playlistFile.loadFileAsString());

        if (jsonData.isObject()) {
            if (auto* tracksArray = jsonData.getProperty("tracks", juce::var()).getArray()) {
                fileLocation.clear();
                for (auto& track : *tracksArray) {
                    fileLocation.push_back(track.toString());
                }
                tableComponent.updateContent();
            }
        }
    }
}