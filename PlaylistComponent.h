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

//==============================================================================
/*
*/
class PlaylistComponent  : 
    public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    //minimum requirements (the pure virtual fn), for TableListBoxModel
    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    //minimum requirements (the pure virtual fn), for Button::Listener
    void buttonClicked(juce::Button* button) override;

private:

    juce::TableListBox tableComponent;

    juce::TextEditor trackTitleEditor; //the text editor
    std::vector<juce::String> trackTitles; //now stores typed text

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
