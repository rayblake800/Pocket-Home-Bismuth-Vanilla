#include "RemovePasswordPage.h"
#include "Password.h"
#include "Layout_Component_ConfigFile.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "RemovePasswordPage";

/* Localized text value keys: */
static const juce::Identifier currentPasswordTextKey    = "currentPassword";
static const juce::Identifier removePasswordTextKey     = "removePassword";
static const juce::Identifier applyTextKey              = "apply";

static const juce::Identifier successTextKey            = "success";
static const juce::Identifier passwordRemovedTextKey    = "passwordRemoved";

static const juce::Identifier cantRemovePasswordTextKey = "cantRemovePassword";
static const juce::Identifier wrongPasswordTextKey      = "wrongPassword";
static const juce::Identifier checkAgentTextKey         = "checkAgent";

static const juce::Identifier errorTextKey              = "error";
static const juce::Identifier filesMissingTextKey       = "filesMissing";
static const juce::Identifier polkitMissingTextKey      = "polkitMissing";
    
RemovePasswordPage::RemovePasswordPage() :
Locale::TextUser(localeClassKey),
pageListener(*this),
curPwdLabel("CurLabel", localeText(currentPasswordTextKey)),
curPassword("Current", 0x2022),
titleLabel("Title", localeText(removePasswordTextKey))
{
#    if JUCE_DEBUG
    setName("RemovePasswordPage");
#    endif
    setBackButton(BackButtonType::left);

    using namespace Layout::Group;
    RelativeLayout layout({
        Row(20, { RowItem(&titleLabel) }),
        Row(10, {
                    RowItem(&curPwdLabel, 20),
                    RowItem(&curPassword, 30)
                }),
        Row(10, { RowItem(&deleteButton, 10) })
    });
    layout.setXMarginFraction(0.1);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(1);
    setLayout(layout);
    
    Layout::Component::ConfigFile config;
    curPassword.setFont(juce::Font(config.getFontHeight
            (Layout::Component::TextSize::smallText)));
    
    titleLabel.setJustificationType(juce::Justification::centred);
    deleteButton.setButtonText(localeText(applyTextKey));
    deleteButton.addListener(&pageListener);
    addAndShowLayoutComponents();
}

/*
 * Attempts to delete the Pocket-Home password when deleteButton is pressed.
 * If this succeeds, the page will close after showing an AlertWindow.
 */
void RemovePasswordPage::PageListener::buttonClicked(juce::Button* button)
{
    using juce::AlertWindow;
    if (button != &passwordPage.deleteButton)
    {
        DBG("RemovePasswordPage::" << __func__ << ": button "
                << button->getName()
                << " should not be triggering this function!");
        passwordPage.curPassword.clear();
        return;
    }
    juce::String title, message;
    switch (Password::removePassword(passwordPage.curPassword.getText()))
    {
        case Password::missingNewPassword:
        case Password::passwordSetSuccess:
        case Password::fileCreateFailed:
        case Password::fileWriteFailed:
        case Password::fileSecureFailed:
            DBG("RemovePasswordPage::" << __func__
                    << ": Illegal result returned!");
            jassertfalse;
            return;
        case Password::passwordRemoveSuccess:
            AlertWindow::showMessageBoxAsync(
                    AlertWindow::AlertIconType::InfoIcon,
                    passwordPage.localeText(successTextKey),
                    passwordPage.localeText(passwordRemovedTextKey),
                    "",
                    nullptr,
                    juce::ModalCallbackFunction::create([this](int i)
                    {
                        passwordPage.removeFromStack();
                    }));
            passwordPage.curPassword.clear();
            return;
        case Password::wrongPasswordError:
            title = passwordPage.localeText(cantRemovePasswordTextKey);
            message = passwordPage.localeText(wrongPasswordTextKey);
            break;
        case Password::fileDeleteFailed:
            title = passwordPage.localeText(cantRemovePasswordTextKey);
            message = passwordPage.localeText(checkAgentTextKey);
            break;
        case Password::noPasswordScript:
            title = passwordPage.localeText(errorTextKey);
            message = passwordPage.localeText(filesMissingTextKey);
            break;
        case Password::noPKExec:
            title = passwordPage.localeText(errorTextKey);
            message = passwordPage.localeText(polkitMissingTextKey);
            break;
    }
    AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
            title, message, "", nullptr);
}
