/**
 * @file DrawableImageButton.h
 * 
 * DrawableImageButton is a button drawn using a DrawableImageComponent.
 */
#pragma once
#include "DrawableImageComponent.h"


class DrawableImageButton : public Button {
public:

    /**
     * @param imageFile  The button image will be loaded from this file.
     * 
     * @param placement  Defines how the image will be scaled to fit the button
     *                    bounds.
     */
    DrawableImageButton(File imageFile,
            RectanglePlacement placement = RectanglePlacement::centred);

    /**
     * @param assetName  The button image will be loaded from the file with 
     *                    this name in the asset folder.
     * 
     * @param placement  Defines how the image will be scaled to fit the button
     *                    bounds.
     */
    DrawableImageButton(String assetName,
            RectanglePlacement placement = RectanglePlacement::centred);

    /**
     * @param imageObject This will be set as the button's image.
     * 
     * @param placement   Defines how the image will be scaled to fit the 
     *                     button bounds.
     */
    DrawableImageButton(Image imageObject,
            RectanglePlacement placement = RectanglePlacement::centred);

    virtual ~DrawableImageButton() { }

    /**
     * Changes the image drawn by this component.
     * 
     * @param assetFilename The filename of an image in the asset folder.
     */
    void setImage(String assetFilename);

    /**
     * Changes the image drawn by this component.
     * 
     * @param imageFile  Any image file.
     */
    void setImage(File imageFile);
    
    /**
     * Changes the image drawn by this component.
     * 
     * @param image  An image object.
     */
    void setImage(Image imageObject);

    enum ColourIds {
        imageColour0Id = 0x1900000,
        imageColour1Id = 0x1900001,
        imageColour2Id = 0x1900002,
        imageColour3Id = 0x1900003,
        imageColour4Id = 0x1900004
    };
    
    /**
     * Updates the image colors if UI color values are changed.
     */
    virtual void colourChanged() override;

protected:
    /**
     * Loads and applies component colors to the button image.
     */
    void updateImageColors();
    
    /**
     * Resizes the image to fit the button.
     */
    void resized() override;
    
private:
    /**
     * Changes button alpha on click.
     */
    void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown);
    
    //Internal button image component.
    ScopedPointer<DrawableImageComponent> imageComponent;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableImageButton)
};
