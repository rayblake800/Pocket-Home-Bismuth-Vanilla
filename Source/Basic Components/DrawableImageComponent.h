/**
 * @file DrawableImageComponent.h
 * @author Anthony Brown
 * 
 * DrawableImageComponent is a component that draws a scaled image.
 */

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

class DrawableImageComponent : public Component {
public:
    /**
     * Create a DrawableImageComponent using an asset file.
     * @param assetFilename the filename of an image in assets
     * @param placement defines how the image will be scaled
     */
    DrawableImageComponent(String assetFilename,
            RectanglePlacement placement = RectanglePlacement::centred);

    /**
     * Create a DrawableImageComponent using any image file.
     * @param imageFile an image file
     * @param placement defines how the image will be scaled
     */
    DrawableImageComponent(File imageFile,
            RectanglePlacement placement = RectanglePlacement::centred);
    
    /**
     * Create a DrawableImageComponent using an image object.
     * @param image an image object
     * @param placement defines how the image will be scaled
     */
    DrawableImageComponent(Image image,
            RectanglePlacement placement = RectanglePlacement::centred);
    
    
    /**
     * Create a DrawableImageComponent without an initial image.
     * @param placement defines how the image will be scaled
     */
    DrawableImageComponent
            (RectanglePlacement placement = RectanglePlacement::centred);

    ~DrawableImageComponent();

    /**
     * Change the image drawn by this component
     * @param assetFilename the filename of an image in assets
     */
    void setImage(String assetFilename);

    /**
     * Change the image drawn by this component
     * @param imageFile an image file
     */
    void setImage(File imageFile);
    
    /**
     * Change the image drawn by this component
     * @param image an image object
     */
    void setImage(Image image);

    /**
     * Recursively replace an image color
     * TODO: test this with non-svg image files
     * @param originalColour
     * @param replacementColour
     * @return true if any instances of originalColour were found.
     */
    bool replaceColour(Colour originalColour, Colour replacementColour);

protected:

    void resized() override;
private:

    File imageSource;
    ScopedPointer<Drawable> imageDrawable;
    RectanglePlacement placement;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableImageComponent)
};
