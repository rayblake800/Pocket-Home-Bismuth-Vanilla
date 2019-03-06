#pragma once
/**
 * @file  PowerPage.h
 * 
 * @brief  Provides the system power control page.
 */

#include "Page_Component.h"
#include "Widgets_OverlaySpinner.h"
#include "Locale_TextUser.h"

/**
 * @brief  A UI menu page that shows buttons that perform actions related to
 *         system power state.
 *
 * From this page, the user can shut down the system, reboot the system, turn 
 * off the display, or enter PocketCHIP flashing mode.
 */
class PowerPage : public Page::Component, private Locale::TextUser
{
public:
    
    PowerPage();

    virtual ~PowerPage() { }
private:
    /**
     * @brief  Turns off the display until key or mouse input is detected.
     * 
     * The lock screen will be visible when the display turns on again.
     */
    void startSleepMode();

    /**
     * @brief  Shows the power spinner to indicate to the user that the system 
     *         is restarting or shutting down.
     */
    void showPowerSpinner();

    /**
     * @brief  Resizes the lock screen and overlay spinner to fit the page.
     */
    virtual void pageResized() override;

    class PageListener : public juce::Button::Listener
    {
    public:
        /**
         * @brief  Connects the listener to its power page.
         *
         * @param powerPage  The PowerPage that owns this PageListener.
         */
        PageListener(PowerPage& powerPage) : powerPage(powerPage) { }

    private:
        /**
         * @brief  Handles all button clicks within the power page.
         *
         * @param button  A button component on the power page that was clicked.
         */
        virtual void buttonClicked(juce::Button* button) override;

        PowerPage& powerPage;
    };
    PageListener pageListener;

    /* Identifies this pocket-home build: */
    Widgets::BoundedLabel buildLabel;
    /* Turns off the system: */
    juce::TextButton powerOffButton;
    /* Starts sleep mode: */
    juce::TextButton sleepButton;
    /* Restarts the system: */
    juce::TextButton rebootButton;
    /* Shows a page that gives the user the option to enter flashing mode: */
    juce::TextButton felButton;
    /* Identifies the pocket-home version:*/
    Widgets::BoundedLabel versionLabel;
    /* Spinner to indicate that the system is rebooting/shutting down: */
    Widgets::OverlaySpinner overlaySpinner;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PowerPage)
};
