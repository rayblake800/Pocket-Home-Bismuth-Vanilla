/*
  ==============================================================================

    ConfigurableImageComponent.cpp
    Created: 17 Jan 2018 8:05:30pm
    Author:  anthony

  ==============================================================================
 */

#include "ConfigurableImageComponent.h"

const Array<Colour> ConfigurableImageComponent::defaultColours =
        ConfigurableImageComponent::loadDefaultColours();

ConfigurableImageComponent::ConfigurableImageComponent
(String componentKey, int assetIndex, RectanglePlacement placement) :
ConfigurableComponent(componentKey),
DrawableImageComponent(placement),
assetIndex(assetIndex)
{
    loadAllConfigProperties();
}

ConfigurableImageComponent::~ConfigurableImageComponent()
{
}

/**
 * Load a new image from a different asset file
 */
int ConfigurableImageComponent::setImageAssetIndex(int index)
{
    if (index >= 0 && assetIndex != index)
    {
        assetIndex = index;
        loadAllConfigProperties();
    }
}

/**
 * (re)load the image file and set the colors.
 */
void ConfigurableImageComponent::applyConfigAssets(Array<String> assetNames,
        Array<Colour> colours)
{
    if (assetIndex >= 0 && assetIndex < assetNames.size())
    {
        DBG(String("applyConfigAssets:Loading image ") + assetNames[assetIndex]);
        setImage(assetNames[assetIndex]);
    } 
    else
    {
        DBG(String("applyConfigAssets: couldn't load asset ") 
        +String(assetIndex) +String(", it doesn't exist!"));
        assetIndex = -1;
    }

    for (int i = 0; i < colours.size() && i < defaultColours.size(); i++)
    {
       replaceColour(defaultColours[i], colours[i]);
    }
    applyConfigBounds();

}