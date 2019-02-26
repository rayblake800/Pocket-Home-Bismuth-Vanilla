#include "SetPasswordPage.h"
#include "Password.h"
#include "Layout_Component_ConfigFile.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "SetPasswordPage";

/* Localized text value keys: */
static const juce::Identifier changePasswordTextKey     = "changePassword";
static const juce::Identifier currentPasswordTextKey    = "currentPassword";
static const juce::Identifier newPasswordTextKey        = "newPassword";
static const juce::Identifier retypePasswordTextKey     = "retypePassword";
static const juce::Identifier applyTextKey              = "apply";

static const juce::Identifier confirmationFailedTextKey = "confirmationFailed";
static const juce::Identifier fieldsDontMatchTextKey    = "fieldsDontMatch";

static const juce::Identifier successTextKey            = "success";
static const juce::Identifier passwordUpdatedTextKey    = "passwordUpdated";

static const juce::Identifier missingPasswordTextKey    = "missingPassword";
static const juce::Identifier askToEnterNewTextKey      = "askToEnterNew";

static const juce::Identifier failedSetTextKey          = "failedSet";
static const juce::Identifier failedUpdateTextKey       = "failedUpdate";
static const juce::Identifier wrongPasswordTextKey      = "wrongPassword";
static const juce::Identifier checkAgentAndRootTextKey  = "checkAgentAndRoot";

static const juce::Identifier errorTextKey              = "error";
static const juce::Identifier securingFailedTextKey     = "securingFailed";
static const juce::Identifier filesMissingTextKey       = "filesMissing";
static const juce::Identifier polkitMissingTextKey      = "polkitMissing";


SetPasswordPage::SetPasswordPage() :
Locale::TextUser(localeClassKey),
pageListener(*this),
title("Title", localeText(changePasswordTextKey)),
curLabel("CurLabel", localeText(currentPasswordTextKey)),
curPassword("Current", 0x2022),
newLabel("NewLabel", localeText(newPasswordTextKey)),
newPassword("New", 0x2022),
confirmLabel("ConfLabel", localeText(retypePasswordTextKey)),
confirmPassword("Confirmation", 0x2022)
{
    using juce::Font;
#    if JUCE_DEBUG
    setName("SetPasswordPage");
#    endif
    setBackButton(BackButtonType::left);
    using namespace Layout::Group;
    RelativeLayout layout({
        Row(20, { RowItem(&title)}),
        Row(10,
        {
            RowItem(&curLabel, 20),
            RowItem(&curPassword, 30)
        }),

        Row(10,
        {
            RowItem(&newLabel, 20),
            RowItem(&newPassword, 30)
        }),
        Row(10,
        { RowItem(&confirmLabel, 20),
            RowItem(&confirmPassword, 30)}),
        Row(10, { RowItem(&setPassword, 10)})
    });
    layout.setYMarginFraction(0.1);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(1);
    setLayout(layout);

    title.setJustificationType(juce::Justification::centred);
    setPassword.setButtonText(localeText(applyTextKey));
    setPassword.addListener(&pageListener);
    bool passwordSet = Password::isPasswordSet();
    addAndShowLayoutComponents();
    curLabel.setVisible(passwordSet);
    curPassword.setVisible(passwordSet);
     
    Layout::Component::ConfigFile config;
    curPassword.setFont(Font(config.getFontHeight
            (Layout::Component::TextSize::smallText)));
    newPassword.setFont(Font(config.getFontHeight
            (Layout::Component::TextSize::smallText)));
    confirmPassword.setFont(Font(config.getFontHeight
            (Layout::Component::TextSize::smallText)));
}

/*
 * If the setPassword button is clicked, attempts to set a new application
 * password.  The result of this operation will be displayed in a message box,
 * and all text fields on the page will be cleared.  If the password was
 * set successfully, the page will be closed.
 */
void SetPasswordPage::PageListener::buttonClicked(juce::Button* button)
{
    if (button != &passwordPage.setPassword)
    {
        DBG("SetPasswordPage::" << __func__ << ": button " << button->getName()
                << " should not be triggering this function!");
        passwordPage.clearAllFields();
        return;
    }
    else if (passwordPage.newPassword.getText() 
            != passwordPage.confirmPassword.getText())
    {
        passwordPage.showErrorMessage(
                passwordPage.localeText(confirmationFailedTextKey),
                passwordPage.localeText(fieldsDontMatchTextKey));
    }
    else
    {
        juce::String title, message;
        switch (Password::changePassword(passwordPage.curPassword.getText(),
                passwordPage.newPassword.getText()))
        {
            case Password::passwordRemoveSuccess:
            case Password::fileDeleteFailed:
                DBG("SetPasswordPage::" << __func__
                        << ": Illegal result returned!");
                jassertfalse;
                return;
            case Password::passwordSetSuccess:
                DBG("SetPasswordPage::" << __func__ << ": passwordSetSuccess");
                juce::AlertWindow::showMessageBoxAsync(
                        juce::AlertWindow::AlertIconType::InfoIcon,
                        passwordPage.localeText(successTextKey),
                        passwordPage.localeText(passwordUpdatedTextKey),
                        "",
                        nullptr,
                        juce::ModalCallbackFunction::create([this](int i)
                        {
                            passwordPage.removeFromStack();
                        }));
                passwordPage.clearAllFields();
                return;
            case Password::missingNewPassword:
                DBG("SetPasswordPage::" << __func__ << ": missingNewPassword");
                title = passwordPage.localeText(missingPasswordTextKey);
                message = passwordPage.localeText(askToEnterNewTextKey);
                break;
            case Password::wrongPasswordError:
                DBG("SetPasswordPage::" << __func__ << ": wrongPasswordError");
                title = passwordPage.localeText(Password::isPasswordSet() ?
                        failedSetTextKey : failedUpdateTextKey);
                message = passwordPage.localeText(wrongPasswordTextKey);
                break;
            case Password::fileCreateFailed:
                DBG("SetPasswordPage::" << __func__ << ": fileCreateFailed");
                title = passwordPage.localeText(failedSetTextKey);
                message = passwordPage.localeText(checkAgentAndRootTextKey);
                break;
            case Password::fileWriteFailed:
                DBG("SetPasswordPage::" << __func__ << ": fileWriteFailed");
                title = passwordPage.localeText(failedUpdateTextKey);
                message = passwordPage.localeText(checkAgentAndRootTextKey);
                break;
            case Password::fileSecureFailed:
                DBG("SetPasswordPage::" << __func__ << ": fileSecureFailed");
                title = passwordPage.localeText(errorTextKey);
                message = passwordPage.localeText(securingFailedTextKey);
                break;
            case Password::noPasswordScript:
                DBG("SetPasswordPage::" << __func__ << ": noPasswordScript");
                title = passwordPage.localeText(errorTextKey);
                message = passwordPage.localeText(filesMissingTextKey);
                break;
            case Password::noPKExec:
                DBG("SetPasswordPage::" << __func__ << ": noPKExec");
                title = passwordPage.localeText(errorTextKey);
                message = passwordPage.localeText(polkitMissingTextKey);
                break;
        }
        juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::AlertIconType::WarningIcon,
                title,
                message,
                "",
                nullptr);
        passwordPage.clearAllFields();
    }
}

/*
 * Opens a message box to display an errorTextKey message, and clears all text 
 * entry fields on the page.
 */
void SetPasswordPage::showErrorMessage(juce::String title, juce::String errorTextKey)
{
    using namespace juce;
    AlertWindow::showMessageBoxAsync
            (AlertWindow::AlertIconType::WarningIcon, title, errorTextKey, "");
    clearAllFields();
}

/*
 * Clears the text in all text entry fields on the page.
 */
void SetPasswordPage::clearAllFields()
{
    curPassword.clear();
    newPassword.clear();
    confirmPassword.clear();
}
