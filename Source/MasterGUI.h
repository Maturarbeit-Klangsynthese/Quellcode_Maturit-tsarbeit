/*
  ==============================================================================

    MasterGUI.h
    Created: 24 Aug 2024 9:56:03am
    Author:  Maximilian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleGUI.h"

class MasterGUI : public ModuleGUI, public juce::Component
{
public:
    MasterGUI(juce::StringArray inputOptions, juce::AudioProcessorValueTreeState& p);
    
    void repaintTrigger() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    
    juce::Component* getComponent() override;
private:
    bool prepared = false;
};
