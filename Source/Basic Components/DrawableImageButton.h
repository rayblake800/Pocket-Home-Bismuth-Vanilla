/**
 * @file DrawableImageButton.h
 * 
 * DrawableImageButton is a button drawn using a DrawableImageComponent
 */


#pragma once
#include "DrawableImageComponent.h"

class DrawableImageButton : public Button {
public:

    /**
     * @param imageFile the button image will be loaded from this file.
     * @param placement defines how the image will be scaled
     */
    DrawableImageButton(File imageFile,
            RectanglePlacement placement = RectanglePlacement::centred);

    /**
     * @param assetName the button image will be loaded from this asset file.
     * @param placement defines how the image will be scaled
     */
    DrawableImageButton(String assetName,
            RectanglePlacement placement = RectanglePlacement::centred);

    /**
     * @param imageObject the button image will be loaded from this image.
     * @param placement defines how the image will be scaled
     */
    DrawableImageButton(Image imageObject,
            RectanglePlacement placement = RectanglePlacement::centred);

    virtual ~DrawableImageButton();

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
    void setImage(Image imageObject);

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
    
    //no special painting needed, button appearance is handled by the 
    //configurable image
    void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown);
    DrawableImageComponent imageComponent;
};
