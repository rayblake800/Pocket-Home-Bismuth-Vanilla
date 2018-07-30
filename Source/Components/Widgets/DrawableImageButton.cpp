#include "DrawableImageButton.h"

DrawableImageButton::DrawableImageButton
(juce::File imageFile, juce::RectanglePlacement placement) :
juce::Button(imageFile.getFileName() + juce::String("ImageButton")),
imageComponent(new DrawableImageComponent(imageFile, placement))
{
    addAndMakeVisible(imageComponent);
}

DrawableImageButton::DrawableImageButton
(juce::String assetName, juce::RectanglePlacement placement) :
Button(assetName + juce::String("ImageButton")),
imageComponent(new DrawableImageComponent(assetName, placement))
{
    addAndMakeVisible(imageComponent);
}

DrawableImageButton::DrawableImageButton
(juce::Image imageObject, juce::RectanglePlacement placement) :
Button("DrawableImageButton"),
imageComponent(new DrawableImageComponent(imageObject, placement))
{
    addAndMakeVisible(imageComponent);
}

/**
 * Changed the image drawn by this component.
 */
void DrawableImageButton::setImage(juce::String assetFilename)
{
    imageComponent->setImage(assetFilename);
    updateImageColors();
}

/**
 * Changes the image drawn by this component.
 */
void DrawableImageButton::setImage(juce::File imageFile)
{
    imageComponent->setImage(imageFile);
    updateImageColors();
}

/**
 * Changes the image drawn by this component.
 */
void DrawableImageButton::setImage(juce::Image imageObject)
{
    imageComponent->setImage(imageObject);
    updateImageColors();
}

/**
 * Updates the image colors if UI color values are changed.
 */
void DrawableImageButton::colourChanged()
{
    updateImageColors();
}

/**
 * Loads and applies component colors to the button image.
 */
void DrawableImageButton::updateImageColors()
{
    using namespace juce;
    if (imageComponent != nullptr)
    {
        for (int colourId = imageColour0Id; 
             colourId <= imageColour4Id; 
             colourId++)
        {
            Colour imageColour = imageComponent->findColour(colourId, false);
            Colour buttonColour = findColour(colourId, false);
            if (imageColour != buttonColour)
            {
                imageComponent->setColour(colourId, buttonColour);
            }
        }
    }
}

/**
 * Resizes the image to fit the button.
 */
void DrawableImageButton::resized()
{
    imageComponent->setBounds(getLocalBounds());
}

/**
 * Changes button alpha on click.
 */
void DrawableImageButton::paintButton
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    if (isMouseOverButton && isButtonDown)
    {
        imageComponent->setAlpha(0.3f);
    }
    else
    {
        imageComponent->setAlpha(1.0f);
    }
}
