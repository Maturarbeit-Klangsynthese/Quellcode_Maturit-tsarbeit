/*
  ==============================================================================

    Master.cpp
    Created: 21 Aug 2024 7:51:13am
    Author:  Maximilian

  ==============================================================================
*/

#include "Master.h"

Master::Master(juce::AudioProcessorValueTreeState& tree) : tree(tree)
{
    
}

int Master::getInputChoice (int inputIndex)
{
    if (inputIndex < 0 || inputIndex > 10)
    {
        return 0;
    }
    auto& choice = *tree.getRawParameterValue("MASTER_" + std::to_string(inputIndex));
    return choice.load();
}

void Master::prepareToPlay (juce::dsp::ProcessSpec spec)
{
    
}

void Master::noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition)
{
    
}

void Master::noteOff (float velocity, bool allowTailOff)
{
    
}

void Master::process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples)
{
    if (inputBuffers.size() > 0)
    {
        for (int channel = 0; channel < outPutBuffer.getNumChannels(); ++channel)
        {
            outPutBuffer.addFrom(channel, 0, inputBuffers[0], channel, 0, numSamples);
        }
    }
}
