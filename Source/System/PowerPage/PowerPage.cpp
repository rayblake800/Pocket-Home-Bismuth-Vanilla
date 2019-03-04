#include "PowerPage.h"
#include "Page_Type.h"
#include "Layout_Transition_Animator.h"
#include "SystemCommands.h"
#include "Utils.h"
#include <map>


/* Localized object class key: */
static const juce::Identifier localeClassKey = "PowerPage";

/* Localized text keys: */
namespace TextKey
{
    static const juce::Identifier shutdown      = "shutdown";
    static const juce::Identifier reboot        = "reboot";
    static const juce::Identifier sleep         = "sleep";
    static const juce::Identifier build         = "build";
    static const juce::Identifier version       = "version";
    static const juce::Identifier flashSoftware = "flashSoftware";
}

PowerPage::PowerPage() : Locale::TextUser(localeClassKey),
pageListener(*this),
powerOffButton(localeText(TextKey::shutdown)),
rebootButton(localeText(TextKey::reboot)),
sleepButton(localeText(TextKey::sleep)),
felButton(localeText(TextKey::flashSoftware)),
versionLabel(localeText(TextKey::version) 
+ juce::JUCEApplication::getInstance()->getApplicationVersion()),
lockscreen([this]()
{
    hideLockscreen();
})
{
#ifdef JUCE_DEBUG
    setName("PowerPage");
#endif
    using namespace Layout::Group;
    RelativeLayout layout({
        Row(6, { RowItem(&versionLabel) }),
        Row(10, { RowItem(&powerOffButton) }),
        Row(10, { RowItem(&sleepButton) }),
        Row(10, { RowItem(&rebootButton) }),
        Row(10, { RowItem(&felButton) }),
        Row(6, { RowItem(&buildLabel) })
    });
    layout.setYMarginFraction(0.1);
    layout.setYPaddingWeight(4);
    setBackButton(BackButtonType::right);
    setLayout(layout);
 
    versionLabel.setJustificationType(juce::Justification::centred);
    versionLabel.setText(localeText(TextKey::version) 
        + juce::JUCEApplication::getInstance()->getApplicationVersion(),
            juce::NotificationType::dontSendNotification);
    
    // Determine release label contents
    juce::String buildText = localeText(TextKey::build) + " ";
#ifdef BUILD_NAME
    buildText += juce::String(BUILD_NAME);
#endif
    buildLabel.setJustificationType(juce::Justification::centred);
    buildLabel.setText(buildText, juce::NotificationType::dontSendNotification);

    powerOffButton.addListener(&pageListener);
    sleepButton.addListener(&pageListener);
    rebootButton.addListener(&pageListener);
    felButton.addListener(&pageListener);
    addChildComponent(overlaySpinner);
    addAndShowLayoutComponents();
}

/*
 * Turns off the display until key or mouse input is detected.
 */
void PowerPage::startSleepMode()
{
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
        removeFromStack(Layout::Transition::Type::moveRight);
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
void PowerPage::PageListener::buttonClicked(juce::Button *button)
{
    if (button == &powerPage.felButton)
    {
        powerPage.pushPageToStack(Page::Type::fel,
                Layout::Transition::Type::moveRight);
        return;
    }
    if (button == &powerPage.sleepButton)
    {
        powerPage.startSleepMode();
        return;
    }

    SystemCommands systemCommands;
    if (button == &powerPage.powerOffButton)
    {
        powerPage.showPowerSpinner();
        systemCommands.runActionCommand
            (SystemCommands::ActionCommand::shutdown);
    }
    else if (button == &powerPage.rebootButton)
    {
        powerPage.showPowerSpinner();
        systemCommands.runActionCommand
            (SystemCommands::ActionCommand::restart);
    }
}
