/*
  ==============================================================================

    SynthVoice.cpp
    Created: 21 Aug 2024 7:49:18am
    Author:  Maximilian

  ==============================================================================
*/

#include "SynthVoice.h"

SynthVoice::SynthVoice(juce::AudioProcessorValueTreeState& tree, int numOscs, int numCombines, int numGains, int numADSRs, int numLFOs) : tree(tree)
{
    // Erstellen der prozessierenden Module
    moduleDatas.clear();
    moduleDatas.push_back(std::make_unique<Master>(tree));
    for (int i = 0; i < numOscs; i++)
    {
        moduleDatas.push_back(std::make_unique<Oscillator>(tree, i));
    }
    
    for (int i = 0; i < numCombines; i++)
    {
        moduleDatas.push_back(std::make_unique<Combine>(tree, i));
    }
    
    for (int i = 0; i < numGains; i++)
    {
        moduleDatas.push_back(std::make_unique<Gain>(tree, i));
    }
    
    for (int i = 0; i < numADSRs; i++)
    {
        moduleDatas.push_back(std::make_unique<ADSR>(tree, i));
    }
    
    for (int i = 0; i < numLFOs; i++)
    {
        moduleDatas.push_back(std::make_unique<LFO>(tree, i));
    }
    
}

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound * sound, int currentPitchWheelPosition)
{
    // Wenn eine Taste gedrückt wird, weiterleiten an die Module
    adsr.noteOn();
    for (int i = 0; i < moduleDatas.size(); i++)
    {
        moduleDatas[i]->noteOn(midiNoteNumber, velocity, currentPitchWheelPosition);
    }
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    // wenn eine Taste losgelassen wird
    adsr.noteOff();
    for (int i = 0; i < moduleDatas.size(); i++)
    {
        moduleDatas[i]->noteOff(velocity, allowTailOff);
    }
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    //nicht implementiert
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    //nicht implementiert
}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    //samplerate weitergeben etc.
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    for (int i = 0; i < moduleDatas.size(); i++)
    {
        moduleDatas[i]->prepareToPlay(spec);
    }
    
    adsrParams.attack = 0.0f;
    adsrParams.decay = 0.0f;
    adsrParams.sustain = 1.0f;
    adsrParams.release = 0.1f;
    adsr.setParameters(adsrParams);
    
    isPrepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer<float> &outPutBuffer, int startSample, int numSamples)
{
    // prozess einer einzelnen stimme
    jassert (isPrepared);
    
    if (! isVoiceActive())
    {
        return;
    }
    
    synthBuffer.setSize(outPutBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    std::vector<juce::AudioBuffer<float>> outPutBuffers;
    Graph graph(static_cast<int>(moduleDatas.size())); // DAG

    for (int i = 0; i < moduleDatas.size(); i++)
    {

        for (int j = 0; j < 10; j++) // 10 mal weil inputlimit = 10
        {
            int from = moduleDatas[i]->getInputChoice(j);
            if (from != 0)
            {
                graph.addEdge(from, i); // i = to
            }
        }
        juce::AudioBuffer<float> moduleOutputbuffer; //buffer, der später als output des moduls gelten wird
        moduleOutputbuffer.setSize(outPutBuffer.getNumChannels(), numSamples, false, false, true);
        moduleOutputbuffer.clear();
        
        outPutBuffers.push_back(moduleOutputbuffer);
    }

    juce::AudioBuffer<float> nullBuffer; // für input = none
    nullBuffer.setSize(outPutBuffer.getNumChannels(), numSamples, false, false, true);
    nullBuffer.clear();
    outPutBuffers.push_back(nullBuffer);
    
    std::vector<int> processingOrder = graph.singletopologicalSort(); // sortieren des DAG
    if (processingOrder.size() < moduleDatas.size())
    {
        return; //es gibt einen zyklus im graph (kein DAG mehr)
    }
    for (int i = 0; i < moduleDatas.size(); i++)
    {
        std::vector<juce::AudioBuffer<float>> inputBuffers;
        for (int j = 0; j < 10; j++)
        {
            int from = moduleDatas[processingOrder[i]]->getInputChoice(j);
            if (from != 0)
            {
                inputBuffers.push_back(outPutBuffers[from]); //buffer des input
            }
            else
            {
                inputBuffers.push_back(outPutBuffers.back()); //nullbuffer
            }
        }

        moduleDatas[processingOrder[i]]->process(outPutBuffers[processingOrder[i]], inputBuffers, startSample, numSamples); //prozess mit eigenem outputbuffer (write) und anderen inputbuffer (read)
    }
    
    for (int channel = 0; channel < outPutBuffer.getNumChannels(); ++channel)
    {
        synthBuffer.addFrom(channel, 0, outPutBuffers[0], channel, 0, numSamples); // synthoutput = master = outputbuffers[0]
    }
    
    //adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples()); evt. globaler adsr
    
    for (int channel = 0; channel < outPutBuffer.getNumChannels(); ++channel)
    {
        outPutBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        
        //if (! adsr.isActive()) { clearCurrentNote(); }
    }
}
