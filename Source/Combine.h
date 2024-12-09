/*
  ==============================================================================

    Combine.h
    Created: 21 Aug 2024 7:51:47am
    Author:  Maximilian

  ==============================================================================
*/

#pragma once
#include "Module.h"

class Combine : public Module
{
public:
    Combine(juce::AudioProcessorValueTreeState& tree, int moduleID);
    
    int getInputChoice (int inputIndex) override;
    
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    
    void noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition) override;
    void noteOff (float velocity, bool allowTailOff) override;
    
    void process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples) override;
    
private:
    juce::AudioProcessorValueTreeState& tree;
    const int moduleID;
};
