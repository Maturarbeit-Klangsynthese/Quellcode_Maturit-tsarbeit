/*
  ==============================================================================

    GainGUI.h
    Created: 1 Oct 2024 4:46:59pm
    Author:  Maximilian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleGUI.h"
//==============================================================================
/*
*/
class GainGUI  : public ModuleGUI, public juce::Component
{
public:
    GainGUI(juce::String moduleId, juce::AudioProcessorValueTreeState& p, juce::StringArray inputOptions);
    
    juce::Component* getComponent() override;
    
    void repaintTrigger() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider gainSlider;
    juce::ToggleButton gainModeButton;
    
    bool prepared = false;
    
    juce::String moduleId;
    juce::AudioProcessorValueTreeState& tree;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainGUI)
public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> gainModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
};
