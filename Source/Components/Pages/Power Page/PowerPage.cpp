#include "MainConfigFile.h"
#include "Utils.h"
#include "PokeLookAndFeel.h"
#include "PowerPage.h"

#if JUCE_DEBUG
//put any includes needed for test routines here.
#include "SavedConnections.h"
#endif

PowerPage::PowerPage() :Localized("PowerPage"),
PageComponent("PowerPage",{
    {1,
        {
            {nullptr, 1}
        }},
    {1,
        {
            {&powerOffButton, 1}
        }},
    {1,
        {
            {&sleepButton, 1}
        }},
    {1,
        {
            {&rebootButton, 1}
        }},
    {1,
        {
            {&felButton, 1}
        }},
#if JUCE_DEBUG
    {1,
        {
            {&testButton, 1}
        }},
#endif
    {1,
        {
            {nullptr, 1}
        }}
}, true, true),
powerOffButton(localeText(shutdown)),
rebootButton(localeText(reboot)),
sleepButton(localeText(sleep)),
felButton(localeText(flash_software)),
#if JUCE_DEBUG
        testButton("Run Test"),
#endif
lockscreen([this]()
{
    hideLockscreen();
})
{

#    if JUCE_DEBUG
    setName("PowerPage");
    testButton.addListener(this);
#    endif
    powerOffButton.addListener(this);
    sleepButton.addListener(this);
    rebootButton.addListener(this);
    felButton.addListener(this);
    addChildComponent(overlaySpinner);
    addAndShowLayoutComponents();
}

/**
 * Turns off the display until key or mouse input is detected.
 * The lock screen will be visible when the display turns on again.
 */
void
PowerPage::startSleepMode()
{
    ChildProcess commandProcess;
    StringArray cmd{ "xset", "q", "|", "grep", "is O"};
    if (commandProcess.start(cmd))
    {
        const String result(commandProcess.readAllProcessOutput());
        if (result == "Monitor is Off")
        {
            commandProcess.start("xset dpms force on");
        }
        else
        {
            addAndMakeVisible(lockscreen);
            lockscreen.setBounds(getLocalBounds());
            lockscreen.setAlwaysOnTop(true);
            //Turn off the screen
            commandProcess.start("xset dpms force off");
        }
    }
    commandProcess.waitForProcessToFinish(10000);
}

/**
 * If the lock screen is visible, this will remove it from the screen.
 */
void PowerPage::hideLockscreen()
{
    if (lockscreen.isShowing())
    {
        removeChildComponent(&lockscreen);
        removeFromStack(Animation::slideInFromLeft);
    }
}

/**
 * Show the power spinner to indicate to the user that the system is
 * restarting or shutting down.
 */
void
PowerPage::showPowerSpinner()
{
    powerOffButton.setVisible(false);
    sleepButton.setVisible(false);
    rebootButton.setVisible(false);
    felButton.setVisible(false);
    overlaySpinner.setVisible(true);
}

/**
 * Resize the lock screen and overlay spinner to fit the page.
 */
void PowerPage::pageResized()
{
    lockscreen.setBounds(getLocalBounds());
    overlaySpinner.setBounds(getLocalBounds());
}

/**
 * Handles all button clicks.
 */
void
PowerPage::pageButtonClicked(Button *button)
{
    if (button == &felButton)
    {
        pushPageToStack(PageType::Fel, Animation::slideInFromLeft);
        return;
    }
    if (button == &sleepButton)
    {
        startSleepMode();
        return;
    }
#if JUCE_DEBUG
    if (button == &testButton)
    {
        DBG("Running connection update tests");
        SavedConnections saved;
        Array<SavedConnection> wifiCons = saved.getWifiConnections();
        if(wifiCons.isEmpty())
        {
            DBG("No connections found!");
            return;
        }
        NMPPConnection first = wifiCons[0].getNMConnection();
        DBG("First connection: ");
        first.printDebugOutput();
        GVariantDict* settingsDict = g_variant_dict_new(nullptr);
        g_variant_dict_insert_value(settingsDict,
                            NM_SETTING_WIRELESS_SECURITY_PSK,
                            g_variant_new_variant(
                            g_variant_new_string("bbbbbbbb!")));
        g_variant_dict_insert_value(settingsDict,
                    NM_SETTING_WIRELESS_SECURITY_PSK_FLAGS,
                    g_variant_new_variant(
                    g_variant_new_int32(NM_SETTING_SECRET_FLAG_NONE)));
        GVariant* newSettings = g_variant_dict_end(settingsDict);
        wifiCons[0].updateWifiSecurity(newSettings);
        g_variant_unref(newSettings);
        
        DBG("Saved updates, checking if changes stuck:");
        SavedConnection refreshed(wifiCons[0].getPath().toRawUTF8());
        DBG("First connection: ");
        refreshed.getNMConnection().printDebugOutput();
    }
#endif
    ChildProcess commandProcess;
    MainConfigFile mainConfig;
    if (button == &powerOffButton)
    {
        showPowerSpinner();
        commandProcess.start(mainConfig.getConfigValue<String>
                (MainConfigFile::shutdownCommandKey));
    }
    else if (button == &rebootButton)
    {
        showPowerSpinner();
        commandProcess.start(mainConfig.getConfigValue<String>
                (MainConfigFile::restartCommandKey));
    }
    commandProcess.waitForProcessToFinish(10000);
}
