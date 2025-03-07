#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate){
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
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!

    g.fillAll(juce::Colours::gainsboro);

    g.setColour(juce::Colours::dimgrey);
    g.setFont(20.0f);
    g.drawText(" ", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized(){
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() *2 / 3);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() * 2 / 3);
    playlistComponent.setBounds(0, getHeight() / 3 * 2, getWidth(), getHeight() / 3);
}






















//void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
//{
    // Your audio-processing code goes here!

    /*for white noise, channel 0, start writing at this sample, channel 1 to avoid overlap? */
    //auto* leftChan = bufferToFill.buffer->getWritePointer(0,bufferToFill.startSample); 
    //auto* rightChan = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    //for (auto i = 0; i < bufferToFill.numSamples; ++i) {

        //pick one method below

        // 1 continuous sound, change volume slider manipulation * 0.0001
        //double sample = random.nextDouble() * 0.005;
        // 2 phase, change volume slider manipulation * 0.0001
        //double sample = fmod(phase, 0.2);
        //3 sin curve, change volume slider manipulation * 0.001
        //double sample = sin(phase) * 100;

        //leftChan[i] = sample;
        //rightChan[i] = sample;
        //rate of change
        //phase += dphase;
    //}

    // For more details, see the help for AudioProcessor::getNextAudioBlock()
    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    //bufferToFill.clearActiveBufferRegion();
//}