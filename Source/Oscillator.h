/*
  ==============================================================================

    Oscillator.h
    Created: 21 Aug 2024 7:51:25am
    Author:  Maximilian

  ==============================================================================
*/

#pragma once
#include "Module.h"

class Oscillator : public Module
{
public:
    Oscillator(juce::AudioProcessorValueTreeState& tree, int moduleID);
    
    int getInputChoice (int inputIndex) override;
    
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    
    void noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition) override;
    void noteOff (float velocity, bool allowTailOff) override;
    
    void setWave(const std::function<float(float)>& waveFunc);
    float getSample();
    
    void frequency();
    void wave();
    
    void process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples) override;
private:
    std::function<float(float)> waveForm = [](float x) {return std::sin (x); };
    float phase = 0.0f;
    float freq = 5.0f;
    
    
    juce::AudioProcessorValueTreeState& tree;
    const int moduleID;
    
    int samplesPerBlock;
    int sampleRate;
    int outputChannels;
    
    int currentNote = 0;
    int waveChoice;
    
};
