#include "MainConfigFile.h"
#include "Utils.h"
#include "PokeLookAndFeel.h"
#include "PowerPage.h"

#if JUCE_DEBUG
//put any includes needed for test routines here.
#include "NMPPClient.h"
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
        DBG("Running LibNM wrapper tests");
        DBG("Testing NMPPClient:");
        NMPPClient client;
        bool enabled = client.wirelessEnabled();
        DBG("\tClient created.  Wireless enabled:"  
                << (enabled ? "yes" : "no"));
        Array<NMPPDeviceWifi> devices = client.getWifiDevices();
        DBG("\tFound " << devices.size() << " wifi devices");
        for(const NMPPDeviceWifi& device : devices)
        {
            DBG("\tDevice " << device.getInterface());
            DBG("\t\tPath " << device.getPath());
            DBG("\t\tManaged:" << (device.isManaged() ? "yes" : "no"));
            Array<NMPPAccessPoint> aps = device.getAccessPoints();
            DBG("\t\tFound "  << aps.size() << " access points");
            Array<NMPPConnection> saved = device.getAvailableConnections();
            DBG("\t\tFound " << saved.size() << " saved connections");
        }
        
        Array<NMPPActiveConnection> active = client.getActiveConnections();
        DBG("\tFound " << active.size() << " active connections");
        
        NMPPActiveConnection primary = client.getPrimaryConnection();
        if(!primary.isNull())
        {
            DBG("\tPrimary Connection id:" << primary.getID());
        }
        else
        {
            DBG("\tNo primary connection found.");
        }
        
        
        NMPPActiveConnection activating = client.getActivatingConnection();
        if(!activating.isNull())
        {
            DBG("\tActivating Connection id:" << activating.getID());
        }
        else
        {
            DBG("\tNo activating connection found.");
        }
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
