/*
  ==============================================================================

    Module.h
    Created: 21 Aug 2024 7:50:59am
    Author:  Maximilian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Module
{
public:
    Module();
    virtual ~Module();
    
    virtual int getInputChoice (int inputIndex) = 0;
    
    virtual void prepareToPlay (juce::dsp::ProcessSpec spec) = 0;
    virtual void update (float bpm);
    
    virtual void noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition) = 0;
    virtual void noteOff (float velocity, bool allowTailOff) = 0;
    
    virtual void process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples) = 0;
    
    juce::AudioBuffer<float> moduleBuffer;
    
    float beatsPerMinute = 120.0f; // habe ich so festgelegt
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Module)
};
