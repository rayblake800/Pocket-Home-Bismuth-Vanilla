/**
 * @file DrawableImageComponent.h
 * 
 * DrawableImageComponent is a component that draws a scaled image.
 * TODO: test this with non-svg image files
 */

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

class DrawableImageComponent : public Component {
public:
    friend class PokeLookAndFeel;
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

    enum ColourIds {
        imageColour0Id = 0x1900000,
        imageColour1Id = 0x1900001,
        imageColour2Id = 0x1900002,
        imageColour3Id = 0x1900003,
        imageColour4Id = 0x1900004
    };

    virtual void colourChanged() override;

protected:
    /**
     * Adjust image size and placement to match whenever component size changes.
     */
    void resized() override;
private:
    
    /**
     * After loading an image through any method, this sets the image colors and
     * scale.
     */
    void initImage();
    
    File imageSource;
    ScopedPointer<Drawable> imageDrawable;
    RectanglePlacement placement;

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
