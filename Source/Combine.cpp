/*
  ==============================================================================

    Combine.cpp
    Created: 21 Aug 2024 7:51:47am
    Author:  Maximilian

  ==============================================================================
*/

#include "Combine.h"

Combine::Combine(juce::AudioProcessorValueTreeState& tree, int moduleID) : tree(tree), moduleID(moduleID)
{
    
}

int Combine::getInputChoice (int inputIndex)
{
    if (inputIndex < 0 || inputIndex > 10)
    {
        return 0;
    }
    auto& choice =  *tree.getRawParameterValue("COMBINE_" + std::to_string(moduleID) + "_" + std::to_string(inputIndex));
    return choice.load();
}

void Combine::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    
}

void Combine::noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition)
{
    
}

void Combine::noteOff (float velocity, bool allowTailOff)
{
    
}

void Combine::process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples)
{
    if (!*tree.getRawParameterValue("ISCOMBINEACTIVE_" + std::to_string(moduleID))) { return; }
    moduleBuffer.setSize(outPutBuffer.getNumChannels(), numSamples);
    moduleBuffer.clear();
    
    juce::AudioBuffer<float> helperBuffer1;
    juce::AudioBuffer<float> helperBuffer2;
    
    helperBuffer1.setSize(outPutBuffer.getNumChannels(), numSamples);
    helperBuffer1.clear();
    helperBuffer2.setSize(outPutBuffer.getNumChannels(), numSamples);
    helperBuffer2.clear();
    
    juce::dsp::AudioBlock<float> block1 {helperBuffer1};
    juce::dsp::AudioBlock<float> block2 {helperBuffer2};

    for (int channel = 0; channel < helperBuffer1.getNumChannels(); ++channel)
    {
        helperBuffer1.addFrom(channel, 0, inputBuffers[0], channel, 0, numSamples);
        helperBuffer2.addFrom(channel, 0, inputBuffers[1], channel, 0, numSamples);
    }
    
    block1.multiplyBy(*tree.getRawParameterValue("MIX_" + std::to_string(moduleID)) / 100.0f);
    block2.multiplyBy(1.0f - *tree.getRawParameterValue("MIX_" + std::to_string(moduleID)) / 100.0f);

    for (int channel = 0; channel < moduleBuffer.getNumChannels(); ++channel)
    {
        moduleBuffer.addFrom(channel, 0, helperBuffer1, channel, 0, numSamples);
        moduleBuffer.addFrom(channel, 0, helperBuffer2, channel, 0, numSamples);
    }
    
    for (int channel = 0; channel < outPutBuffer.getNumChannels(); ++channel)
    {
        outPutBuffer.addFrom(channel, 0, moduleBuffer, channel, 0, numSamples);
    }
}
