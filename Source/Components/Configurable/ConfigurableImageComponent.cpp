#include "ConfigurableImageComponent.h"

ConfigurableImageComponent::ConfigurableImageComponent(
        juce::String componentKey,
        int assetIndex,
        juce::RectanglePlacement placement) :
ConfigurableComponent(componentKey),
DrawableImageComponent(placement),
assetIndex(assetIndex) { }

/**
 * Load a new image from a different asset file.
 */
void ConfigurableImageComponent::setImageAssetIndex(int index)
{
    if (index >= 0 && assetIndex != index)
    {
        assetIndex = index;
    }
}

/**
 * (re)load the image file and set the colors.
 */
void ConfigurableImageComponent::applyConfigAssets(juce::StringArray assetNames,
        juce::Array<juce::Colour> colours)
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
