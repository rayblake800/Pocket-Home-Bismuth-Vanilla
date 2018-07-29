#pragma once
#include "DrawableImageComponent.h"

/**
 * @file DrawableImageButton.h
 * 
 * @brief a Juce button component drawn using a DrawableImageComponent.
 */

class DrawableImageButton : public juce::Button {
public:

    /**
     * @param imageFile  The button image will be loaded from this file.
     * 
     * @param placement  Defines how the image will be scaled to fit the button
     *                    bounds.
     */
    DrawableImageButton(juce::File imageFile, juce::RectanglePlacement placement
            = juce::RectanglePlacement::centred);

    /**
     * @param assetName  The button image will be loaded from the file with 
     *                    this name in the asset folder.
     * 
     * @param placement  Defines how the image will be scaled to fit the button
     *                    bounds.
     */
    DrawableImageButton(juce::String assetName, 
            juce::RectanglePlacement placement 
            = juce::RectanglePlacement::centred);

    /**
     * @param imageObject This will be set as the button's image.
     * 
     * @param placement   Defines how the image will be scaled to fit the 
     *                     button bounds.
     */
    DrawableImageButton(juce::Image imageObject,
            juce::RectanglePlacement placement 
            = juce::RectanglePlacement::centred);

    virtual ~DrawableImageButton() { }

    /**
     * Changes the image drawn by this component.
     * 
     * @param assetFilename The filename of an image in the asset folder.
     */
    void setImage(juce::String assetFilename);

    /**
     * Changes the image drawn by this component.
     * 
     * @param imageFile  Any image file.
     */
    void setImage(juce::File imageFile);
    
    /**
     * Changes the image drawn by this component.
     * 
     * @param image  An image object.
     */
    void setImage(juce::Image imageObject);

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
    void paintButton
    (juce::Graphics &g, bool isMouseOverButton, bool isButtonDown);
    
    //Internal button image component.
    juce::ScopedPointer<DrawableImageComponent> imageComponent;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableImageButton)
};
