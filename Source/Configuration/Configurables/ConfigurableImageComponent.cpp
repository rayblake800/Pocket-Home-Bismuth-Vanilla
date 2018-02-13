#include "ConfigurableImageComponent.h"

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
 * reload all image files
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
    for(int i=0;i<colours.size();i++){
        int colourId = imageColour0Id+i;
        setColour(colourId,colours[i]);
    }
    applyConfigBounds();

}