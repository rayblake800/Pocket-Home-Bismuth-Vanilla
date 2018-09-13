#include "ConfigurableImageComponent.h"

ConfigurableImageComponent::ConfigurableImageComponent(
        const juce::Identifier& componentKey,
        const int assetIndex,
        const juce::RectanglePlacement placement) :
ConfigurableComponent(componentKey),
DrawableImageComponent(placement),
assetIndex(assetIndex) 
{
    applyConfigSettings();
}

/*
 * Loads a new image from a different asset file.
 */
void ConfigurableImageComponent::setImageAssetIndex(const int index)
{
    if (index >= 0 && assetIndex != index)
    {
        assetIndex = index;
    }
}

/**
 * Reloads the image file and sets the image colors.
 */
void ConfigurableImageComponent::applyConfigAssets(
        const juce::StringArray& assetNames,
        const juce::Array<juce::Colour>& colours)
{
    if (assetIndex >= 0 && assetIndex < assetNames.size())
    {
        setImage(assetNames[assetIndex]);
    }
    else
    {
        DBG("ConfigurableImageComponent::" << __func__
                << ": couldn't load asset index " << assetIndex
                << ", asset count is " << assetNames.size());
        assetIndex = -1;
    }
    for (int i = 0; i < colours.size(); i++)
    {
        int colourId = imageColour0Id + i;
        setColour(colourId, colours[i]);
    }
    applyConfigBounds();
}
