/*
  ==============================================================================

    CombineGUI.h
    Created: 24 Aug 2024 9:56:33am
    Author:  Maximilian

  ==============================================================================
*/

#pragma once
#include "ModuleGUI.h"

class CombineGUI : public ModuleGUI, public juce::Component
{
public:
    CombineGUI(juce::String moduleId, juce::AudioProcessorValueTreeState& p, juce::StringArray inputOptions);
    
    void repaintTrigger() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    
    juce::Component* getComponent() override;
    
private:
    bool prepared = false;
    
    juce::Slider mixSlider;
    
    juce::String moduleId;
    juce::AudioProcessorValueTreeState& tree;
    
public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;


};
