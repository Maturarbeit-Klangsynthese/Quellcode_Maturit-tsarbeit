/*
  ==============================================================================

    ModuleGUI.h
    Created: 24 Aug 2024 9:55:34am
    Author:  Maximilian

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/*
*/
class ModuleGUI : public juce::Button::Listener
{
public:
    ModuleGUI(juce::String identifier, juce::AudioProcessorValueTreeState& tree, juce::String ID);
    virtual ~ModuleGUI();
    void buttonClicked(juce::Button* button) override;
    
    int getModuleWidth();
    int getModuleHeight();
    
    void repaintModuleFrame(juce::Graphics& g, juce::Rectangle<int> bounds);
    void active(juce::Graphics& g, juce::Rectangle<int> bounds);
    void inactive(juce::Graphics& g, juce::Rectangle<int> bounds);
    virtual void repaintTrigger() = 0;
    
    virtual juce::Component* getComponent();
    
    void setModuleSize(int x, int y);
    void setInputLayout(int numInputs, juce::StringArray inputList, juce::String moduleID, juce::AudioProcessorValueTreeState& tree);
    
    int moduleTypeID = 0;
    
    int numInputs;
    
    std::vector<std::unique_ptr<juce::ComboBox>> inputBoxList;
    juce::TextButton deactivateModuleButton;

    juce::String identifier;
    bool isActive = true;

private:
    juce::AudioProcessorValueTreeState& tree;
    juce::String ID;
    int moduleWidth = 0;
    int moduleHeight = 0;
protected:
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>> inputAttachments;
};
