/*
  ==============================================================================

    globalState.cpp
    Created: 7 Mar 2025 10:56:34pm
    Author:  JJ

  ==============================================================================
*/

#include "globalState.h"

juce::Array<juce::Array<juce::String>> globalFileQueue; // Declare global array

// Initialize the array with two empty juce::Array<String> elements
void initializeGlobalState()
{
    globalFileQueue.add(juce::Array<juce::String>()); // Deck 1 queue
    globalFileQueue.add(juce::Array<juce::String>()); // Deck 2 queue
    DBG("test");
}
