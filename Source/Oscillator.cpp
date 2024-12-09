/*
  ==============================================================================

    Oscillator.cpp
    Created: 21 Aug 2024 7:51:25am
    Author:  Maximilian

  ==============================================================================
*/

#include "Oscillator.h"

Oscillator::Oscillator(juce::AudioProcessorValueTreeState& tree, int moduleID) : tree(tree), moduleID(moduleID)
{
    
}

int Oscillator::getInputChoice (int inputIndex)
{
    if (inputIndex < 0 || inputIndex > 10)
    {
        return 0;
    }
    auto& choice =  *tree.getRawParameterValue("OSC_" + std::to_string(moduleID) + "_" + std::to_string(inputIndex));
    return choice.load();
}

void Oscillator::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    samplesPerBlock = spec.maximumBlockSize;
    sampleRate = spec.sampleRate;
    outputChannels = spec.numChannels;
}

void Oscillator::noteOn (int midiNoteNumber, float velocity, int currentPitchWheelPosition)
{
    currentNote = midiNoteNumber;
    phase = 0.0f;

}
void Oscillator::noteOff (float velocity, bool allowTailOff)
{
    
}

void Oscillator::frequency ()
{
    freq = juce::MidiMessage::getMidiNoteInHertz(currentNote + *tree.getRawParameterValue("TRANSPOSE_" + std::to_string(moduleID)));
}

void Oscillator::setWave (const std::function<float(float)>& waveFunc)
{
    waveForm = waveFunc;
}

float Oscillator::getSample()
{
    phase += juce::MathConstants<float>::twoPi * freq / sampleRate;
    if (phase > juce::MathConstants<float>::twoPi)
    {
        phase = 0.0f;
    }
    else if (phase < 0.0f)
    {
        phase = juce::MathConstants<float>::twoPi;
    }
    return waveForm(phase);
}

void Oscillator::wave ()
{
    int newWaveChoice = *tree.getRawParameterValue("WAVECHOICE_" + std::to_string(moduleID));
    
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

void Oscillator::process(juce::AudioBuffer<float> &outPutBuffer, std::vector<juce::AudioBuffer<float>> inputBuffers, int startSample, int numSamples)
{
    if (!*tree.getRawParameterValue("ISOSCACTIVE_" + std::to_string(moduleID))) { return; }
    moduleBuffer.setSize(outPutBuffer.getNumChannels(), numSamples);
    moduleBuffer.clear();
    wave();
    frequency();
    
    float startPhase = phase;
    float baseFreq = freq;
    
    for (int channel = 0; channel < outPutBuffer.getNumChannels(); channel++)
    {
        phase = startPhase;
        float* const buffer = outPutBuffer.getWritePointer(channel);
        float const* fmBuffer = inputBuffers[0].getReadPointer(channel);

        for (int sample = 0; sample < outPutBuffer.getNumSamples(); sample++)
        {
            freq = baseFreq + 10.0f * fmBuffer[sample];
            float sampleValue = getSample();
            buffer[sample] = juce::jlimit(-1.0f, 1.0f, sampleValue);
        }
    }
}
