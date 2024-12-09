/*
  ==============================================================================

    Gain.cpp
    Created: 1 Oct 2024 4:54:13pm
    Author:  Maximilian

  ==============================================================================
*/

#include "Gain.h"

Gain::Gain(juce::AudioProcessorValueTreeState& tree, int moduleID) : tree(tree), moduleID(moduleID)
{
    
}

int Gain::getInputChoice (int inputIndex)
{
    if (inputIndex < 0 || inputIndex > 10)
    {
        return 0;
    }
    auto& choice =  *tree.getRawParameterValue("GAIN_" + std::to_string(moduleID) + "_" + std::to_string(inputIndex));
    return choice.load();
}

void Gain::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    sampleRate = spec.sampleRate;
}

void Gain::noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition)
{
    
}

void Gain::noteOff (float velocity, bool allowTailOff)
{
    
}

float Gain::getSample(float inSample, float addition)
{
    if (dbMode)
    {
        return std::pow(10.0f, (gainValue + addition) / 20.0f) * inSample; //Dezibel
    }
    else
    {
        return (gainValue + addition) * inSample;
    }
}

void Gain::process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples)
{
    if (!*tree.getRawParameterValue("ISGAINACTIVE_" + std::to_string(moduleID))) { return; }
    moduleBuffer.setSize(outPutBuffer.getNumChannels(), numSamples);
    moduleBuffer.clear();
    for (int channel = 0; channel < outPutBuffer.getNumChannels(); ++channel)
    {
        moduleBuffer.addFrom(channel, 0, inputBuffers[1], channel, 0, numSamples);
    }
    
    juce::dsp::AudioBlock<float> moduleBlock {moduleBuffer};
    auto& rawGainValue = *tree.getRawParameterValue("GAINVALUE_" + std::to_string(moduleID));
    gainValue = rawGainValue.load();
    dbMode = *tree.getRawParameterValue("DBMODE_" + std::to_string(moduleID));
    
    for (int channel = 0; channel < outPutBuffer.getNumChannels(); ++channel)
    {
        float *const buffer = moduleBuffer.getWritePointer(channel);
        float const* modBuffer = inputBuffers[0].getReadPointer(channel);
        for (int sample = 0; sample < numSamples; sample++)
        {
            float base = buffer[sample];
            float addition = modBuffer[sample];
            float result =  getSample(base, addition * 1.0f);
            buffer[sample] = result;
        }
    }
    
    
    for (int channel = 0; channel < outPutBuffer.getNumChannels(); ++channel)
    {
        outPutBuffer.addFrom(channel, 0, moduleBuffer, channel, 0, numSamples);
    }
}
