/*
  ==============================================================================

    LFO.cpp
    Created: 10 Oct 2024 11:52:40am
    Author:  Maximilian Aebersold

  ==============================================================================
*/

#include "LFO.h"

LFO::LFO(juce::AudioProcessorValueTreeState& tree, int moduleID) : tree(tree), moduleID(moduleID)
{
    
}

int LFO::getInputChoice (int inputIndex)
{
    if (inputIndex < 0 || inputIndex > 10)
    {
        return 0;
    }
    auto& choice =  *tree.getRawParameterValue("LFO_" + std::to_string(moduleID) + "_" + std::to_string(inputIndex));
    return choice.load();
}

void LFO::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    samplesPerBlock = spec.maximumBlockSize;
    sampleRate = spec.sampleRate;
    outputChannels = spec.numChannels;
}

void LFO::noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition)
{
    phase = 0.0f;

}
void LFO::noteOff (float velocity, bool allowTailOff)
{
    
}

void LFO::frequency ()
{
    if (*tree.getRawParameterValue("USEBPM_" + std::to_string(moduleID)))
    {
        auto top = tree.getRawParameterValue("BPMRATETOP_" + std::to_string(moduleID))->load();
        auto bottom = tree.getRawParameterValue("BPMRATEBOTTOM_" + std::to_string(moduleID))->load();
        rate = ((top / bottom) / (beatsPerMinute / 60.0f)) * 4.0f;
    }
    else
    {
        rate = *tree.getRawParameterValue("RATE_" + std::to_string(moduleID));
    }
}

void LFO::setWave (const std::function<float(float)>& waveFunc)
{
    waveForm = waveFunc;
}

float LFO::getSample()
{
    phase += juce::MathConstants<float>::twoPi / (sampleRate * rate);
    phase = std::fmod(phase, juce::MathConstants<float>::twoPi);
    return waveForm(phase);
}

void LFO::wave ()
{
    int newWaveChoice = *tree.getRawParameterValue("LFOWAVECHOICE_" + std::to_string(moduleID));
    
    if (waveChoice != newWaveChoice)
    {
        waveChoice = newWaveChoice;

        if (waveChoice == 0)
        {
            setWave([](float phase) { return std::sin(phase); });
        }
        else if (waveChoice == 1)
        {
            setWave([](float phase) { return 2.0f * (phase / juce::MathConstants<float>::twoPi) - 1.0f; }); // Sawtooth
        }
        else if (waveChoice == 2)
        {
            setWave([](float phase) { return phase < juce::MathConstants<float>::pi ? 1.0f : -1.0f; }); // Square wave
        }
    }
}

void LFO::process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples)
{
    if (!*tree.getRawParameterValue("ISLFOACTIVE_" + std::to_string(moduleID))) { return; }
    wave();
    frequency();
    
    float startPhase = phase;
    float baseFreq = rate;
    
    for (int channel = 0; channel < outPutBuffer.getNumChannels(); channel++)
    {
        phase = startPhase;
        float* const buffer = outPutBuffer.getWritePointer(channel);
        float const* mBuffer = inputBuffers[0].getReadPointer(channel);

        for (int sample = 0; sample < outPutBuffer.getNumSamples(); sample++)
        {
            rate = baseFreq + 10.0f * mBuffer[sample];
            float sampleValue = getSample();
            buffer[sample] = juce::jlimit(-1.0f, 1.0f, sampleValue);
        }
    }
}
