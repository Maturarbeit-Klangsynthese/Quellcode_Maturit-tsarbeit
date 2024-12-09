/*
  ==============================================================================

    GainGUI.cpp
    Created: 1 Oct 2024 4:46:59pm
    Author:  Maximilian

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GainGUI.h"

//==============================================================================
GainGUI::GainGUI(juce::String moduleId, juce::AudioProcessorValueTreeState& p, juce::StringArray inputOptions) : ModuleGUI("GAIN", p, moduleId), moduleId(moduleId), tree(p)
{
    moduleTypeID = 3;
    setModuleSize(200, 200);
    setInputLayout(2, inputOptions, "GAIN_" + moduleId, tree);
    
    for (int i = 0; i < numInputs; i++)
    {
        addAndMakeVisible(*inputBoxList[i]);
    }
    
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "GAINVALUE_" + moduleId, gainSlider);
    gainModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(tree, "DBMODE_" + moduleId, gainModeButton);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 10);
    gainSlider.setRange(-48.0f, 48.0f);
    gainSlider.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(gainSlider);
    
    gainModeButton.setButtonText("use dB");
    addAndMakeVisible(gainModeButton);
    addAndMakeVisible(deactivateModuleButton);

    
    prepared = true;
    resized();
}

void GainGUI::repaintTrigger()
{
    repaint();
}

void GainGUI::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgreen);
    repaintModuleFrame(g, getLocalBounds());

    g.setColour(juce::Colours::white);
    g.drawText("Gain " + moduleId, 0, 5, getWidth(), 10, juce::Justification::centred);
}

void GainGUI::resized()
{
    deactivateModuleButton.setBounds(getWidth() - 20, 5, 15, 15);

    if (prepared == true)
    {
        for (int i = 0; i < numInputs && i < 10; i++)
        {
            inputBoxList[i]->setBounds(getWidth() / 2 - 80, 70 + i * 40, 160, 20);
        }
    }
    gainSlider.setBounds(getWidth() / 2 - 40, 10, 80, 50);
    gainModeButton.setBounds(getWidth() / 2 - 40, getHeight() - 30, 80, 30);
}

juce::Component* GainGUI::getComponent()
{
    return this;
}
