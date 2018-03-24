#include "MainConfigFile.h"
#include "Utils.h"
#include "PokeLookAndFeel.h"
#include "PowerPage.h"

PowerPage::PowerPage(
        PageFactoryInterface* pageFactory,
        MainConfigFile& mainConfig,
        ComponentConfigFile& componentConfig) :
Localized("PowerPage"),
PageComponent(componentConfig, "PowerPage",{
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
    {1,
        {
            {nullptr, 1}
        }}
}, pageFactory, true, true),
mainConfig(mainConfig),
powerOffButton(localeText(shutdown)),
rebootButton(localeText(reboot)),
sleepButton(localeText(sleep)),
felButton(localeText(flash_software)),
overlaySpinner(componentConfig),
lockscreen(componentConfig, [this]()
{
    hideLockscreen();
})
{

#    if JUCE_DEBUG
    setName("PowerPage");
#    endif
    setColour(backgroundColourId, Colours::black);
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
#    if JUCE_LINUX
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
#    endif
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
    ChildProcess commandProcess;
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
