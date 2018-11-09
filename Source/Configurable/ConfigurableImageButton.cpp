#include "ConfigurableImageButton.h"

ConfigurableImageButton::ConfigurableImageButton(
        const juce::Identifier& componentKey,
        const int assetIndex,
        const juce::RectanglePlacement placement) :
buttonImage(componentKey, assetIndex, placement),
ConfigurableComponent(componentKey),
Button(componentKey + "Button")
{
    addAndMakeVisible(buttonImage);
}

/*
 * Loads a new image from a different asset file.
 */
void ConfigurableImageButton::setImageAssetIndex(const int index)
{
    buttonImage.setImageAssetIndex(index);
}

/*
 * Keeps the image set to component bounds.
 */
void ConfigurableImageButton::resized()
{
    buttonImage.setBounds(getLocalBounds());
}


