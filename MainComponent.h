#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "GlobalState.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

private:

    /*creates random*/
    juce::Random random;

    /////////////////////////////////////////////////////////////

    DJAudioPlayer player1{ formatManager , deckGUI1};
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };
    DJAudioPlayer player2{ formatManager, deckGUI2 };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache };

    /////////////////////////////////////////////////////////////

    juce::MixerAudioSource mixerSource;

    /////////////////////////////////////////////////////////////

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{ 100 };

    /////////////////////////////////////////////////////////////

    PlaylistComponent playlistComponent;

    /////////////////////////////////////////////////////////////

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};