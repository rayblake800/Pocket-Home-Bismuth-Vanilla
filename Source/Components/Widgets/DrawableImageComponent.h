/**
 * @file DrawableImageComponent.h
 * 
 * DrawableImageComponent is a component that draws a scaled image.
 * TODO: test this with non-svg image files
 */

#pragma once
#include "JuceHeader.h"

class DrawableImageComponent : public Component {
public:
    friend class PokeLookAndFeel;
    /**
     * Create a DrawableImageComponent using an asset file.
     * 
     * @param assetFilename  The filename of an image in the asset folder.
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
     * Create a DrawableImageComponent without an initial image.
     * 
     * @param placement  Defines how the image will be scaled to fit the
     *                    component.
     */
    DrawableImageComponent
    (RectanglePlacement placement = RectanglePlacement::centred);

    virtual ~DrawableImageComponent() { }

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
     * @param image  Any image object.
     */
    void setImage(Image image);

    enum ColourIds {
        imageColour0Id = 0x1900000,
        imageColour1Id = 0x1900001,
        imageColour2Id = 0x1900002,
        imageColour3Id = 0x1900003,
        imageColour4Id = 0x1900004
    };

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
