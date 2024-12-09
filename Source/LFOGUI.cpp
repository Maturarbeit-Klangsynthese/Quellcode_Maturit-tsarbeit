/*
  ==============================================================================

    LFOGUI.cpp
    Created: 10 Oct 2024 11:52:32am
    Author:  Maximilian Aebersold

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOGUI.h"

LFOGUI::LFOGUI(juce::String moduleId, juce::AudioProcessorValueTreeState& p, juce::StringArray inputOptions) : ModuleGUI("LFO", p, moduleId), moduleId(moduleId), tree(p)
{
    
    
    moduleTypeID = 1;
    setModuleSize(400, 200);
    setInputLayout(1, inputOptions, "LFO_" + moduleId, tree);
    
    waveTypeMenu.addItem("Sine", 1);
    waveTypeMenu.addItem("Saw", 2);
    waveTypeMenu.addItem("Square", 3);
    
    rateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    rateSlider.setRange(0.1f, 10.0f);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 15);
    
    topRateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    topRateSlider.setRange(1, 32);
    topRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 15);
    
    bottomRateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    bottomRateSlider.setRange(1, 32);
    bottomRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 15);
    
    useBPMButton.setButtonText("Use BPM");
    
    waveChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(tree, "LFOWAVECHOICE_" + moduleId, waveTypeMenu);

    useBPMAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(tree, "USEBPM_" + moduleId, useBPMButton);
    
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "RATE_" + moduleId, rateSlider);
    topRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "BPMRATETOP_" + moduleId, topRateSlider);
    bottomRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "BPMRATEBOTTOM_" + moduleId, bottomRateSlider);

    
    addAndMakeVisible(waveTypeMenu);
    addAndMakeVisible(rateSlider);
    addAndMakeVisible(topRateSlider);
    addAndMakeVisible(bottomRateSlider);
    addAndMakeVisible(useBPMButton);
    
    for (int i = 0; i < numInputs; i++)
    {
        addAndMakeVisible(*inputBoxList[i]);
    }
    
    addAndMakeVisible(deactivateModuleButton);
    
}

void LFOGUI::repaintTrigger()
{
    repaint();
}

void LFOGUI::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::gold);
    repaintModuleFrame(g, getLocalBounds());

    g.setColour(juce::Colours::black);
    g.drawText("LFO " + moduleId, 0, 5, getWidth(), 10, juce::Justification::centred);
}

void LFOGUI::resized()
{
    deactivateModuleButton.setBounds(getWidth() - 20, 5, 15, 15);
    
    waveTypeMenu.setBounds(10, getModuleHeight()/2 - 15, 80, 30);
    rateSlider.setBounds(110, getModuleHeight()/4 - 40, 80, 80);
    topRateSlider.setBounds(getWidth() - 90, getModuleHeight()/4 - 40, 80, 80);
    bottomRateSlider.setBounds(getWidth() - 90, (3 * getModuleHeight()/4) - 40, 80, 80);
    useBPMButton.setBounds(getWidth() - 180, getModuleHeight()/2 - 20, 80, 40);

    inputBoxList[0]->setBounds(110, getModuleHeight() * 3 / 4 - 15, 80, 30);
}

juce::Component* LFOGUI::getComponent()
{
    return this;
}
