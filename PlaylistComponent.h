/*
  ==============================================================================

    PlaylistComponent.h
    Created: 24 Jan 2025 12:44:29pm
    Author:  JJ

  ==============================================================================
*/

#pragma once
#include <vector>
#include <JuceHeader.h>
#include "GlobalState.h"
#include "DeckGUI.h"

//==============================================================================
/*
*/
class PlaylistComponent :
    public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener,
    public juce::FileDragAndDropTarget
{
public:
    PlaylistComponent(DeckGUI* deck1, DeckGUI* deck2);
    ~PlaylistComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    int getNumRows() override; //minimum requirements (the pure virtual fn), for TableListBoxModel
    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    void buttonClicked(juce::Button* button) override; //minimum requirements (the pure virtual fn), for Button::Listener

    std::vector<juce::String> trackTitles; //stores typed text
    std::vector<juce::String> fileLocation; //stores unaltered file location of music
    juce::Array<juce::String> filePaths;

private:

    ///////////////////////////////////////////////////////////// table actual, text editor in table

    juce::TableListBox tableComponent;
    juce::TextEditor trackTitleEditor; 

    ///////////////////////////////////////////////////////////// load file

    bool isInterestedInFileDrag(const juce::StringArray& files);
    void filesDropped(const juce::StringArray& files, int x, int y);

    ///////////////////////////////////////////////////////////// ref pointers (to pass music to deck)

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    ///////////////////////////////////////////////////////////// save button, load button

    juce::TextButton savePlaylistButton{ "Save playlist" };
    juce::TextButton loadPlaylistButton{ "Load playlist" };

    void loadPlaylist();
    void savePlaylist();

    ///////////////////////////////////////////////////////////// delete button

    void updateTableContentAndRebuildButtons();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};