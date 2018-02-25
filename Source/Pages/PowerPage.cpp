
#include <numeric>
#include "../Configuration/MainConfigFile.h"
#include "../Utils.h"
#include "../PokeLookAndFeel.h"
#include "PowerPage.h"


PowerPage::PowerPage() :
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
    {1,
        {
            {nullptr, 1}
        }}
}, true, true),
powerOffButton("Shutdown"),
rebootButton("Reboot"),
sleepButton("Sleep"),
felButton("Flash Software"),
lockscreen([this]()
{

    hideLockscreen();
})
{
    setColour(backgroundColourId,Colours::black);
    powerOffButton.addListener(this);
    sleepButton.addListener(this);
    rebootButton.addListener(this);
    felButton.addListener(this);
    addChildComponent(overlaySpinner);
    addAndShowLayoutComponents();
}

PowerPage::~PowerPage() { }

/**
 * Turns off the display until key or mouse input is detected.
 * The lock screen will be visible when the display turns on again.
 */
void
PowerPage::startSleepMode()
{
#if JUCE_LINUX
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
            lockscreen.setAlwaysOnTop(true);
            //Turn off the screen
            commandProcess.start("xset dpms force off");
        }
    }
    commandProcess.waitForProcessToFinish(10000);
#endif
}

/**
 * If the lock screen is visible, this will remove it from the screen.
 */
void PowerPage::hideLockscreen()
{
    if (lockscreen.isShowing())
    {
        removeChildComponent(&lockscreen);
        removeFromStack(PageStackComponent::kTransitionNone);
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
 * Resize all child components to fit the page.
 */
void PowerPage::pageResized()
{
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
        pushPageToStack(&felPage,
                PageStackComponent::kTransitionTranslateHorizontalLeft);
        return;
    }
    if (button == &sleepButton)
    {
        startSleepMode();
        return;
    }
    ChildProcess commandProcess;
    MainConfigFile config;
    if (button == &powerOffButton)
    {
        showPowerSpinner();
        commandProcess.start(config.getConfigValue<String>
                (MainConfigFile::shutdownCommandKey));
    }
    else if (button == &rebootButton)
    {
        showPowerSpinner();
        commandProcess.start(config.getConfigValue<String>
                (MainConfigFile::restartCommandKey));
    }
    commandProcess.waitForProcessToFinish(10000);
}
