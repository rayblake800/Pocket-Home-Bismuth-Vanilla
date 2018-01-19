/*
  ==============================================================================

    DrawableImageComponent.cpp
    Created: 17 Jan 2018 3:38:35pm
    Author:  anthony

  ==============================================================================
 */

#include "../Utils.h"
#include "DrawableImageComponent.h"

/**
 * Create a DrawableImageComponent using an asset file.
 */
DrawableImageComponent::DrawableImageComponent
(String assetFilename, RectanglePlacement placement) :
placement(placement)
{
    setImage(assetFilename);
    setInterceptsMouseClicks(false,false);
}

/**
 * Create a DrawableImageComponent using any image file.
 */
DrawableImageComponent::DrawableImageComponent
(File imageFile, RectanglePlacement placement) :
placement(placement)
{
    setImage(imageFile);
    setInterceptsMouseClicks(false,false);
}

/**
 * Create a DrawableImageComponent using an image object.
 */
DrawableImageComponent::DrawableImageComponent(Image image,
        RectanglePlacement placement):
placement(placement)
{
    setImage(image);
    setInterceptsMouseClicks(false,false);
}

/**
 * Create a DrawableImageComponent without an initial image.
 */
DrawableImageComponent::DrawableImageComponent
(RectanglePlacement placement) :
placement(placement)
{
    setInterceptsMouseClicks(false,false);
}

DrawableImageComponent::~DrawableImageComponent()
{
}

/**
 * Change the image drawn by this component
 */
void DrawableImageComponent::setImage(String assetFilename)
{
    setImage(assetFile(assetFilename));
}

/**
 * Change the image drawn by this component
 */
void DrawableImageComponent::setImage(File imageFile)
{
    imageDrawable = Drawable::createFromImageFile(imageFile);
    DBG("loading Image file");
    addAndMakeVisible(imageDrawable);
    imageDrawable->setTransformToFit(getLocalBounds().toFloat(), placement);
}

/**
 * Change the image drawn by this component
 */
void DrawableImageComponent::setImage(Image image)
{
    DrawableImage * drawable = new DrawableImage();
    DBG("loading Image object");
    drawable->setImage(image);
    imageDrawable=drawable;
    addAndMakeVisible(imageDrawable);
    imageDrawable->setTransformToFit(getLocalBounds().toFloat(), placement);
}

/**
 * Recursively replace an image color
 */
bool DrawableImageComponent::replaceColour(Colour originalColour,
        Colour replacementColour)
{
    if (imageDrawable == nullptr)
    {
        return false;
    }
    return imageDrawable->replaceColour(originalColour, replacementColour);
}

void DrawableImageComponent::resized()
{
    if (imageDrawable != nullptr)
    {
        imageDrawable->setTransformToFit(getLocalBounds().toFloat(), placement);
    }
}