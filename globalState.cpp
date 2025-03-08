/*
  ==============================================================================

    globalState.cpp
    Created: 7 Mar 2025 10:56:34pm
    Author:  JJ

  ==============================================================================
*/

#include "globalState.h"

juce::Array<juce::Array<juce::String>> globalFileQueue; 

void initializeGlobalState()
{
    globalFileQueue.add(juce::Array<juce::String>()); //deck 1 queue
    globalFileQueue.add(juce::Array<juce::String>()); //deck 2 queue
    DBG("global state initialised");
}
