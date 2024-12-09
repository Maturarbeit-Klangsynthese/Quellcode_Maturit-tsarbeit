/*
  ==============================================================================

    OscillatorGUI.h
    Created: 24 Aug 2024 9:56:18am
    Author:  Maximilian

  ==============================================================================
*/

#pragma once
#include "ModuleGUI.h"

class OscillatorGUI : public ModuleGUI, public juce::Component
{
public:
    OscillatorGUI(juce::String moduleId, juce::AudioProcessorValueTreeState& p, juce::StringArray inputOptions);
    
    void repaintTrigger() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    
    juce::Component* getComponent() override;
private:
    juce::String moduleId;
    juce::AudioProcessorValueTreeState& tree;
    
    juce::ComboBox waveTypeMenu;
    juce::Slider transposeSlider;
    
public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveChoiceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> transposeAttachment;

};
