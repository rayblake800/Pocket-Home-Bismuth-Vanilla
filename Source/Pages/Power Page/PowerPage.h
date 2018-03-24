/**
 * @file PowerPageComponent.h
 * 
 * PowerPageComponent is a UI menu page that shows buttons that perform actions
 * related to system power state.  From this page, the user can shut down the
 * system, reboot the system, turn off the display, or enter PocketCHIP flashing
 * mode.
 */
#pragma once
#include "PageComponent.h"
#include "ConfigurableImageButton.h"
#include "OverlaySpinner.h"
#include "SwitchComponent.h"
#include "LoginPage.h"
#include "Localized.h"
#include "MainConfigFile.h"

/**
 * TODO:
 * 
 * -Set the sleep mode command in config.json
 * -Restore build/version labels.
 * -Make background color configurable.
 */

class PowerPage : public PageComponent, private Localized
{
public:
    /**
     * @param pageFactory        Needed to open the flashing page.
     * 
     * @param mainConfig         Shared config data where power commands are
     *                            stored.
     * 
     * @param componentConfig    Shared UI component preferences. 
     */
    PowerPage(PageFactoryInterface* pageFactory,
            MainConfigFile& mainConfig,
            ComponentConfigFile& componentConfig);

    virtual ~PowerPage() { }
private:
    /**
     * Turns off the display until key or mouse input is detected.
     * The lock screen will be visible when the display turns on again.
     */
    void startSleepMode();

    /**
     * If the lock screen is visible, this will remove it from the screen.
     */
    void hideLockscreen();

    /**
     * Show the power spinner to indicate to the user that the system is
     * restarting or shutting down.
     */
    void showPowerSpinner();
    
    /**
     * Handles all button clicks.
     */
    void pageButtonClicked(Button*) override;

    /**
     * Resize the lock screen and overlay spinner to fit the page.
     */
    void pageResized() override;

    //Used to load power commands
    MainConfigFile& mainConfig;
    //Turns off the system using the shutdown command in the MainConfigFile.
    TextButton powerOffButton;
    //Starts sleep mode with startSleepMode()
    TextButton sleepButton;
    //Restarts the system using the reboot command in the MainConfigFile.
    TextButton rebootButton;
    //Shows a page that gives the user the option to enter flashing mode.
    TextButton felButton;
    //Spinner to indicate that the system is rebooting/shutting down
    OverlaySpinner overlaySpinner;
    //The lock screen is displayed after entering sleep mode.
    LoginPage lockscreen;
    
    
    //localized text keys;
    static const constexpr char * shutdown = "shutdown";
    static const constexpr char * reboot = "reboot";
    static const constexpr char * sleep = "sleep";
    static const constexpr char * flash_software = "flash_software";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PowerPage)
};

