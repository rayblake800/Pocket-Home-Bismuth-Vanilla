#pragma once
/**
 * @file  DrawableImageComponent.h
 * 
 * @brief  A Juce component that draws a scaled image. 
 */

#include "JuceHeader.h"

/** 
 * Unlike Juce DrawableImage objects, this component's image will resize itself
 * whenever the component bounds change.
 */
class DrawableImageComponent : public juce::Component {
public:
    /**
     * These ColourId values correspond to the colors set in loadDefaultColours.
     * By using setColour(imageColour<i>Id, newColor), any part of the image
     * with the same color as defaultColors[i] will have its color changed to
     * newColor.  
     * 
     * When changing colors, potential color conflicts are checked
     * for and averted, so you can safely do things like set imageColour0Id
     * to defaultColors[3], and then set imageColour3Id to some new color
     * without also changing the areas set by imageColour0Id.
     */
    enum ColourIds {
        imageColour0Id = 0x1900000,
        imageColour1Id = 0x1900001,
        imageColour2Id = 0x1900002,
        imageColour3Id = 0x1900003,
        imageColour4Id = 0x1900004
    };
    
    /**
     * @brief  Creates a DrawableImageComponent using an image file.
     * 
     * @param assetFilename  The filename of an image in the asset folder, or
     *                        a full image path.
     * 
     * @param placement      Defines how the image will be scaled to fit the
     *                        component.
     */
    DrawableImageComponent(const juce::String assetFilename,
            const juce::RectanglePlacement placement 
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImageComponent using any image file.
     * 
     * @param imageFile  Any image file.
     * 
     * @param placement  Defines how the image will be scaled to fit the
     *                   component.
     */
    DrawableImageComponent(const juce::File imageFile,
            const juce::RectanglePlacement placement 
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImageComponent using an image object.
     * 
     * @param image      Any image object.
     * 
     * @param placement  Defines how the image will be scaled to fit the
     *                   component.
     */
    DrawableImageComponent(const juce::Image image,
            const juce::RectanglePlacement placement 
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImageComponent using a Drawable object.
     * 
     * @param drawable   Any non-null drawable object.
     * 
     * @param placement  Defines how the image will be scaled to fit the
     *                   component.
     */
    DrawableImageComponent(juce::Drawable* drawable,
            const juce::RectanglePlacement placement 
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImageComponent without an initial image.
     * 
     * @param placement  Defines how the image will be scaled to fit the
     *                   component, once it is added.
     */
    DrawableImageComponent(const juce::RectanglePlacement placement 
            = juce::RectanglePlacement::centred);

    virtual ~DrawableImageComponent() { }

    /**
     * @brief  Changes the image drawn by this component.
     * 
     * @param assetFilename  The filename of an image in the asset folder, or
     *                        a full image path.
     */
    void setImage(juce::String assetFilename);

    /**
     * @brief  Changes the image drawn by this component.
     * 
     * @param imageFile  Any image file.
     */
    void setImage(juce::File imageFile);

    /**
     * @brief  Changes the image drawn by this component.
     * 
     * @param image  Any image object.
     */
    void setImage(juce::Image image);

    /**
     * @brief  Changes the image drawn by this component.
     * 
     * @param drawable  Any drawable object.
     */
    void setImage(juce::Drawable* drawable);
    
    /**
     * @brief  Checks if an image is set for this component.
     * 
     * @return  Whether the drawable image is null or has an area of zero
     *          pixels. 
     */
    bool isEmpty();

    /**
     * @brief  Applies component colors to the image.
     */
    virtual void colourChanged() override;

protected:
    /**
     * @brief  Adjusts image size and placement whenever component size changes.
     */
    void resized() override;
    
private:
    /**
     * @brief  Sets the initial image colors and scale.
     */
    void initImage();
    
    /* The image source file, if one is provided: */
    juce::File imageSource;
    
    /* The internal image component: */
    juce::ScopedPointer<juce::Drawable> imageDrawable;
    
    /* The image placement setting: */
    juce::RectanglePlacement placement;

    /* Default image colors, to be replaced by the actual image colors. */
    static const juce::Array<juce::Colour> defaultColours;

    /**
     * @brief  Gets the list of default image colors.
     * 
     * @return  The default image color list. 
     */
    static const juce::Array<juce::Colour> loadDefaultColours() {
        using juce::Colour;
        juce::Array<Colour> defaults;
        defaults.add(Colour(0xffffffff));
        defaults.add(Colour(0xff000000));
        defaults.add(Colour(0xffff0000));
        defaults.add(Colour(0xff00ff00));
        defaults.add(Colour(0xff0000ff));
        return defaults;
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableImageComponent)
};
