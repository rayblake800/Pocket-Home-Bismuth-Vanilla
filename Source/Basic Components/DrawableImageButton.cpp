#include "DrawableImageButton.h"

/**
 * @param imageFile the button image will be loaded from this file.
 * @param placement defines how the image will be scaled
 */
DrawableImageButton::DrawableImageButton(File imageFile,
        RectanglePlacement placement) :
Button(imageFile.getFileName() + String("ImageButton")),
imageComponent(new DrawableImageComponent(imageFile, placement))
{
    addAndMakeVisible(imageComponent);
}

/**
 * @param assetName the button image will be loaded from this asset file.
 * @param placement defines how the image will be scaled
 */
DrawableImageButton::DrawableImageButton(String assetName,
        RectanglePlacement placement) :
Button(assetName + String("ImageButton")),
imageComponent(new DrawableImageComponent(assetName, placement))
{
    addAndMakeVisible(imageComponent);
}

/**
 * @param imageObject the button image will be loaded from this image.
 * @param placement defines how the image will be scaled
 */
DrawableImageButton::DrawableImageButton(Image imageObject,
        RectanglePlacement placement) :
Button("DrawableImageButton"),
imageComponent(new DrawableImageComponent(imageObject, placement))
{
    addAndMakeVisible(imageComponent);
}

DrawableImageButton::~DrawableImageButton()
{
}

/**
 * Change the image drawn by this component
 */
void DrawableImageButton::setImage(String assetFilename)
{
    imageComponent->setImage(assetFilename);
}

/**
 * Change the image drawn by this component
 */
void DrawableImageButton::setImage(File imageFile)
{
    imageComponent->setImage(imageFile);
}

/**
 * Change the image drawn by this component
 */
void DrawableImageButton::setImage(Image imageObject)
{
    imageComponent->setImage(imageObject);
}

/**
 * Recursively replace an image color
 */
bool DrawableImageButton::replaceColour
(Colour originalColour, Colour replacementColour)
{
    imageComponent->replaceColour(originalColour, replacementColour);
}

/**
 * Resize the image to fit the button.
 */
void DrawableImageButton::resized()
{
    imageComponent->setBounds(getLocalBounds());
}

/**
 * Change button alpha on click.
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
