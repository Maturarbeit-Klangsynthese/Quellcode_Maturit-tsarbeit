/*
  ==============================================================================

    ADSR.h
    Created: 8 Oct 2024 6:23:45pm
    Author:  Maximilian Aebersold

  ==============================================================================
*/

#pragma once
#include "Module.h"

class ADSR : public Module
{
public:
    ADSR(juce::AudioProcessorValueTreeState& tree, int moduleID);
    
    int getInputChoice (int inputIndex) override;
    
    void prepareToPlay(juce::dsp::ProcessSpec spec) override;
    
    void noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition) override;
    void noteOff (float velocity, bool allowTailOff) override;
    
    float getSample();
    float getPointInLine(float start, float end, float phase);
    
    void update();
    void process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples) override;
    
private:
    juce::AudioProcessorValueTreeState& tree;
    const int moduleID;
    
    float tA = 1.0f; //time Attack
    float tD = 1.0f; //time Decay
    float tR = 1.0f; //time Release
    
    float vA = 1.0f; //value Attack, = vMax
    float vS = 1.0f; //value Sustain
    
    float vMax = 1.0f; //obergrenze
    float vMin = 0.0f; //untergrenze

    float currentValue = 0.0f;
    
    int sampleRate = 0;
    int adsrState = 0; // 0 = off, 1 = attack, 2 = decay, 3 = sustain, 4 = release
    float phase = 0.0f;
};
