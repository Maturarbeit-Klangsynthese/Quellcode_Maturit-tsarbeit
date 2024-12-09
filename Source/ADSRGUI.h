/*
  ==============================================================================

    ADSRGUI.h
    Created: 8 Oct 2024 6:24:07pm
    Author:  Maximilian Aebersold

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleGUI.h"

//==============================================================================
/*
*/
class ADSRGUI  : public ModuleGUI, public juce::Component
{
public:
    ADSRGUI(juce::String moduleId, juce::AudioProcessorValueTreeState& p, juce::StringArray inputOptions);
    
    juce::Component* getComponent() override;
    
    void repaintTrigger() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider aSlider;
    juce::Slider dSlider;
    juce::Slider sSlider;
    juce::Slider rSlider;
    juce::Slider minSlider;
    juce::Slider maxSlider;

    bool prepared = false;
    
    juce::String moduleId;
    juce::AudioProcessorValueTreeState& tree;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRGUI)
public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> aSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> minSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> maxSliderAttachment;
};
