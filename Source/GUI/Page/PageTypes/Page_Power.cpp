#include "Page_Power.h"
#include "Page_Type.h"
#include "Layout_Transition_Animator.h"
#include "SystemCommands.h"
#include "PocketHomeWindow.h"
#include "Utils.h"
#include <map>


/* Localized object class key: */
static const juce::Identifier localeClassKey = "Page::Power";

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

Page::Power::Power() : Locale::TextUser(localeClassKey),
pageListener(*this),
powerOffButton(localeText(TextKey::shutdown)),
rebootButton(localeText(TextKey::reboot)),
sleepButton(localeText(TextKey::sleep)),
felButton(localeText(TextKey::flashSoftware)),
versionLabel(localeText(TextKey::version) 
        + juce::JUCEApplication::getInstance()->getApplicationVersion())
{
#ifdef JUCE_DEBUG
    setName("Page::Power");
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
void Page::Power::startSleepMode()
{
    SystemCommands systemCommands;
    if(systemCommands.runIntCommand(SystemCommands::IntCommand::sleepCheck)
            == 0)
    {
        systemCommands.runActionCommand(SystemCommands::ActionCommand::wake);
    }
    else
    {
        PocketHomeWindow* window = PocketHomeWindow::getOpenWindow();
        jassert(window != nullptr);
        window->showLoginScreen();
        systemCommands.runActionCommand(SystemCommands::ActionCommand::sleep);
    }
}

/*
 * Shows the power spinner to indicate to the user that the system is
 * restarting or shutting down.
 */
void Page::Power::showPowerSpinner()
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
void Page::Power::pageResized()
{
    overlaySpinner.setBounds(getLocalBounds());
}

/*
 * Handles all button clicks.
 */
void Page::Power::PageListener::buttonClicked(juce::Button *button)
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
