/*
  ==============================================================================

    Gain.h
    Created: 1 Oct 2024 4:54:13pm
    Author:  Maximilian

  ==============================================================================
*/

#pragma once
#include "Module.h"

class Gain : public Module
{
public:
    Gain(juce::AudioProcessorValueTreeState& tree, int moduleID);
    
    int getInputChoice (int inputIndex) override;
    
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    
    void noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition) override;
    void noteOff (float velocity, bool allowTailOff) override;
    
    float getSample(float inSample, float addition);
    
    void process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples) override;
    
private:
    float sampleRate = 1.0f; // benötige ich eigentlich nicht, weil ich keine gainramp habe
    float gainValue;
    bool dbMode = true;
    
    juce::AudioProcessorValueTreeState& tree;
    const int moduleID;
};
