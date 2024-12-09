/*
  ==============================================================================

    Module.cpp
    Created: 21 Aug 2024 7:50:59am
    Author:  Maximilian

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Module.h"

//==============================================================================
Module::Module()
{
    
}

Module::~Module()
{
}

void Module::update (float bpm)
{
    beatsPerMinute = bpm;
}
