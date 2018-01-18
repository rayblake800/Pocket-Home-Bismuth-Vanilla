/*
  ==============================================================================

    ConfigurableLabel.cpp
    Created: 17 Jan 2018 10:13:25pm
    Author:  anthony

  ==============================================================================
 */

#include "ConfigurableLabel.h"

ConfigurableLabel::ConfigurableLabel(String componentKey,
        const String &componentName,
        const String &labelText,
        const int& fontPadding) :
ScalingLabel(componentName, labelText, fontPadding),
ConfigurableComponent(componentKey)
{
    loadAllConfigProperties();
}

ConfigurableLabel::~ConfigurableLabel()
{}

void ConfigurableLabel::applyConfigAssets(Array<String> assetNames,
        Array<Colour> colours)
{
    std::vector<int> colourIds = 
    {
        Label::textColourId,
        Label::backgroundColourId,
        Label::outlineColourId
    };
    
    for(int i = 0; i<colours.size() && i < colourIds.size();i++){
        setColour(colourIds[i],colours[i]);
    }
}