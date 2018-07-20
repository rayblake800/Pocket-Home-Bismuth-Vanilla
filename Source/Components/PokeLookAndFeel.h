#pragma once
#include "JuceHeader.h"
#include "MainConfigFile.h"
#include "ColourConfigFile.h"
#include "ComponentConfigFile.h"

/**
 * @file PokeLookAndFeel.h
 * 
 * @brief sets the default appearance and behavior of all UI component objects. 
 * 
 * It sets UI values and defines custom draw routines for Juce
 * sliders and buttons. It also passes default component colors from 
 * ComponentConfigFile to all components.
 * 
 * Directly interacting with this class should not be necessary, except when
 * the application calls LookAndFeel::setDefaultLookAndFeel() to set a 
 * PokeLookAndFeel object as the default.
 */


class PokeLookAndFeel : public LookAndFeel_V4, public ColourConfigFile::Listener
{
public:
    PokeLookAndFeel();

    virtual ~PokeLookAndFeel() { }

    /**
     * Get the appropriate typeface for the given Font.
     * 
     * @return seguibl, no other typeface is used.
     */
    Typeface::Ptr getTypefaceForFont(const Font &font) override;

    /**
     * Draws the thumb portion of a linear Slider.
     */
    void drawLinearSliderThumb(Graphics &g, int x, int y,
            int width, int height, float sliderPos,
            float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle style, Slider &slider) override;

    
    
    /**
     * Draws the background of a linear slider.
     */
    void drawLinearSliderBackground(Graphics &g, int x, int y,
            int width, int height, float sliderPos,
            float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle style, Slider &slider) override;

    /**
     * Draws the entire linear slider component.  
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
     * 
     * @param component
     * 
     * @return the same cursor for all components, this application doesn't
     *          need different cursors for different components.
     */
    MouseCursor getMouseCursorFor(Component &component) override;

   /**
    * Gets the default font to use for popup menu text.
    *
    * @return  The default typeface, set to the medium font size defined in
    *          ComponentConfigFile.
    */
    Font getPopupMenuFont() override;

   /**
    * Gets the default font to use for combo box text.
    *
    * @return  The default typeface, set to the small font size defined in
    *          ComponentConfigFile.
    */  
    Font getComboBoxFont (ComboBox&) override;

   /**
    * Gets the default font to use for Label components.
    *
    * @return  The default typeface, set to the small font size defined in
    *          ComponentConfigFile.
    */ 
    Font getLabelFont (Label&) override;


   /**
    * Gets the default font to use for alert window title text.
    *
    * @return  The default typeface, set to the large font size defined in
    *          ComponentConfigFile.
    */ 
    Font getAlertWindowTitleFont() override;
   
   /**
    * Gets the default font to use for alert window message text.
    *
    * @return  The default typeface, set to the medium font size defined in
    *          ComponentConfigFile.
    */ 
    Font getAlertWindowMessageFont() override;

private:
    /**
     * Updates the cursor visibility when the associated config key is changed. 
     * 
     * @param key  The cursor visibility key.
     */
    void nonColorValueChanged(String key) override;
    
    /**
     * Updates Component colours when they're changed in the ColourConfigFile.
     * 
     * @param colourID   The Juce ColourID of the updated Component element.
     * 
     * @param colourKey  The key string of the value updated in the config file.
     * 
     * @param newColour  The new colour value to apply to the colourID.
     */
    void colourValueChanged(int colourID, String colourKey, Colour newColour) 
    override;

    //Defines the maximum number of characters that will fit on a text button.
    static const int maxButtonStrSize = 30;

    //application font
    Typeface::Ptr seguibl;
    //application cursor
    MouseCursor::StandardCursorType cursor;
};
