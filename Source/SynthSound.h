/*
  ==============================================================================

    SynthSound.h
    Created: 21 Aug 2024 7:49:28am
    Author:  Maximilian

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override { return true; }
    bool appliesToChannel (int midiChannel) override { return true; }
    
};
