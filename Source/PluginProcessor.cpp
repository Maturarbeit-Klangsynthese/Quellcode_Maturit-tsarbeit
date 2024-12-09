/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthPONCAudioProcessor::SynthPONCAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, juce::Identifier("Parameters"), createParams())
#endif
{
    synth.addSound(new SynthSound());
    for (int i = 0; i < 5; i++) // Maximal 5 Stimmen werden gespielt
    {
        //SynthVoice ist eine abgeleitete Klasse, die der Synth von JUCE managen kann
        synth.addVoice(new SynthVoice(apvts, numOscs, numCombines, numGains, numADSRs, numLFOs));
    }
}

SynthPONCAudioProcessor::~SynthPONCAudioProcessor()
{
}

//==============================================================================
const juce::String SynthPONCAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthPONCAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthPONCAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthPONCAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthPONCAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthPONCAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthPONCAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthPONCAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthPONCAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthPONCAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthPONCAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    // Samplerate weitergeben
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}
//
void SynthPONCAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthPONCAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthPONCAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Hier leert man den Outputbuffer und gibt ihn dem Synth zum füllen
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
}

//==============================================================================
bool SynthPONCAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthPONCAudioProcessor::createEditor()
{
    return new SynthPONCAudioProcessorEditor (*this, apvts);
}

//==============================================================================
void SynthPONCAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    //XML File erstellen für die Parameterwerte im apvts
    std::unique_ptr<juce::XmlElement> xml (apvts.copyState().createXml());

    if (xml != nullptr)
    {
        copyXmlToBinary(*xml, destData);
    }
}

void SynthPONCAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //Falls ein XML existiert, soll apvts den Zustand übernehmen
    
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr && xml->hasTagName(apvts.state.getType()))
    {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthPONCAudioProcessor();
}

//==============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout SynthPONCAudioProcessor::createParams()
{
    // Erstellen von Parametern für den APVTS (wird bei der Initialisierung des APVTS aufgerufen)
    universalInputOptions.add("None");
    
    for (int i = 0; i < numOscs; i++)
    {
        createOscParams(i);
    }
     
    for (int i = 0; i < numCombines; i++)
    {
        createCombineParams(i);
    }
    
    for (int i = 0; i < numGains; i++)
    {
        createGainParams(i);
    }
    
    for (int i = 0; i < numADSRs; i++)
    {
        createADSRParams(i);
    }
    
    for (int i = 0; i < numLFOs; i++)
    {
        createLFOParams(i);
    }
    
    createInputParams();
    
    createMasterParams();
    
    return { params.begin(), params.end()};
}

void SynthPONCAudioProcessor::createOscParams(int OscID)
{
    params.push_back(std::make_unique<juce::AudioParameterChoice> (juce::ParameterID("WAVECHOICE_" + std::to_string(OscID), 1), "Wave Choice " + std::to_string(OscID), juce::StringArray {"Sine", "Saw", "Square" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt> (juce::ParameterID("TRANSPOSE_" + std::to_string(OscID), 1), "Transpose " + std::to_string(OscID), -48, 48, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterBool> (juce::ParameterID("ISOSCACTIVE_" + std::to_string(OscID), 1), "Is Oscillator Active " + std::to_string(OscID), false));
    universalInputOptions.add("Oscillator " + std::to_string(OscID));
}

void SynthPONCAudioProcessor::createCombineParams(int CombineID)
{
    params.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("MIX_" + std::to_string(CombineID), 1), "Mix " + std::to_string(CombineID), juce::NormalisableRange<float>{0.0f, 100.0f}, 50.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool> (juce::ParameterID("ISCOMBINEACTIVE_" + std::to_string(CombineID), 1), "Is Combine Active " + std::to_string(CombineID), false));
    universalInputOptions.add("Combine " + std::to_string(CombineID));
}

void SynthPONCAudioProcessor::createGainParams(int GainID)
{
    params.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("GAINVALUE_" + std::to_string(GainID), 1), "Gain " + std::to_string(GainID), juce::NormalisableRange<float>{-48.0f, 48.0f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool> (juce::ParameterID("DBMODE_" + std::to_string(GainID), 1), "dB Mode " + std::to_string(GainID), true));
    params.push_back(std::make_unique<juce::AudioParameterBool> (juce::ParameterID("ISGAINACTIVE_" + std::to_string(GainID), 1), "Is Gain Active " + std::to_string(GainID), false));
    universalInputOptions.add("Gain " + std::to_string(GainID));
}

void SynthPONCAudioProcessor::createADSRParams(int ADSRID)
{
    params.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("ATTACKTIME_" + std::to_string(ADSRID), 1), "Attack Time " + std::to_string(ADSRID), juce::NormalisableRange<float>{0.0f, 10.0f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("DECAYTIME_" + std::to_string(ADSRID), 1), "Decay Time " + std::to_string(ADSRID), juce::NormalisableRange<float>{0.0f, 10.0f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("SUSTAINVALUE_" + std::to_string(ADSRID), 1), "Sustain Value " + std::to_string(ADSRID), juce::NormalisableRange<float>{0.0f, 1.0f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("RELEASETIME_" + std::to_string(ADSRID), 1), "Release Time " + std::to_string(ADSRID), juce::NormalisableRange<float>{0.0f, 10.0f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("MINVALUE_" + std::to_string(ADSRID), 1), "Min Value " + std::to_string(ADSRID), juce::NormalisableRange<float>{-1.0f, 1.0f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("MAXVALUE_" + std::to_string(ADSRID), 1), "Max Value " + std::to_string(ADSRID), juce::NormalisableRange<float>{-1.0f, 1.0f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool> (juce::ParameterID("ISADSRACTIVE_" + std::to_string(ADSRID), 1), "Is ADSR Active " + std::to_string(ADSRID), false));
    universalInputOptions.add("ADSR " + std::to_string(ADSRID));
}

void SynthPONCAudioProcessor::createLFOParams(int LFOID)
{
    params.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("RATE_" + std::to_string(LFOID), 1), "Rate " + std::to_string(LFOID), juce::NormalisableRange<float>{0.1f, 10.0f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterInt> (juce::ParameterID("BPMRATETOP_" + std::to_string(LFOID), 1), "BPM Rate Top " + std::to_string(LFOID), 1, 32, 1));
    params.push_back(std::make_unique<juce::AudioParameterInt> (juce::ParameterID("BPMRATEBOTTOM_" + std::to_string(LFOID), 1), "BPM Rate Bottom " + std::to_string(LFOID), 1, 32, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice> (juce::ParameterID("LFOWAVECHOICE_" + std::to_string(LFOID), 1), "LFO Wave Choice " + std::to_string(LFOID), juce::StringArray {"Sine", "Saw", "Square" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterBool> (juce::ParameterID("USEBPM_" + std::to_string(LFOID), 1), "Use BPM " + std::to_string(LFOID), false));
    params.push_back(std::make_unique<juce::AudioParameterBool> (juce::ParameterID("ISLFOACTIVE_" + std::to_string(LFOID), 1), "Is LFO Active " + std::to_string(LFOID), false));
    universalInputOptions.add("LFO " + std::to_string(LFOID));
}

void SynthPONCAudioProcessor::createMasterParams()
{
    // Der Master besitzt keine Regler
}

void SynthPONCAudioProcessor::createInputParams()
{
    for (int i = 0; i < inputLimit; i++)
    {
        for (int j = 0; j < numOscs; j++)
        {
            params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("OSC_" + std::to_string(j) + "_" + std::to_string(i), 1), "Oscillator: " + std::to_string(j) + ", Input: " + std::to_string(i), universalInputOptions, 0));
        }
        for (int j = 0; j < numCombines; j++)
        {
            params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("COMBINE_" + std::to_string(j) + "_" + std::to_string(i), 1), "Combine: " + std::to_string(j) + ", Input: " + std::to_string(i), universalInputOptions, 0));
        }
        for (int j = 0; j < numGains; j++)
        {
            params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("GAIN_" + std::to_string(j) + "_" + std::to_string(i), 1), "Gain: " + std::to_string(j) + ", Input: " + std::to_string(i), universalInputOptions, 0));
        }
        for (int j = 0; j < numADSRs; j++)
        {
            params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("ADSR_" + std::to_string(j) + "_" + std::to_string(i), 1), "Adsr: " + std::to_string(j) + ", Input: " + std::to_string(i), universalInputOptions, 0));
        }
        for (int j = 0; j < numLFOs; j++)
        {
            params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("LFO_" + std::to_string(j) + "_" + std::to_string(i), 1), "LFO: " + std::to_string(j) + ", Input: " + std::to_string(i), universalInputOptions, 0));
        }
        
        params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("MASTER_" + std::to_string(i), 1), "Master Input: " + std::to_string(i), universalInputOptions, 0));
    }
}

void SynthPONCAudioProcessor::addModuleButtonClicked (int moduleChoice) // Wenn "+" gedrückt wird (vom PluginEditor aufgerufen)
{
        switch (moduleChoice)
        {
            case 0:
                std::cout << "Choose a module Type first!" << std::endl;
                return;
                break;
                
            case 1:
                for (int i = 0; i < numOscs; i++)
                {
                    if (!*apvts.getRawParameterValue("ISOSCACTIVE_" + std::to_string(i)))
                    {
                        apvts.getParameter("ISOSCACTIVE_" + std::to_string(i))->setValueNotifyingHost(true);
                        addModule();
                        return;
                    }
                }
                break;
                
            case 2:
                for (int i = 0; i < numCombines; i++)
                {
                    if (!*apvts.getRawParameterValue("ISCOMBINEACTIVE_" + std::to_string(i)))
                    {
                        apvts.getParameter("ISCOMBINEACTIVE_" + std::to_string(i))->setValueNotifyingHost(true);
                        addModule();
                        return;
                    }
                }
                //oscillator.isActive = true
                break;
            case 3:

                for (int i = 0; i < numGains; i++)
                {
                    if (!*apvts.getRawParameterValue("ISGAINACTIVE_" + std::to_string(i)))
                    {
                        apvts.getParameter("ISGAINACTIVE_" + std::to_string(i))->setValueNotifyingHost(true);
                        addModule();
                        return;
                    }
                }
                break;
            case 4:
                
                for (int i = 0; i < numADSRs; i++)
                {
                    if (!*apvts.getRawParameterValue("ISADSRACTIVE_" + std::to_string(i)))
                    {
                        apvts.getParameter("ISADSRACTIVE_" + std::to_string(i))->setValueNotifyingHost(true);
                        addModule();
                        return;
                    }
                }
                break;
            case 5:
                
                for (int i = 0; i < numLFOs; i++)
                {
                    if (!*apvts.getRawParameterValue("ISLFOACTIVE_" + std::to_string(i)))
                    {
                        apvts.getParameter("ISLFOACTIVE_" + std::to_string(i))->setValueNotifyingHost(true);
                        addModule();
                        return;
                    }
                }
                break;
                
            default:
                std::cout << "Something went wrong!" << std::endl; return;
                break;
        }
}

void SynthPONCAudioProcessor::addModule() // Editor wird aufgefordert, das UI zu updaten
{
    if (auto* editor = dynamic_cast<SynthPONCAudioProcessorEditor*>(getActiveEditor()))
        {
            editor->refreshUI();
        }
}
