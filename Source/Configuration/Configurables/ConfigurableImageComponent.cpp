#include "ConfigurableImageComponent.h"

ConfigurableImageComponent::ConfigurableImageComponent
(String componentKey, int assetIndex, RectanglePlacement placement) :
ConfigurableComponent(componentKey),
DrawableImageComponent(placement),
assetIndex(assetIndex)
{
    loadAllConfigProperties();
}

ConfigurableImageComponent::~ConfigurableImageComponent() { }

/**
 * Load a new image from a different asset file
 */
void ConfigurableImageComponent::setImageAssetIndex(int index)
{
    if (index >= 0 && assetIndex != index)
    {
        assetIndex = index;
        loadAllConfigProperties();
    }
}

/**
 * reload all image files
 */
void ConfigurableImageComponent::applyConfigAssets(StringArray assetNames,
        Array<Colour> colours)
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
