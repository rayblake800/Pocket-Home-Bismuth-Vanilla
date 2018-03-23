#include "ConfigurableImageButton.h"

ConfigurableImageButton::ConfigurableImageButton
(String componentKey, ComponentConfigFile& config, int assetIndex, 
        RectanglePlacement placement) :
buttonImage(componentKey, config, assetIndex, placement),
ConfigurableComponent(componentKey, config),
Button(componentKey + "Button")
{
    addAndMakeVisible(buttonImage);
}

/**
 * Load a new image from a different asset file
 */
void ConfigurableImageButton::setImageAssetIndex(int index)
{
    buttonImage.setImageAssetIndex(index);
}

/**
 * Keeps the image set to component bounds.
 */
void ConfigurableImageButton::resized()
{
    buttonImage.setBounds(getLocalBounds());
}


