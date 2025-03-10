/*
  ==============================================================================

    globalState.h
    Created: 7 Mar 2025 10:56:34pm
    Author:  JJ

  ==============================================================================
*/

#pragma once

#include <juce_core/juce_core.h>

extern juce::Array<juce::Array<juce::String>> globalFileQueue; // inner array holds songs, external array holds playlists

void initializeGlobalState();