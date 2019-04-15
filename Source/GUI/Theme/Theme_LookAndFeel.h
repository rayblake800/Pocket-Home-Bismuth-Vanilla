#pragma once
/**
 * @file  Theme_LookAndFeel.h
 *
 * @brief  Sets the default appearance and behavior of all UI component objects.
 */

#include "JuceHeader.h"
#include "Config_MainFile.h"
#include "Config_MainListener.h"
#include "Theme_Colour_ConfigFile.h"
#include "Theme_Colour_ConfigListener.h"

namespace Theme { class LookAndFeel; }

/**
 * @brief  Sets UI values and defines custom draw routines for JUCE sliders and
 *         buttons. It also passes default component colours from the Colour
 *         configuration file to all components.
 *
 *  Directly interacting with this class should not be necessary, except when
 * the application calls LookAndFeel::setDefaultLookAndFeel() to set a
 * Theme::LookAndFeel object as the default.
 */
class Theme::LookAndFeel : public juce::LookAndFeel_V4,
        public Colour::ConfigListener
{
public:
    LookAndFeel();

    virtual ~LookAndFeel() { }

    /**
     * @brief  Get the appropriate typeface for the given Font.
     *
     * @return  seguibl, no other typeface is used.
     */
    juce::Typeface::Ptr getTypefaceForFont(const juce::Font &font) override;

    /**
     * @brief  Draws the thumb portion of a linear Slider.
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
     * @brief  Draws the background of a linear slider.
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
     * @brief  Draws the entire linear slider component.
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
     * @brief  Defines the radius in pixels of the Slider thumb.
     */
    int getSliderThumbRadius(juce::Slider &slider) override;

    /**
     * @brief  Draws the text onto a TextButton.
     */
    void drawButtonText(
            juce::Graphics &g,
            juce::TextButton &button,
            bool isMouseOverButton,
            bool isButtonDown) override;

    /**
     * @brief  Draws the background of a Button component.
     */
    void drawButtonBackground(
            juce::Graphics& g,
            juce::Button& button,
            const juce::Colour& backgroundColour,
            bool isMouseOverButton,
            bool isButtonDown) override;

    /**
     * @brief  Get the cursor to display over a given component.
     *
     * @param component
     *
     * @return            The same cursor for all components, as this
     *                    application doesn't need different cursors for
     *                    different components.
     */
    juce::MouseCursor getMouseCursorFor(juce::Component &component) override;

    /**
     * @brief  Gets the default font to use for popup menu text.
     *
     * @return  The default typeface, set to the medium font size defined in
     *          component layout configuration file.
     */
    juce::Font getPopupMenuFont() override;

    /**
     * @brief  Gets the default font to use for combo box text.
     *
     * @return  The default typeface, set to the small font size defined in the
     *          component layout configuration file.
     */
    juce::Font getComboBoxFont(juce::ComboBox& comboBox) override;

    /**
     * @brief  Gets the default font to use for Label components.
     *
     * @return  The default typeface, set to the small font size defined in
     *          component layout configuration file.
     */
    juce::Font getLabelFont(juce::Label& label) override;


    /**
     * @brief  Gets the default font to use for alert window title text.
     *
     * @return  The default typeface, set to the large font size defined in
     *          component layout configuration file.
     */
    juce::Font getAlertWindowTitleFont() override;

    /**
     * @brief  Gets the default font to use for alert window message text.
     *
     * @return  The default typeface, set to the medium font size defined in
     *          component layout configuration file.
     */
    juce::Font getAlertWindowMessageFont() override;

private:
    /**
     * @brief  Updates the cursor visibility when the associated config key is
     *         changed.
     */
    class MainListener : public Config::MainListener
    {
    public:
        MainListener(LookAndFeel& owner);
        virtual ~MainListener() { }

        void loadSettings()
        {
            loadAllConfigProperties();
        }
    private:
        /**
         * @brief  Notifies the LookAndFeel object when cursor visibility
         *         settings change.
         *
         * @param key  The updated key, which should always be the cursor
         *             visibility key.
         */
        void configValueChanged(const juce::Identifier& key) override;
        LookAndFeel& owner;
    };
    friend class MainListener;
    MainListener mainListener;

    /**
     * @brief  Updates Component colours when they're changed in the component
     *         layout configuration file.
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

    // All colours are tracked by colourId, this method should do nothing.
    virtual void configValueChanged(const juce::Identifier& key) override { }

    // Defines the maximum number of characters that will fit on a text button.
    static const int maxButtonStrSize = 30;

    // Application font:
    juce::Typeface::Ptr seguibl;
    // Application cursor:
    juce::MouseCursor::StandardCursorType cursor;
};
