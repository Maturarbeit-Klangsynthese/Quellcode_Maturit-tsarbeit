/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OscillatorGUI.h"
#include "CombineGUI.h"
#include "GainGUI.h"
#include "MasterGUI.h"
#include "ADSRGUI.h"
#include "LFOGUI.h"

//==============================================================================
/**
*/
class SynthPONCAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    SynthPONCAudioProcessorEditor (SynthPONCAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~SynthPONCAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    
    void refreshUI();
    void clearPage();
    void removeModule(Module* moduleToDelete);
    void showPage(std::vector<std::unique_ptr<ModuleGUI>>& compsToShow, juce::String identifier);
    
    //==============================================================================
    
    bool checkSameName (juce::String moduleNameToCheck);
    
    //==============================================================================

    juce::ComboBox masterInputMenu;
    
    
private:

    
    int currentPage = 1;
    static constexpr int numPages = 10;
    
    
    // Graphische Objekte der Module
    MasterGUI masterGui;
    std::vector<std::unique_ptr<ModuleGUI>> oscGuis;
    std::vector<std::unique_ptr<ModuleGUI>> combineGuis;
    std::vector<std::unique_ptr<ModuleGUI>> gainGuis;
    std::vector<std::unique_ptr<ModuleGUI>> adsrGuis;
    std::vector<std::unique_ptr<ModuleGUI>> lfoGuis;

    
    // Knöpfe für den Rahmen sprich Navigation und Bearbeiten
    std::array<std::unique_ptr<juce::TextButton>, numPages> pageNavigationButtons;
    juce::ComboBox moduleTypeBox;
    juce::TextButton addModuleButton;
    juce::TextButton removeInactiveModulesButton;


    SynthPONCAudioProcessor& audioProcessor; // Referenz auf den Prozessor
    juce::AudioProcessorValueTreeState& tree; // Referenz auf den APVTS

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthPONCAudioProcessorEditor)
};
