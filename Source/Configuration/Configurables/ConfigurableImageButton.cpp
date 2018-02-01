#include "ConfigurableImageButton.h"

ConfigurableImageButton::ConfigurableImageButton
(String componentKey, int assetIndex, RectanglePlacement placement) :
ConfigurableComponent(componentKey),
DrawableImageButton("", placement)
{
    imageComponent = new ConfigurableImageComponent
            (componentKey, assetIndex, placement);
    addAndMakeVisible(imageComponent);
}

ConfigurableImageButton::~ConfigurableImageButton()
{
}

/**
 *Adjust bounds based on config settings
 */
void ConfigurableImageButton::applyConfigBounds()
{
    ConfigurableImageComponent * image =
            dynamic_cast<ConfigurableImageComponent *> (imageComponent.get());
    if (image != nullptr)
    {
        image->applyConfigBounds();
        Rectangle<int> bounds = image->getBounds();
        setBounds(bounds);
        image->setBounds(bounds.withPosition(0, 0));
    }
}

