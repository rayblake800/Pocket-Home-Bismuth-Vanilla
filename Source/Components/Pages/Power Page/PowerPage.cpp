#include "MainConfigFile.h"
#include "Utils.h"
#include "PokeLookAndFeel.h"
#include "PowerPage.h"
#include "TransitionAnimator.h"

#if JUCE_DEBUG
//put any includes needed for test routines here.
#include "SavedConnections.h"
#endif

PowerPage::PowerPage() :Localized("PowerPage"),
PageComponent("PowerPage"),
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
      
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(10, { RowItem(&powerOffButton) }),
        Row(10, { RowItem(&sleepButton) }),
        Row(10, { RowItem(&rebootButton) }),
        Row(10, { RowItem(&felButton) })
#if JUCE_DEBUG
        ,Row(10, { RowItem(&testButton) })
#endif
    });
    layout.setYMarginFraction(0.1);
    layout.setYPaddingWeight(4);
    setBackButton(PageComponent::rightBackButton);
    setLayout(layout);
    
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
        removeFromStack(TransitionAnimator::moveRight);
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
        pushPageToStack(PageType::Fel, TransitionAnimator::moveRight);
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
