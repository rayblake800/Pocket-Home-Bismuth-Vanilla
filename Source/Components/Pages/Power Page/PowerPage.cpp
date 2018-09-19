#include <map>
#include "Utils.h"
#include "PokeLookAndFeel.h"
#include "PowerPage.h"
#include "TransitionAnimator.h"
#include "SystemCommands.h"

/* Localized text keys: */
static const constexpr char * shutdown = "shutdown";
static const constexpr char * reboot = "reboot";
static const constexpr char * sleep = "sleep";
static const constexpr char * build = "build";
static const constexpr char * version = "version";
static const constexpr char * flash_software = "flash_software";

/* Path to the release file used to determine build type */
static const constexpr char* releaseFilePath = "/etc/os-release";
/* Alternate path where the release file may be located */
static const constexpr char* altReleaseFilePath = "/usr/lib/os-release";

/* Release file data keys: */
/* Nicely formatted system name key */
static const constexpr char* prettyNameKey = "PRETTY_NAME";
/* System name key */
static const constexpr char* nameKey = "NAME";
/* System version key */
static const constexpr char* versionKey = "VERSION";

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
    
    // Determine release label contents
    File releaseFile(releaseFilePath);
    if(!releaseFile.existsAsFile())
    {
        releaseFile = File(altReleaseFilePath);
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
        if(releaseVars.count(prettyNameKey))
        {
            buildText += releaseVars[prettyNameKey];
        }
        else
        {
            buildText += releaseVars[nameKey];
            buildText += " ";
            buildText += releaseVars[versionKey];
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

/*
 * Turns off the display until key or mouse input is detected.
 */
void PowerPage::startSleepMode()
{
    using namespace juce;
    SystemCommands systemCommands;
    if(systemCommands.runIntCommand(SystemCommands::IntCommand::sleepCheck)
            == 0)
    {
        systemCommands.runActionCommand(SystemCommands::ActionCommand::wake);
    }
    else
    {
        addAndMakeVisible(lockscreen);
        lockscreen.setBounds(getLocalBounds());
        lockscreen.setAlwaysOnTop(true);
        systemCommands.runActionCommand(SystemCommands::ActionCommand::sleep);
    }
}

/*
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

/*
 * Shows the power spinner to indicate to the user that the system is
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

/*
 * Resizes the lock screen and overlay spinner to fit the page.
 */
void PowerPage::pageResized()
{
    lockscreen.setBounds(getLocalBounds());
    overlaySpinner.setBounds(getLocalBounds());
}

/*
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

    SystemCommands systemCommands;
    if (button == &powerOffButton)
    {
        showPowerSpinner();
        systemCommands.runActionCommand
            (SystemCommands::ActionCommand::shutdown);
    }
    else if (button == &rebootButton)
    {
        showPowerSpinner();
        systemCommands.runActionCommand
            (SystemCommands::ActionCommand::restart);
    }
}
