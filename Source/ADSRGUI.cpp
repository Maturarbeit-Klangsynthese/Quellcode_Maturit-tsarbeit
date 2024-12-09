/*
  ==============================================================================

    ADSRGUI.cpp
    Created: 8 Oct 2024 6:24:07pm
    Author:  Maximilian Aebersold

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRGUI.h"

//==============================================================================
ADSRGUI::ADSRGUI(juce::String moduleId, juce::AudioProcessorValueTreeState& p, juce::StringArray inputOptions) : ModuleGUI("ADSR", p, moduleId), moduleId(moduleId), tree(p)
{
    moduleTypeID = 4;
    setModuleSize(600, 200);
    setInputLayout(0, inputOptions, "ADSR_" + moduleId, tree);
    
    for (int i = 0; i < numInputs; i++)
    {
        addAndMakeVisible(*inputBoxList[i]);
    }
    aSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "ATTACKTIME_" + moduleId, aSlider);
    dSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "DECAYTIME_" + moduleId, dSlider);
    sSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "SUSTAINVALUE_" + moduleId, sSlider);
    rSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "RELEASETIME_" + moduleId, rSlider);
    minSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "MINVALUE_" + moduleId, minSlider);
    maxSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "MAXVALUE_" + moduleId, maxSlider);

    aSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    aSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 10);
    aSlider.setRange(0.0f, 10.0f);
    aSlider.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(aSlider);
    
    dSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    dSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 10);
    dSlider.setRange(0.0f, 10.0f);
    dSlider.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(dSlider);
    
    sSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 10);
    sSlider.setRange(0.0f, 1.0f);
    sSlider.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(sSlider);
    
    rSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    rSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 10);
    rSlider.setRange(0.0f, 10.0f);
    rSlider.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(rSlider);
    
    minSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    minSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 10);
    minSlider.setRange(-1.0f, 1.0f);
    minSlider.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(minSlider);
    
    maxSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    maxSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 10);
    maxSlider.setRange(-1.0f, 1.0f);
    maxSlider.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(maxSlider);
    
    addAndMakeVisible(deactivateModuleButton);
    
    prepared = true;
    resized();
}

void ADSRGUI::repaintTrigger()
{
    repaint();
}

void ADSRGUI::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    repaintModuleFrame(g, getLocalBounds());

    g.setColour(juce::Colours::white);
    g.drawText("ADSR " + moduleId, 0, 5, getWidth(), 10, juce::Justification::centred);
}

void ADSRGUI::resized()
{
    deactivateModuleButton.setBounds(getWidth() - 20, 5, 15, 15);

    int padding = 5;
    int totalPadding = 7 * padding;
    int availableWidth = getWidth() - totalPadding;
    int sliderWidth = availableWidth / 6;
    int sliderHeight = getHeight() - 2 * padding;
    
    aSlider.setBounds(padding, 5, sliderWidth, sliderHeight);
    dSlider.setBounds(sliderWidth + 2 * padding, 5, sliderWidth, sliderHeight);
    sSlider.setBounds(2 * sliderWidth + 3 * padding, 5, sliderWidth, sliderHeight);
    rSlider.setBounds(3 * sliderWidth + 4 * padding, 5, sliderWidth, sliderHeight);
    minSlider.setBounds(4 * sliderWidth + 5 * padding, 5, sliderWidth, sliderHeight);
    maxSlider.setBounds(5 * sliderWidth + 6 * padding, 5, sliderWidth, sliderHeight);

}

juce::Component* ADSRGUI::getComponent()
{
    return this;
}
