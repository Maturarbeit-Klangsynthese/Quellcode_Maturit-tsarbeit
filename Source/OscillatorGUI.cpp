/*
  ==============================================================================

    OscillatorGUI.cpp
    Created: 24 Aug 2024 9:56:18am
    Author:  Maximilian

  ==============================================================================
*/

#include "OscillatorGUI.h"

OscillatorGUI::OscillatorGUI(juce::String moduleId, juce::AudioProcessorValueTreeState& p, juce::StringArray inputOptions) : ModuleGUI("OSC", p, moduleId), moduleId(moduleId), tree(p)
{
    
    
    moduleTypeID = 1;
    setModuleSize(400, 200);
    setInputLayout(1, inputOptions, "OSC_" + moduleId, tree);
    
    waveTypeMenu.addItem("Sine", 1);
    waveTypeMenu.addItem("Saw", 2);
    waveTypeMenu.addItem("Square", 3);
    
    transposeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    transposeSlider.setRange(-48, 48);
    transposeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 15);
    
    waveChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(tree, "WAVECHOICE_" + moduleId, waveTypeMenu);
    
    transposeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(tree, "TRANSPOSE_" + moduleId, transposeSlider);
    
    
    addAndMakeVisible(waveTypeMenu);
    addAndMakeVisible(transposeSlider);
    
    for (int i = 0; i < numInputs; i++)
    {
        addAndMakeVisible(*inputBoxList[i]);
    }
    addAndMakeVisible(deactivateModuleButton);

}

void OscillatorGUI::repaintTrigger()
{
    repaint();
}

void OscillatorGUI::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::blue);
    repaintModuleFrame(g, getLocalBounds());
    g.setColour(juce::Colours::black);
    g.drawText("Oscillator " + moduleId, 0, 5, getWidth(), 10, juce::Justification::centred);
}

void OscillatorGUI::resized()
{
    deactivateModuleButton.setBounds(getWidth() - 20, 5, 15, 15);

    waveTypeMenu.setBounds(10, getModuleHeight()/2 - 15, 80, 30);
    transposeSlider.setBounds(110, getModuleHeight()/4 - 40, 80, 80);
    inputBoxList[0]->setBounds(110, getModuleHeight() * 3 / 4 - 15, 80, 30);
}

juce::Component* OscillatorGUI::getComponent()
{
    return this;
}
