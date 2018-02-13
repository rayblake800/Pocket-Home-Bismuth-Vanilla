/**
 * @file PokeLookAndFeel.h
 * 
 * TODO: documentation, organization, better config integration.
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Configuration/ComponentConfigFile.h"
#include "Configuration/Configurables/Configurable.h"

class PokeLookAndFeel : public LookAndFeel_V3, public Configurable {
public:
    PokeLookAndFeel(ComponentConfigFile& config);
    virtual ~PokeLookAndFeel();

    static float getDrawableButtonTextHeightForBounds
    (const Rectangle<int> &bounds);
    
    static float getDrawableButtonImageHeightForBounds
    (const Rectangle<int> &bounds);

    Typeface::Ptr getTypefaceForFont(const Font &font) override;

    void drawLinearSliderThumb(Graphics &g, int x, int y,
            int width, int height, float sliderPos,
            float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle style, Slider &slider) override;
    
    void drawLinearSliderBackground(Graphics &g, int x, int y,
            int width, int height, float sliderPos,
            float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle style, Slider &slider) override;
    
    void drawLinearSlider(Graphics &g, int x, int y,
            int width, int height, float sliderPos,
            float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle style, Slider &slider) override;

    int getSliderThumbRadius(Slider &slider) override;

    void drawButtonText(Graphics &g, TextButton &button,
            bool isMouseOverButton, bool isButtonDown) override;

    void drawButtonBackground(Graphics &, Button &, 
            const Colour &backgroundColour, bool isMouseOverButton, 
            bool isButtonDown) override;

    void drawDrawableButton(Graphics &g, DrawableButton &button, 
            bool isMouseOverButton, bool isButtonDown) override;

    MouseCursor getMouseCursorFor(Component &component) override;

    virtual void setCursorVisible(bool);

    static Colour lightGrey;
    static Colour medGrey;
    static Colour chipPink;
    static Colour chipLightPink;
    static Colour chipPurple;

private:
    /**
     * Reloads and applies object properties defined by a single key in
     * a configuration file
     * @param config the configFile containing the updated data value
     * @param key the key of property that has changed
     */
    virtual void loadConfigProperties(ConfigFile * config,String key);
    
    Typeface::Ptr seguibl;
    MouseCursor::StandardCursorType cursor;
};
