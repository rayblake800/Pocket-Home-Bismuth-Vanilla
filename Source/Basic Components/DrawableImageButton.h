/**
 * @file DrawableImageButton.h
 * 
 * DrawableImageButton is a button drawn using a DrawableImageComponent
 */
#pragma once
#include "DrawableImageComponent.h"

/**
 * TODO: test replaceColour() with non-svg image files
 */

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

    enum ColourIds {
        imageColour0 = 0x1900000,
        imageColour1 = 0x1900001,
        imageColour2 = 0x1900002,
        imageColour3 = 0x1900003,
        imageColour4 = 0x1900004
    };
    
    
    virtual void colourChanged() override;

protected:
    void updateImageColors();
    /**
     * Resize the image to fit the button.
     */
    void resized() override;
    ScopedPointer<DrawableImageComponent> imageComponent;
private:
    /**
     * Change button alpha on click.
     */
    void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown);
};
