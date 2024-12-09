/*
  ==============================================================================

    CombineGUI.cpp
    Created: 24 Aug 2024 9:56:33am
    Author:  Maximilian

  ==============================================================================
*/

#include "CombineGUI.h"

CombineGUI::CombineGUI(juce::String moduleId, juce::AudioProcessorValueTreeState& p, juce::StringArray inputOptions) : ModuleGUI("COMBINE", p, moduleId), moduleId(moduleId), tree(p)
{
    moduleTypeID = 2;
    setModuleSize(200, 200);
    setInputLayout(2, inputOptions, "COMBINE_" + moduleId, tree);
    
    for (int i = 0; i < numInputs; i++)
    {
        addAndMakeVisible(*inputBoxList[i]);
    }
    
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mixSlider.setNumDecimalPlacesToDisplay(2);

    mixSlider.setRange(0.0f, 100.0f);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 15);
    
    addAndMakeVisible(mixSlider);
    addAndMakeVisible(deactivateModuleButton);

    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "MIX_" + moduleId, mixSlider);
    
    prepared = true;
    resized();
}

void CombineGUI::repaintTrigger()
{
    repaint();
}

void CombineGUI::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::red);
    repaintModuleFrame(g, getLocalBounds());

    g.setColour(juce::Colours::black);
    g.drawText("Combine " + moduleId, 0, 5, getWidth(), 10, juce::Justification::centred);
}

void CombineGUI::resized()
{
    deactivateModuleButton.setBounds(getWidth() - 20, 5, 15, 15);

    if (prepared == true)
    {
        for (int i = 0; i < numInputs && i < 10; i++)
        {
            inputBoxList[i]->setBounds(getWidth() / 2 - 80,  ((i * 2) + 1) * (getHeight() / (numInputs * 2)) - 10, 160, 20);
        }
    }
    mixSlider.setBounds(getWidth()/2 - 80, getModuleHeight()/2 - 40, 160, 80);
}

juce::Component* CombineGUI::getComponent()
{
    return this;
}
