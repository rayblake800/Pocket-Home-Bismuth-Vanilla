#pragma once
#include "JuceHeader.h"

/**
 * @file DrawableImageComponent.h
 * 
 * @brief A Juce component that draws a scaled image. 
 * 
 * Unlike Juce DrawableImage objects, this component's image will resize itself
 * whenever the component bounds change.
 */

class DrawableImageComponent : public Component {
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
     * 
     * TODO: check if this works with images not loaded from .svg files
     */
    enum ColourIds {
        imageColour0Id = 0x1900000,
        imageColour1Id = 0x1900001,
        imageColour2Id = 0x1900002,
        imageColour3Id = 0x1900003,
        imageColour4Id = 0x1900004
    };
    
    friend class PokeLookAndFeel;
    /**
     * Create a DrawableImageComponent using an image file.
     * 
     * @param assetFilename  The filename of an image in the asset folder, or
     *                        a full image path.
     * 
     * @param placement      Defines how the image will be scaled to fit the
     *                        component.
     */
    DrawableImageComponent(String assetFilename,
            RectanglePlacement placement = RectanglePlacement::centred);

    /**
     * Create a DrawableImageComponent using any image file.
     * 
     * @param imageFile  Any image file.
     * 
     * @param placement  Defines how the image will be scaled to fit the
     *                    component.
     */
    DrawableImageComponent(File imageFile,
            RectanglePlacement placement = RectanglePlacement::centred);

    /**
     * Create a DrawableImageComponent using an image object.
     * 
     * @param image      Any image object.
     * 
     * @param placement  Defines how the image will be scaled to fit the
     *                    component.
     */
    DrawableImageComponent(Image image,
            RectanglePlacement placement = RectanglePlacement::centred);

    /**
     * Create a DrawableImageComponent using a Drawable object.
     * 
     * @param drawable   Any non-null drawable object.
     * 
     * @param placement  Defines how the image will be scaled to fit the
     *                    component.
     */
    DrawableImageComponent(Drawable* drawable,
            RectanglePlacement placement = RectanglePlacement::centred);


    /**
     * Create a DrawableImageComponent without an initial image.
     * 
     * @param placement  Defines how the image will be scaled to fit the
     *                    component, once it is added.
     */
    DrawableImageComponent
    (RectanglePlacement placement = RectanglePlacement::centred);

    virtual ~DrawableImageComponent() { }

    /**
     * Changes the image drawn by this component.
     * 
     * @param assetFilename  The filename of an image in the asset folder, or
     *                        a full image path.
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
     * @param image  Any image object.
     */
    void setImage(Image image);

    /**
     * Changes the image drawn by this component.
     * 
     * @param drawable  Any drawable object.
     */
    void setImage(Drawable* drawable);
    
    /**
     * Checks if an image is set for this component.
     * 
     * @return  True iff the drawable image is null or has an area of zero
     *          pixels. 
     */
    bool isEmpty();

    /**
     * Apply component colors to the image.
     */
    virtual void colourChanged() override;

protected:
    
    /**
     * Adjust image size and placement whenever component size changes.
     */
    void resized() override;
    
private:
    
    /**
     * After loading an image through any method, this sets the image colors and
     * scale.
     */
    void initImage();
    
    //Image source file, if one is provided.
    File imageSource;
    
    //Internal image component.
    ScopedPointer<Drawable> imageDrawable;
    
    //Image placement setting.
    RectanglePlacement placement;

    //Default image colors, to be replaced by the actual image colors.
    static const Array<Colour> defaultColours;

    /**
     * This gets the list of default image colors, which can be changed through
     * setColour().  
     * 
     * @return the default colors 
     */
    static const Array<Colour> loadDefaultColours() {
        Array<Colour> defaults;
        defaults.add(Colour(0xffffffff));
        defaults.add(Colour(0xff000000));
        defaults.add(Colour(0xffff0000));
        defaults.add(Colour(0xff00ff00));
        defaults.add(Colour(0xff0000ff));
        return defaults;
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableImageComponent)
};
