#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : playlistComponent(&deckGUI1, &deckGUI2)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();

    initializeGlobalState();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) { //getNextAudioBlock function is used to supply audio blocks to the system
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources() {
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
    mixerSource.removeAllInputs();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g) {
    juce::Colour pennRed(149, 25, 12), berkeleyBlue(18, 53, 91), darkPurple(66, 0, 57);
    g.fillAll(berkeleyBlue);
}

void MainComponent::resized() {
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() * 2 / 3);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() * 2 / 3);
    playlistComponent.setBounds(0, getHeight() / 3 * 2, getWidth(), getHeight() / 3);
}