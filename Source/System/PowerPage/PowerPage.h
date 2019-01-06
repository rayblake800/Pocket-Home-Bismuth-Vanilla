#pragma once
/**
 * @file  PowerPageComponent.h
 * 
 * @brief  A UI menu page that shows buttons that perform actions related to
 *         system power state.  
 */

#include "PageComponent.h"
#include "OverlaySpinner.h"
#include "LoginPage.h"
#include "Locale/TextUser.h"

/**
 * From this page, the user can shut down the system, reboot the system, turn 
 * off the display, or enter PocketCHIP flashing mode.
 */
class PowerPage : public PageComponent, private Locale::TextUser
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
     * @brief  If the lock screen is visible, removes it from the screen.
     */
    void hideLockscreen();

    /**
     * @brief  Shows the power spinner to indicate to the user that the system 
     *         is restarting or shutting down.
     */
    void showPowerSpinner();
    
    /**
     * @brief  Handles all button clicks.
     *
     * @param button  A button component on the page that was clicked.
     */
    virtual void pageButtonClicked(juce::Button* button) override;

    /**
     * @brief  Resizes the lock screen and overlay spinner to fit the page.
     */
    virtual void pageResized() override;

    /* Identifies this pocket-home build */
    ScalingLabel buildLabel;
    /* Turns off the system using the shutdown command in the MainConfigFile. */
    juce::TextButton powerOffButton;
    /* Starts sleep mode with startSleepMode() */
    juce::TextButton sleepButton;
    /* Restarts the system using the reboot command in the MainConfigFile. */
    juce::TextButton rebootButton;
    /* Shows a page that gives the user the option to enter flashing mode. */
    juce::TextButton felButton;
    /* Identifies the pocket-home version */
    ScalingLabel versionLabel;
    /* Spinner to indicate that the system is rebooting/shutting down */
    OverlaySpinner overlaySpinner;
    /* The lock screen is displayed after entering sleep mode. */
    LoginPage lockscreen;   
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PowerPage)
};
