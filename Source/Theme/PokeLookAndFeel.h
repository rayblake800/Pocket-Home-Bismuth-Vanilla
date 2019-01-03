#pragma once
#include "JuceHeader.h"
#include "Config/MainFile.h"
#include "Config/MainListener.h"
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


class PokeLookAndFeel : public juce::LookAndFeel_V4,
        public ColourConfigFile::Listener
{
public:
    PokeLookAndFeel();

    virtual ~PokeLookAndFeel() { }

    /**
     * Get the appropriate typeface for the given Font.
     * 
     * @return seguibl, no other typeface is used.
     */
    juce::Typeface::Ptr getTypefaceForFont(const juce::Font &font) override;

    /**
     * Draws the thumb portion of a linear Slider.
     */
    void drawLinearSliderThumb(
            juce::Graphics &g,
            int x,
            int y,
            int width,
            int height,
            float sliderPos,
            float minSliderPos,
            float maxSliderPos,
            const juce::Slider::SliderStyle style,
            juce::Slider &slider) override;
  
    /**
     * Draws the background of a linear slider.
     */
    void drawLinearSliderBackground(
            juce::Graphics &g,
            int x,
            int y,
            int width,
            int height,
            float sliderPos,
            float minSliderPos,
            float maxSliderPos,
            const juce::Slider::SliderStyle style,
            juce::Slider &slider) override;

    /**
     * Draws the entire linear slider component.  
     */
    void drawLinearSlider(
            juce::Graphics &g,
            int x,
            int y,
            int width,
            int height,
            float sliderPos,
            float minSliderPos,
            float maxSliderPos,
            const juce::Slider::SliderStyle style,
            juce::Slider &slider) override;

    /**
     * Defines the radius in pixels of the Slider thumb.
     */
    int getSliderThumbRadius(juce::Slider &slider) override;

    /**
     * Draws the text onto a TextButton.
     */
    void drawButtonText(
            juce::Graphics &g, 
            juce::TextButton &button,
            bool isMouseOverButton,
            bool isButtonDown) override;

    /**
     * Draws the background of a Button component.
     */
    void drawButtonBackground(
            juce::Graphics& g,
            juce::Button& button,
            const juce::Colour& backgroundColour,
            bool isMouseOverButton,
            bool isButtonDown) override;

    /**
     * Get the cursor to display over a given component.
     * 
     * @param component
     * 
     * @return the same cursor for all components, this application doesn't
     *          need different cursors for different components.
     */
    juce::MouseCursor getMouseCursorFor(juce::Component &component) override;

   /**
    * Gets the default font to use for popup menu text.
    *
    * @return  The default typeface, set to the medium font size defined in
    *          ComponentConfigFile.
    */
    juce::Font getPopupMenuFont() override;

   /**
    * Gets the default font to use for combo box text.
    *
    * @return  The default typeface, set to the small font size defined in
    *          ComponentConfigFile.
    */  
    juce::Font getComboBoxFont (juce::ComboBox& comboBox) override;

   /**
    * Gets the default font to use for Label components.
    *
    * @return  The default typeface, set to the small font size defined in
    *          ComponentConfigFile.
    */ 
    juce::Font getLabelFont (juce::Label& label) override;


   /**
    * Gets the default font to use for alert window title text.
    *
    * @return  The default typeface, set to the large font size defined in
    *          ComponentConfigFile.
    */ 
    juce::Font getAlertWindowTitleFont() override;
   
   /**
    * Gets the default font to use for alert window message text.
    *
    * @return  The default typeface, set to the medium font size defined in
    *          ComponentConfigFile.
    */ 
    juce::Font getAlertWindowMessageFont() override;

private:
    /**
     * Updates the cursor visibility when the associated config key is changed. 
     */
    class MainListener : protected Config::MainListener
    {
    public:
        MainListener(PokeLookAndFeel& owner);
        virtual ~MainListener() { }

        void loadSettings()
        {
            loadAllConfigProperties();
        }
    private:
        /**
         * @param key  The cursor visibility key.
         */
        void configValueChanged(const juce::Identifier& key) override;
        PokeLookAndFeel& owner;
    };
    friend class MainListener;
    MainListener mainListener;
    
    /**
     * Updates Component colours when they're changed in the ColourConfigFile.
     * 
     * @param colourId   The Juce ColourId of the updated Component element.
     * 
     * @param colourKey  The key string of the value updated in the config file.
     * 
     * @param newColour  The new colour value to apply to the colourID.
     */
    virtual void colourChanged(const int colourId,
            const juce::Identifier& colourKey, 
            const juce::Colour newColour) override;

    // All colors are tracked by colourId, this method should do nothing.
    virtual void configValueChanged(const juce::Identifier& key) override { }

    //Defines the maximum number of characters that will fit on a text button.
    static const int maxButtonStrSize = 30;

    //application font
    juce::Typeface::Ptr seguibl;
    //application cursor
    juce::MouseCursor::StandardCursorType cursor;
};
