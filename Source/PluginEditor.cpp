/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthPONCAudioProcessorEditor::SynthPONCAudioProcessorEditor (SynthPONCAudioProcessor& p, juce::AudioProcessorValueTreeState& t)
: AudioProcessorEditor (&p), masterGui(p.universalInputOptions, t), audioProcessor (p), tree(t)
{
    setSize (1600, 900);
    
    moduleTypeBox.addItem("Oscillator", 1);
    moduleTypeBox.addItem("Combine", 2);
    moduleTypeBox.addItem("Gain", 3);
    moduleTypeBox.addItem("ADSR", 4);
    moduleTypeBox.addItem("LFO", 5);

    addAndMakeVisible(&moduleTypeBox);
    
    addModuleButton.setButtonText("+");
    addModuleButton.addListener(this);
    addAndMakeVisible(&addModuleButton);
    
    removeInactiveModulesButton.setButtonText("-");
    removeInactiveModulesButton.addListener(this);
    addAndMakeVisible(&removeInactiveModulesButton);
    
    for (int i = 0; i < numPages; i++)
    {
        pageNavigationButtons[i] = std::make_unique<juce::TextButton>();
        pageNavigationButtons[i]->setButtonText("Page " + std::to_string(i + 1));
        pageNavigationButtons[i]->addListener(this);
        addAndMakeVisible(*pageNavigationButtons[i]);
    }
    
    // erstellt soviele GUIs der Module, wie im processor angegeben
    for (int i = 0; i < audioProcessor.numOscs; i++)
    {
        oscGuis.push_back(std::make_unique<OscillatorGUI>(std::to_string(i), tree, audioProcessor.universalInputOptions));
    }
    
    for (int i = 0; i < audioProcessor.numCombines; i++)
    {
        combineGuis.push_back(std::make_unique<CombineGUI>(std::to_string(i), tree, audioProcessor.universalInputOptions));
    }
    
    for (int i = 0; i < audioProcessor.numGains; i++)
    {
        gainGuis.push_back(std::make_unique<GainGUI>(std::to_string(i), tree, audioProcessor.universalInputOptions));
    }
    
    for (int i = 0; i < audioProcessor.numADSRs; i++)
    {
        adsrGuis.push_back(std::make_unique<ADSRGUI>(std::to_string(i), tree, audioProcessor.universalInputOptions));
    }
    
    for (int i = 0; i < audioProcessor.numLFOs; i++)
    {
        lfoGuis.push_back(std::make_unique<LFOGUI>(std::to_string(i), tree, audioProcessor.universalInputOptions));
    }
    
    addAndMakeVisible(*masterGui.getComponent());
    refreshUI();
}

SynthPONCAudioProcessorEditor::~SynthPONCAudioProcessorEditor()
{
    
}

//==============================================================================
void SynthPONCAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void SynthPONCAudioProcessorEditor::resized()
{
    // Platzieren etc.
    int x = 100;
    int y = 50;
    
    moduleTypeBox.setBounds(0, 0, x, y);
    addModuleButton.setBounds(2 * x, 0, x, y);
    removeInactiveModulesButton.setBounds(1 * x, 0, x, y);
    
    masterGui.setBounds(getWidth() - 100, getHeight() - 100, 100, 100);
}

void SynthPONCAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    // Aktionen bei Knopfdruck
    if (button == &addModuleButton)
    {
        audioProcessor.addModuleButtonClicked(moduleTypeBox.getSelectedId());
    }
    if (button == &removeInactiveModulesButton)
    {
        clearPage();
        refreshUI();
    }
    
    for (int i = 0; i < numPages; i++)
    {
        if (button == &*pageNavigationButtons[i])
        {
            clearPage();
            currentPage = i + 1;
            refreshUI();
        }
    }
}


void SynthPONCAudioProcessorEditor::refreshUI()
{
    // refresht UI mit verschiedenen Modulflächen
    switch (currentPage)
    {
        case 1:
            showPage(oscGuis, "ISOSCACTIVE_");
            break;
        case 2:
            showPage(combineGuis, "ISCOMBINEACTIVE_");
            break;
        case 3:
            showPage(gainGuis, "ISGAINACTIVE_");
            break;
        case 4:
            showPage(adsrGuis, "ISADSRACTIVE_");
            break;
        case 5:
            showPage(lfoGuis, "ISLFOACTIVE_");
            break;
            
        default:
            clearPage();
            break;
    }
    
    int padding = 10;
    int totalPadding = ((numPages - 1) * padding);
    int availableWidth = getWidth() - totalPadding - 110;
    int pnbWidth = availableWidth / numPages;
    
    
    for (int i = 0; i < 10; i++)
    {
        pageNavigationButtons[i]->setBounds(i * (pnbWidth + padding), getHeight() - 30, pnbWidth, 20);
    }
}

void SynthPONCAudioProcessorEditor::clearPage()
{
    // macht alle module unsichtbar
    for (int i = 0; i < oscGuis.size(); i++)
    {
        oscGuis[i]->getComponent()->setVisible(false);
    }
    for (int i = 0; i < combineGuis.size(); i++)
    {
        combineGuis[i]->getComponent()->setVisible(false);
    }
    for (int i = 0; i < gainGuis.size(); i++)
    {
        gainGuis[i]->getComponent()->setVisible(false);
    }
    for (int i = 0; i < adsrGuis.size(); i++)
    {
        adsrGuis[i]->getComponent()->setVisible(false);
    }
    for (int i = 0; i < lfoGuis.size(); i++)
    {
        lfoGuis[i]->getComponent()->setVisible(false);
    }
}

void SynthPONCAudioProcessorEditor::showPage(std::vector<std::unique_ptr<ModuleGUI>>& compsToShow, juce::String identifier)
{
    // macht module mittels einem identifier sichtbar und platziert diese
    int compCount = 0;
    int xPos = 0;
    int yPos = 100;
    const int editorWidth = getWidth();

    for (int i = 0; i < compsToShow.size(); i++)
    {
        auto& isActive = *tree.getRawParameterValue(identifier + std::to_string(i));
        if (isActive.load())
        {
            int moduleWidth = compsToShow[i]->getModuleWidth();
            int moduleHeight = compsToShow[i]->getModuleHeight();

            if (xPos + moduleWidth > editorWidth)
            {
                xPos = 0;
                yPos += moduleHeight;
            }

            compsToShow[i]->getComponent()->setBounds(xPos, yPos, moduleWidth, moduleHeight);
            
            addAndMakeVisible(*compsToShow[i]->getComponent());
                        
            xPos += moduleWidth;
            compCount++;
        }
    }
}
