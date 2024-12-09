/*
  ==============================================================================

    LFOGUI.h
    Created: 10 Oct 2024 11:52:32am
    Author:  Maximilian Aebersold

  ==============================================================================
*/

#pragma once

#include "ModuleGUI.h"

class LFOGUI : public ModuleGUI, public juce::Component
{
public:
    LFOGUI(juce::String moduleId, juce::AudioProcessorValueTreeState& p, juce::StringArray inputOptions);
    
    void repaintTrigger() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    
    juce::Component* getComponent() override;
private:
    juce::String moduleId;
    juce::AudioProcessorValueTreeState& tree;
    
    juce::ComboBox waveTypeMenu;
    juce::Slider rateSlider;
    juce::Slider topRateSlider;
    juce::Slider bottomRateSlider;
    juce::ToggleButton useBPMButton;
    
public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveChoiceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> topRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bottomRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> useBPMAttachment;
};
