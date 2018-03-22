#include "ConfigurableImageButton.h"

ConfigurableImageButton::ConfigurableImageButton
(ComponentConfigFile& config, String componentKey, int assetIndex, 
        RectanglePlacement placement) :
buttonImage(config, componentKey, assetIndex, placement),
ConfigurableComponent(config, componentKey),
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


