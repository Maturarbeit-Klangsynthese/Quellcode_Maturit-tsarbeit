/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SynthVoice.h"
#include "SynthSound.h"

#include "Master.h"

//==============================================================================
/**
*/
class SynthPONCAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SynthPONCAudioProcessor();
    ~SynthPONCAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    // Ab hier habe ich programmiert
    //==============================================================================
      
    void setSamplesPerBlock(int samplesPerBlock);
    int getSamplesPerBlock();
    
    void addModuleButtonClicked (int moduleChoice);
    void addModule();
    
    void removeModule(Module* module);
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    void createOscParams(int OscID);
    void createCombineParams(int CombineID);
    void createGainParams(int GainID);
    void createADSRParams(int ADSRID);
    void createLFOParams(int LFOID);

    void createMasterParams();
    void createInputParams();
    
    
    // Jegliche Limits, die man festlegen kann
    
    static constexpr int moduleLimit = 5;
    
    static constexpr int numOscs = moduleLimit;
    static constexpr int numCombines = moduleLimit;
    static constexpr int numGains = moduleLimit;
    static constexpr int numADSRs = moduleLimit;
    static constexpr int numLFOs = moduleLimit;
    
    static constexpr int inputLimit = 10;
    
    
    juce::StringArray universalInputOptions; // Stringarray, in dem die Namen der Module gesammelt werden
    
private:
    juce::AudioProcessorValueTreeState apvts; // APVTS (Controller mit allen Parametern)
    
    
    
    int sPerBlock; //samplesPerBlock
    
    std::unique_ptr<juce::AudioProcessorEditor> editor;
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params; // Parameter für APVTS
    
    juce::Synthesiser synth; // Synthesizer von JUCE (managed die Stimmen automatisch, man muss nur eine Stimme definieren (siehe SynthVoice.h/cpp & SynthSound.h)
    
    juce::dsp::ProcessSpec spec; // Speichert Sachen wie Samplerate
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthPONCAudioProcessor)
};
