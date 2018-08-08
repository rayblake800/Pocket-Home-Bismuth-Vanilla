#include <map>
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
versionLabel(localeText(version) 
+ juce::JUCEApplication::getInstance()->getApplicationVersion()),
lockscreen([this]()
{
    hideLockscreen();
})
{
    using namespace juce; 
    powerOffButton.setMaxTextScale(ComponentConfigFile::mediumText);
    rebootButton.setMaxTextScale(ComponentConfigFile::mediumText);
    sleepButton.setMaxTextScale(ComponentConfigFile::mediumText);
    felButton.setMaxTextScale(ComponentConfigFile::mediumText);
    
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(6, { RowItem(&versionLabel) }),
        Row(10, { RowItem(&powerOffButton) }),
        Row(10, { RowItem(&sleepButton) }),
        Row(10, { RowItem(&rebootButton) }),
        Row(10, { RowItem(&felButton) }),
        Row(6, { RowItem(&buildLabel) })
    });
    layout.setYMarginFraction(0.1);
    layout.setYPaddingWeight(4);
    setBackButton(PageComponent::rightBackButton);
    setLayout(layout);
 
    versionLabel.setJustificationType(Justification::centred);
    versionLabel.setText(localeText(version) 
        + juce::JUCEApplication::getInstance()->getApplicationVersion(),
            NotificationType::dontSendNotification);
    
    //Determine release label contents
    File releaseFile("/etc/os-release");
    if(!releaseFile.existsAsFile())
    {
        releaseFile = File("/usr/lib/os-release");
    }
    if(releaseFile.existsAsFile())
    {
        StringArray releaseLines
                = StringArray::fromLines(releaseFile.loadFileAsString());
        std::map<String, String> releaseVars;
        for(const String& line : releaseLines)
        {
            int eqIndex = line.indexOfChar(0, '=');
            if(eqIndex >= 0)
            {
                releaseVars[line.substring(0, eqIndex)]
                        = line.substring(eqIndex + 1).unquoted();
            }
        }
        String buildText = localeText(build);
        if(releaseVars.count("PRETTY_NAME"))
        {
            buildText += releaseVars["PRETTY_NAME"];
        }
        else
        {
            buildText += releaseVars["NAME"];
            buildText += " ";
            buildText += releaseVars["VERSION"];
        }
        buildLabel.setJustificationType(Justification::centred);
        buildLabel.setText(buildText, NotificationType::dontSendNotification);
    }
    
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
void PowerPage::startSleepMode()
{
    using namespace juce;
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
void PowerPage::showPowerSpinner()
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
void PowerPage::pageButtonClicked(juce::Button *button)
{
    using namespace juce;
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
