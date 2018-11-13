#include <map>
#include "Utils.h"
#include "PokeLookAndFeel.h"
#include "PowerPage.h"
#include "TransitionAnimator.h"
#include "SystemCommands.h"


/* Localized object class key: */
static const juce::Identifier localeClassKey = "PowerPage";

/* Localized text keys: */
static const juce::Identifier shutdownTextKey      = "shutdown";
static const juce::Identifier rebootTextKey        = "reboot";
static const juce::Identifier sleepTextKey         = "sleep";
static const juce::Identifier buildTextKey         = "build";
static const juce::Identifier versionTextKey       = "version";
static const juce::Identifier flashSoftwareTextKey = "flashSoftware";

PowerPage::PowerPage() : Locale::TextUser(localeClassKey),
PageComponent("PowerPage"),
powerOffButton(localeText(shutdownTextKey)),
rebootButton(localeText(rebootTextKey)),
sleepButton(localeText(sleepTextKey)),
felButton(localeText(flashSoftwareTextKey)),
versionLabel(localeText(versionTextKey) 
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
    versionLabel.setText(localeText(versionTextKey) 
        + juce::JUCEApplication::getInstance()->getApplicationVersion(),
            NotificationType::dontSendNotification);
    
    // Determine release label contents
    String buildText = localeText(buildTextKey) + " ";
#ifdef BUILD_NAME
    buildText += String(BUILD_NAME);
#else
    buildText += String("unset");
#endif
    buildLabel.setJustificationType(Justification::centred);
    buildLabel.setText(buildText, NotificationType::dontSendNotification);

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
