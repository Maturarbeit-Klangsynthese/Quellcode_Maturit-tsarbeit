/*
  ==============================================================================

    SynthVoice.h
    Created: 21 Aug 2024 7:49:18am
    Author:  Maximilian

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "Module.h"
#include "Master.h"
#include "Oscillator.h"
#include "Combine.h"
#include "Gain.h"
#include "ADSR.h"
#include "LFO.h"

#include "Graph.h"

#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice(juce::AudioProcessorValueTreeState& tree, int numOscs, int numCombines, int numGains, int numADSRs, int numLFOs);
    
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound * sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    
    void renderNextBlock (juce::AudioBuffer<float> &outPutBuffer, int startSample, int numSamples) override;
    
    bool checkFeedback (std::vector<int>& vectorToCheck, int intToCheck);
    
    juce::dsp::ProcessSpec spec;
    
private:
    juce::AudioProcessorValueTreeState& tree; //referenz auf apvts
    
    std::vector<std::unique_ptr<Module>> moduleDatas; // Module mit prozesseigenschaften
    
    juce::ADSR adsr; // evt. globaler adsr
    juce::ADSR::Parameters adsrParams;
    
    juce::AudioBuffer<float> synthBuffer;
    bool isPrepared { false };
};
