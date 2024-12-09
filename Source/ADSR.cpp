/*
  ==============================================================================

    ADSR.cpp
    Created: 8 Oct 2024 6:23:45pm
    Author:  Maximilian Aebersold

  ==============================================================================
*/

#include "ADSR.h"

ADSR::ADSR(juce::AudioProcessorValueTreeState& tree, int moduleID) : tree(tree), moduleID(moduleID)
{
    
}

int ADSR::getInputChoice (int inputIndex)
{
    if (inputIndex < 0 || inputIndex > 10)
    {
        return 0;
    }
    auto& choice =  *tree.getRawParameterValue("ADSR_" + std::to_string(moduleID) + "_" + std::to_string(inputIndex));
    return choice.load();
}

void ADSR::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;
}

void ADSR::noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition)
{
    adsrState = 1;
    phase = 0.0f;
}

void ADSR::noteOff (float velocity, bool allowTailOff)
{
    if (adsrState == 1 || adsrState == 2 || adsrState == 3) // die noteOff messages kommen manchmal an unerwarteten zeitpunkten vor
    {
        adsrState = 4;
        phase = 0.0f;

    }
}

float ADSR::getSample()
{
    if (adsrState == 1)
    {
        phase += 1.0f / (sampleRate * tA);
        if (phase < 1.0f)
        {
            float value = getPointInLine(vMin, vA, phase);
            currentValue = value;
            return value;
        }
        adsrState = 2;
        phase = 0.0f;
    }
    if (adsrState == 2)
    {
        phase += 1.0f / (sampleRate * tD);
        if (phase < 1.0f)
        {
            float value = getPointInLine(vA, vS, phase);
            currentValue = value;
            return value;
        }
        adsrState = 3;
        phase = 0.0f;
    }
    if (adsrState == 3)
    {
        float value = vS;
        currentValue = value;
        return value;
    }
    if (adsrState == 4)
    {
        phase += 1.0f / (sampleRate * tR);
        if (phase < 1.0f)
        {
            float value = getPointInLine(currentValue, vMin, phase);
            return value;
        }
        phase = 0.0f;
        adsrState = 0;
    }
    return vMin;
}

float ADSR::getPointInLine(float start, float end, float phase)
{
    return start + ((end - start) * phase);
}

void ADSR::update()
{
    
    tA = *tree.getRawParameterValue("ATTACKTIME_" + std::to_string(moduleID));
    tD = *tree.getRawParameterValue("DECAYTIME_" + std::to_string(moduleID));
    vS = *tree.getRawParameterValue("SUSTAINVALUE_" + std::to_string(moduleID));
    tR = *tree.getRawParameterValue("RELEASETIME_" + std::to_string(moduleID));
    
    vMax = *tree.getRawParameterValue("MAXVALUE_" + std::to_string(moduleID));
    vMin = *tree.getRawParameterValue("MINVALUE_" + std::to_string(moduleID));
    
    vS = vS * vMax;
    vA = vMax;
}

void ADSR::process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples)
{
    if (!*tree.getRawParameterValue("ISADSRACTIVE_" + std::to_string(moduleID))) { return; }
    moduleBuffer.setSize(outPutBuffer.getNumChannels(), numSamples);
    moduleBuffer.clear();
    
    update();

    float startPhase = phase;
    int startADSRState = adsrState;
    
    for (int channel = 0; channel < moduleBuffer.getNumChannels(); ++channel)
    {
        phase = startPhase;
        adsrState = startADSRState;
        
        float* const buffer = moduleBuffer.getWritePointer(channel);
        for (int sample = 0; sample < moduleBuffer.getNumSamples(); sample++)
        {
            float value = getSample();
            buffer[sample] = value;
        }
    }
    
    for (int channel = 0; channel < outPutBuffer.getNumChannels(); ++channel)
    {
        outPutBuffer.addFrom(channel, 0, moduleBuffer, channel, 0, numSamples);
    }
}
