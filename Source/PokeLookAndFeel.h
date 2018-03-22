/**
 * @file PokeLookAndFeel.h
 * 
 * PokeLookAndFeel sets the default appearance and behavior of all UI component
 * objects.  It sets UI values and defines custom draw routines for Juce
 * sliders and buttons. It also passes default component colors from 
 * ComponentConfigFile to all components.
 * 
 * Directly interacting with this class should not be necessary, except when
 * the application calls LookAndFeel::setDefaultLookAndFeel() to set a 
 * PokeLookAndFeel object as the default.
 */

#pragma once
#include "JuceHeader.h"
#include "MainConfigFile.h"
#include "ComponentConfigFile.h"

class PokeLookAndFeel : public LookAndFeel_V3, public ConfigFile::Listener
{
public:
    PokeLookAndFeel(MainConfigFile& mainConfig,
            ComponentConfigFile& componentConfig);

    virtual ~PokeLookAndFeel() {}

    /**
     * @return the most appropriate text height for a DrawableButton with the
     * given bounds.
     */
    static float getDrawableButtonTextHeightForBounds
    (const Rectangle<int>& bounds);

    /**
     * @return the most appropriate image height for a DrawableButton with the
     * given bounds.
     */
    static float getDrawableButtonImageHeightForBounds
    (const Rectangle<int> &bounds);

    /**
     * Draws a DrawableButton component.
     */
    void drawDrawableButton(Graphics &g, DrawableButton &button,
            bool isMouseOverButton, bool isButtonDown) override;

    /**
     * Get the appropriate typeface for the given Font.
     * @return seguibl, no other typeface is used.
     */
    Typeface::Ptr getTypefaceForFont(const Font &font) override;

    /**
     * Draws the thumb portion of a linear Slider
     */
    void drawLinearSliderThumb(Graphics &g, int x, int y,
            int width, int height, float sliderPos,
            float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle style, Slider &slider) override;

    /**
     * Draws the background of a linear Slider
     */
    void drawLinearSliderBackground(Graphics &g, int x, int y,
            int width, int height, float sliderPos,
            float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle style, Slider &slider) override;

    /**
     * Draws the entire linear Slider component.
     */
    void drawLinearSlider(Graphics &g, int x, int y,
            int width, int height, float sliderPos,
            float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle style, Slider &slider) override;

    /**
     * Defines the radius in pixels of the Slider thumb.
     */
    int getSliderThumbRadius(Slider &slider) override;

    /**
     * Draws the text onto a TextButton.
     */
    void drawButtonText(Graphics &g, TextButton &button,
            bool isMouseOverButton, bool isButtonDown) override;

    /**
     * Draws the background of a Button component.
     */
    void drawButtonBackground(Graphics &, Button &,
            const Colour &backgroundColour, bool isMouseOverButton,
            bool isButtonDown) override;

    /**
     * Get the cursor to display over a given component.
     * @param component
     * @return the same cursor for all components, this application doesn't
     * need different cursors for different components.
     */
    MouseCursor getMouseCursorFor(Component &component) override;

private:
    /**
     * Loads and applies component colors from components.json, and updates
     * cursor visibility.
     * @param config
     * @param key
     */
    void configValueChanged(ConfigFile* config, String key) override;
    
    //reference to configuration files
    MainConfigFile& mainConfig;
    ComponentConfigFile& componentConfig;

    //Defines the maximum number of characters that will fit on a text button.
    static const int maxButtonStrSize = 30;

    Typeface::Ptr seguibl;
    MouseCursor::StandardCursorType cursor;
};
