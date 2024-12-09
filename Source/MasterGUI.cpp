/*
  ==============================================================================

    MasterGUI.cpp
    Created: 24 Aug 2024 9:56:03am
    Author:  Maximilian

  ==============================================================================
*/

#include "MasterGUI.h"

MasterGUI::MasterGUI(juce::StringArray inputOptions, juce::AudioProcessorValueTreeState& p) : ModuleGUI("NONE", p, "M")
{
    setModuleSize(100, 100);
    setInputLayout(1, inputOptions, "MASTER", p);
    
    for (int i = 0; i < numInputs; i++)
    {
        addAndMakeVisible(*inputBoxList[i]);
    }
    addAndMakeVisible(deactivateModuleButton);
    prepared = true;
    resized();
}
void MasterGUI::repaintTrigger()
{
    repaint();
}

void MasterGUI::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::wheat);
    
    g.setColour(juce::Colours::black);
    g.drawText("Master", 0, 5, getWidth(), 10, juce::Justification::centred);
}
void MasterGUI::resized()
{
    if (prepared == true)
    {
        for (int i = 0; i < numInputs && i < 10; i++)
        {
            inputBoxList[i]->setBounds(getWidth() / 2 - 40,  ((i * 2) + 1) * (getHeight() / (numInputs * 2)) - 10, 80, 20);
        }
    }
}

juce::Component* MasterGUI::getComponent()
{
    return this;
}
