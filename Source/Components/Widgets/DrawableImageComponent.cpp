#include <map>
#include "AssetFiles.h"
#include "DrawableImageComponent.h"

const Array<Colour> DrawableImageComponent::defaultColours =
        DrawableImageComponent::loadDefaultColours();

/**
 * Create a DrawableImageComponent using an asset file.
 */
DrawableImageComponent::DrawableImageComponent
(String assetFilename, RectanglePlacement placement) :
placement(placement)
{

#    if JUCE_DEBUG
    setName(String("DrawableImageComponent:") + assetFilename);
#    endif
    setImage(assetFilename);
    setInterceptsMouseClicks(false, false);
}

/**
 * Create a DrawableImageComponent using any image file.
 */
DrawableImageComponent::DrawableImageComponent
(File imageFile, RectanglePlacement placement) :
placement(placement)
{
#    if JUCE_DEBUG
    setName(String("DrawableImageComponent:") + imageFile.getFileName());
#    endif
    setImage(imageFile);
    setInterceptsMouseClicks(false, false);
}

/**
 * Create a DrawableImageComponent using an image object.
 */
DrawableImageComponent::DrawableImageComponent(Image image,
        RectanglePlacement placement) :
placement(placement)
{

#    if JUCE_DEBUG
    setName("DrawableImageComponent");
#    endif
    setImage(image);
    setInterceptsMouseClicks(false, false);
}

/**
 * Create a DrawableImageComponent using a Drawable object.
 */
DrawableImageComponent::DrawableImageComponent
(Drawable* drawable, RectanglePlacement placement) :
placement(placement)
{
#    if JUCE_DEBUG
    setName("DrawableImageComponent");
#    endif
    setImage(drawable);
    setInterceptsMouseClicks(false, false);

}

/**
 * Create a DrawableImageComponent without an initial image.
 */
DrawableImageComponent::DrawableImageComponent
(RectanglePlacement placement) :
placement(placement)
{
#    if JUCE_DEBUG
    setName("DrawableImageComponent");
#    endif
    setInterceptsMouseClicks(false, false);
}

/**
 * Changes the image drawn by this component.
 */
void DrawableImageComponent::setImage(String assetFilename)
{
    if (assetFilename.isNotEmpty())
    {
        setImage(AssetFiles::findAssetFile(assetFilename));
    }
}

/**
 * Changes the image drawn by this component.
 */
void DrawableImageComponent::setImage(File imageFile)
{
    if (imageFile.existsAsFile())
    {
        imageDrawable = Drawable::createFromImageFile(imageFile);
        if (imageDrawable != nullptr)
        {
            imageSource = imageFile;
            initImage();
        }
        else
        {
            DBG("DrawableImageComponent::" << __func__ << ": Failed to load "
                    << imageFile.getFullPathName());
        }
    }
}

/**
 * Changes the image drawn by this component.
 */
void DrawableImageComponent::setImage(Image image)
{
    DrawableImage * drawable = new DrawableImage();
    drawable->setImage(image);
    imageDrawable = drawable;
    initImage();
}

/**
 * Changes the image drawn by this component.
 */
void DrawableImageComponent::setImage(Drawable* drawable)
{
    imageDrawable = drawable;
    initImage();
}
 
/*
 * Checks if an image is set for this component.
 */
bool DrawableImageComponent::isEmpty()
{
    if(imageDrawable == nullptr)
    {
        return true;
    }
    return imageDrawable->getBounds().isEmpty();
}

/**
 * Apply component colors to the image.
 */
void DrawableImageComponent::colourChanged()
{
    if (imageSource.existsAsFile())
    {
        setImage(imageSource);
    }
}

/**
 * Adjust image size and placement whenever component size changes.
 */
void DrawableImageComponent::resized()
{
    if (imageDrawable != nullptr)
    {
        imageDrawable->setTransformToFit(getLocalBounds().toFloat(), placement);
    }
}

/**
 * After loading an image through any method, this sets the image colors and
 * scale.
 */
void DrawableImageComponent::initImage()
{
    addAndMakeVisible(imageDrawable);
    imageDrawable->setTransformToFit(getLocalBounds().toFloat(), placement);
    Array<Colour> imageColours;
    std::map<uint32, uint32> tempColours;
    for (int i = 0; i < defaultColours.size(); i++)
    {
        imageColours.add(findColour(imageColour0Id + i));
    }
    for (int i = 0; i < defaultColours.size(); i++)
    {
        int colourId = imageColour0Id + i;
        /**
         * Let i and j be colour indices where j > i.  If colour[i] is changing 
         * from a to b, and colour[j] is changing from b to c, set colour[i] to 
         * some temporary value so that in the end we get colour[i] = b, 
         * colour[j]=c instead of colour[i] = c, colour[j] = c
         */
        int existingIndex = defaultColours.indexOf(imageColours[i]);
        if (existingIndex < i)
        {
            //Color conflict doesn't exist, or involves a color that has
            //already been changed, so direct replacement is possible.
            imageDrawable->replaceColour(defaultColours[i], imageColours[i]);
        }
        else if (existingIndex > i)
        {
            //Color conflict exists, replace the color with a temporary color
            //that's not already used.
            Colour tempColour = Colours::aqua;
            Random randGen;
            while (defaultColours.contains(tempColour) ||
                   imageColours.contains(tempColour) ||
                   tempColours.find(tempColour.getARGB()) != tempColours.end())
            {
                tempColour = Colour(0xff000000
                        + randGen.nextInt(Range<int>::between(0, 0xffffff)));
            }
            tempColours[tempColour.getARGB()] = imageColours[i].getARGB();
            imageDrawable->replaceColour(defaultColours[i], tempColour);
        }
        //if existingIndex == i, the color doesn't change, so no action needed.
    }
    //temporary colors can now be safely replaced with their actual values
    for (std::map<uint32, uint32>::iterator it = tempColours.begin();
         it != tempColours.end(); it++)
    {
        imageDrawable->replaceColour(Colour(it->first), Colour(it->second));
    }
}
