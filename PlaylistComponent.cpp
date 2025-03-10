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
    juce::Colour pennRed(149, 25, 12), berkeleyBlue(18, 53, 91), darkPurple(66, 0, 57);

    tableComponent.getHeader().addColumn("Track Title / Description", 1, 350);
    tableComponent.getHeader().addColumn("", 2, 150);
    tableComponent.getHeader().addColumn("", 3, 150);
    tableComponent.getHeader().addColumn("", 4, 150);
    tableComponent.setModel(this);
    tableComponent.setColour(juce::TableListBox::backgroundColourId, berkeleyBlue);
    addAndMakeVisible(tableComponent);

    loadPlaylist();

    addAndMakeVisible(loadPlaylistButton);
    loadPlaylistButton.setColour(juce::TextButton::buttonColourId, darkPurple);
    loadPlaylistButton.addListener(this);
    
    addAndMakeVisible(savePlaylistButton);
    savePlaylistButton.setColour(juce::TextButton::buttonColourId, darkPurple);
    savePlaylistButton.addListener(this);
}

PlaylistComponent::~PlaylistComponent() {
}

void PlaylistComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void PlaylistComponent::resized() {
    tableComponent.setBounds(0, 0, getWidth(), getHeight() - 20);
    trackTitleEditor.setBounds(0, 0, getWidth() - 20, 30);
    loadPlaylistButton.setBounds(getWidth() - 100, getHeight()-20, 50, 20);
    savePlaylistButton.setBounds(getWidth() - 50, getHeight()-20, 50, 20);
}

int PlaylistComponent::getNumRows() {
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    juce::Colour pennRed(149, 25, 12), berkeleyBlue(18, 53, 91), darkPurple(66, 0, 57);
    g.fillRect(0, 0, width, height);
    if (rowIsSelected) {
        g.fillAll(darkPurple);
    }
    else {
        g.fillAll(berkeleyBlue);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height - 2, juce::Justification::centredLeft, true);
    juce::Colour pennRed(149, 25, 12), berkeleyBlue(18, 53, 91), darkPurple(66, 0, 57);
    g.fillAll(berkeleyBlue);
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

    juce::StringArray tokens;
    tokens.addTokens(id, "-", ""); //first token is the row number, second token is the position of stored file address in fileLocation array

    int tableRow = tokens[0].getIntValue(); // rows
    int tableColumn = tokens[1].getIntValue() - 1; //column 2 = deck 1, column 3 = deck 2, column 4 = remove button

    if (tableColumn == 1 || tableColumn == 2) {
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
    if (tableColumn == 3) {
        if (tableRow >= 0 && tableRow < static_cast<int>(fileLocation.size())) {
            fileLocation.erase(fileLocation.begin() + tableRow);
        }
        if (tableRow >= 0 && tableRow < static_cast<int>(trackTitles.size())) {
            trackTitles.erase(trackTitles.begin() + tableRow); 
        }
        DBG("PlaylistComponent::buttonClicked - item removed");
        tableComponent.updateContent();
    }
    if (button == &loadPlaylistButton) {
        DBG("PlaylistComponent::buttonClicked loadPlaylistButton");
        loadPlaylist();
    }
    if (button == &savePlaylistButton) {
        DBG("PlaylistComponent::buttonClicked savePlaylistButton");
        savePlaylist();
    }
}

void PlaylistComponent::updateTableContentAndRebuildButtons() {
    for (int row = 0; row < fileLocation.size(); ++row) {
        juce::TextButton* removeButton = dynamic_cast<juce::TextButton*>(tableComponent.getCellComponent(3, row));
        if (removeButton != nullptr) {
            removeButton->setComponentID(juce::String(row) + "-4");
        }
    }
    tableComponent.updateContent();
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

void PlaylistComponent::savePlaylist() { // save in CWD
    juce::File playlistFile(juce::File::getCurrentWorkingDirectory().getChildFile("playlist.json"));

    juce::DynamicObject::Ptr jsonObject = new juce::DynamicObject();
    juce::Array<juce::var> tracksArray;

    for (const auto& file : fileLocation) {
        tracksArray.add(file);
    }

    jsonObject->setProperty("tracks", tracksArray);
    juce::var jsonData = juce::var(jsonObject.get());

    if (playlistFile.replaceWithText(juce::JSON::toString(jsonData))) {
        DBG("Playlist saved to: " << playlistFile.getFullPathName() << " track array size: " << juce::String(tracksArray.size()));
    }
    else {
        DBG("Failed to save playlist.");
    }
}

void PlaylistComponent::loadPlaylist() {
    juce::File playlistFile(juce::File::getCurrentWorkingDirectory().getChildFile("playlist.json"));
    if (playlistFile.existsAsFile()) {
        DBG("JSON file found");
        juce::var jsonData = juce::JSON::parse(playlistFile.loadFileAsString());

        if (jsonData.isObject()) {
            DBG("JSON data object recognised");
            if (auto* tracksArray = jsonData.getProperty("tracks", juce::var()).getArray()) {
                fileLocation.clear();
                int x = 0;
                for (auto& track : *tracksArray) {
                    fileLocation.push_back(track.toString());
                    trackTitles.push_back(track.toString());
                    x += 1;
                    DBG("Tracks added: " << x << " : " << track.toString());
                }
                DBG("JSON data loaded");
                tableComponent.updateContent();
                tableComponent.repaint();
            }
        }
    }
    DBG("Entries in fileLocation contains: " << fileLocation.size());
    DBG("Entries in fileLocation contains: " << trackTitles.size());
}