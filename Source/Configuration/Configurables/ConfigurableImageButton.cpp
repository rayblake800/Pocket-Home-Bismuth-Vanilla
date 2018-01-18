/*
  ==============================================================================

    ConfigurableImageButton.cpp
    Created: 18 Jan 2018 11:18:18am
    Author:  anthony

  ==============================================================================
 */

#include "ConfigurableImageButton.h"

ConfigurableImageButton::ConfigurableImageButton
(String componentKey, int assetIndex, RectanglePlacement placement) :
Button(String("Button:")+componentKey),
image(componentKey, assetIndex, placement)
{
    addAndMakeVisible(image);
}

ConfigurableImageButton::~ConfigurableImageButton()
{
}

void ConfigurableImageButton::applyConfigBounds()
{
    image.applyConfigBounds();
    Rectangle<int> bounds = image.getBounds();
    setBounds(bounds);
    image.setBounds(bounds.withPosition(0, 0));
}

void ConfigurableImageButton::paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
}
