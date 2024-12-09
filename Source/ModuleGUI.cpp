/*
  ==============================================================================

    ModuleGUI.cpp
    Created: 24 Aug 2024 9:55:34am
    Author:  Maximilian

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ModuleGUI.h"

//==============================================================================
ModuleGUI::ModuleGUI(juce::String identifier, juce::AudioProcessorValueTreeState& tree, juce::String ID) :  identifier(identifier), tree(tree), ID(ID)
{
    for (int i = 0; i < 10; i++)
    {
        inputBoxList.push_back(std::make_unique<juce::ComboBox>());
    }
    deactivateModuleButton.setButtonText("-");
    deactivateModuleButton.addListener(this);
}

ModuleGUI::~ModuleGUI()
{
    
}

void ModuleGUI::buttonClicked(juce::Button* button)
{
    if (button == &deactivateModuleButton)
    {
        if (*tree.getRawParameterValue("IS" + identifier + "ACTIVE_" + ID))
        {
            tree.getParameter("IS" + identifier + "ACTIVE_" + ID)->setValueNotifyingHost(false);
            isActive = false;
        }
        else
        {
            tree.getParameter("IS" + identifier + "ACTIVE_" + ID)->setValueNotifyingHost(true);
            isActive = true;
        }
        repaintTrigger();
    }
    
}

int ModuleGUI::getModuleWidth()
{
    return moduleWidth;
}

int ModuleGUI::getModuleHeight()
{
    return moduleHeight;
}
void ModuleGUI::repaintModuleFrame(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    if (isActive)
    {
        active(g, bounds);
    }
    else
    {
        inactive(g, bounds);
    }
}

void ModuleGUI::active(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    g.setColour(juce::Colours::lime);
    g.drawRect(bounds);
}
void ModuleGUI::inactive(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    g.setColour(juce::Colours::red);
    g.drawRect(bounds);
}

void ModuleGUI::setModuleSize(int x, int y)
{
    moduleWidth = x;
    moduleHeight = y;
}

void ModuleGUI::setInputLayout(int numInputs, juce::StringArray inputList, juce::String moduleID, juce::AudioProcessorValueTreeState& tree)
{
    numInputs = std::min(numInputs, static_cast<int>(inputBoxList.size()));
    
    inputAttachments.clear();

    for (int i = 0; i < inputBoxList.size(); i++)
    {
        inputBoxList[i]->clear(); // Clear any previous items
        inputBoxList[i]->addItemList(inputList, 1); // Add the input list to the ComboBox
        
        if (i < numInputs)
        {
            // Create ComboBoxAttachment only for valid inputs (within numInputs)
            inputAttachments.push_back(
                std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
                    tree, moduleID + "_" + std::to_string(i), *inputBoxList[i]
                )
            );
        }
    }
    this->numInputs = numInputs;
}

juce::Component* ModuleGUI::getComponent()
{
    return nullptr;
}
