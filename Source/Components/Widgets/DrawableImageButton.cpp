#include "DrawableImageButton.h"

DrawableImageButton::DrawableImageButton
(File imageFile, RectanglePlacement placement) :
Button(imageFile.getFileName() + String("ImageButton")),
imageComponent(new DrawableImageComponent(imageFile, placement))
{
    addAndMakeVisible(imageComponent);
}

DrawableImageButton::DrawableImageButton
(String assetName, RectanglePlacement placement) :
Button(assetName + String("ImageButton")),
imageComponent(new DrawableImageComponent(assetName, placement))
{
    addAndMakeVisible(imageComponent);
}

DrawableImageButton::DrawableImageButton
(Image imageObject, RectanglePlacement placement) :
Button("DrawableImageButton"),
imageComponent(new DrawableImageComponent(imageObject, placement))
{
    addAndMakeVisible(imageComponent);
}

/**
 * Changed the image drawn by this component.
 */
void DrawableImageButton::setImage(String assetFilename)
{
    imageComponent->setImage(assetFilename);
    updateImageColors();
}

/**
 * Changes the image drawn by this component.
 */
void DrawableImageButton::setImage(File imageFile)
{
    imageComponent->setImage(imageFile);
    updateImageColors();
}

/**
 * Changes the image drawn by this component.
 */
void DrawableImageButton::setImage(Image imageObject)
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
(Graphics &g, bool isMouseOverButton, bool isButtonDown)
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
