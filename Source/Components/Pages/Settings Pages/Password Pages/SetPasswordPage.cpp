#include "SetPasswordPage.h"
#include "Password.h"

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
PageComponent("SetPasswordPage"),
title("Title", localeText(changePasswordTextKey)),
curLabel("CurLabel", localeText(currentPasswordTextKey)),
curPassword("Current", 0x2022),
newLabel("NewLabel", localeText(newPasswordTextKey)),
newPassword("New", 0x2022),
confirmLabel("ConfLabel", localeText(retypePasswordTextKey)),
confirmPassword("Confirmation", 0x2022)
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("SetPasswordPage");
#    endif
    setBackButton(PageComponent::leftBackButton);
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
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

    title.setJustificationType(Justification::centred);
    setPassword.setButtonText(localeText(applyTextKey));
    setPassword.addListener(this);
    bool passwordSet = Password::isPasswordSet();
    addAndShowLayoutComponents();
    curLabel.setVisible(passwordSet);
    curPassword.setVisible(passwordSet);
     
    ComponentConfigFile config;
    curPassword.setFont(Font(config.getFontHeight
            (ComponentConfigFile::smallText)));
    newPassword.setFont(Font(config.getFontHeight
            (ComponentConfigFile::smallText)));
    confirmPassword.setFont(Font(config.getFontHeight
            (ComponentConfigFile::smallText)));
}

/*
 * If the setPassword button is clicked, attempts to set a new application
 * password.  The result of this operation will be displayed in a message box,
 * and all text fields on the page will be cleared.  If the password was
 * set successully, the page will be closed.
 */
void SetPasswordPage::pageButtonClicked(juce::Button* button)
{
    using namespace juce;
    if (button != &setPassword)
    {
        DBG("SetPasswordPage::" << __func__ << ": button " << button->getName()
                << " should not be triggering this function!");
        clearAllFields();
        return;
    }
    else if (newPassword.getText() != confirmPassword.getText())
    {
        showErrorMessage(localeText(confirmationFailedTextKey),
                localeText(fieldsDontMatchTextKey));
    }
    else
    {
        String title, message;
        switch (Password::changePassword(curPassword.getText(),
                newPassword.getText()))
        {
            case Password::passwordRemoveSuccess:
            case Password::fileDeleteFailed:
                DBG("SetPasswordPage::" << __func__
                        << ": Illegal result returned!");
                jassertfalse;
                return;
            case Password::paswordSetSuccess:
                DBG("SetPasswordPage::" << __func__ << ": paswordSetSuccess");
                AlertWindow::showMessageBoxAsync(
                        AlertWindow::AlertIconType::InfoIcon,
                        localeText(successTextKey),
                        localeText(passwordUpdatedTextKey),
                        "",
                        nullptr,
                        ModalCallbackFunction::create([this](int i)
                        {
                            removeFromStack();
                        }));
                clearAllFields();
                return;
            case Password::missingNewPassword:
                DBG("SetPasswordPage::" << __func__ << ": missingNewPassword");
                title = localeText(missingPasswordTextKey);
                message = localeText(askToEnterNewTextKey);
                break;
            case Password::wrongPasswordError:
                DBG("SetPasswordPage::" << __func__ << ": wrongPasswordError");
                title = localeText(Password::isPasswordSet() ?
                        failedSetTextKey : failedUpdateTextKey);
                message = localeText(wrongPasswordTextKey);
                break;
            case Password::fileCreateFailed:
                DBG("SetPasswordPage::" << __func__ << ": fileCreateFailed");
                title = localeText(failedSetTextKey);
                message = localeText(checkAgentAndRootTextKey);
                break;
            case Password::fileWriteFailed:
                DBG("SetPasswordPage::" << __func__ << ": fileWriteFailed");
                title = localeText(failedUpdateTextKey);
                message = localeText(checkAgentAndRootTextKey);
                break;
            case Password::fileSecureFailed:
                DBG("SetPasswordPage::" << __func__ << ": fileSecureFailed");
                title = localeText(errorTextKey);
                message = localeText(securingFailedTextKey);
                break;
            case Password::noPasswordScript:
                DBG("SetPasswordPage::" << __func__ << ": noPasswordScript");
                title = localeText(errorTextKey);
                message = localeText(filesMissingTextKey);
                break;
            case Password::noPKExec:
                DBG("SetPasswordPage::" << __func__ << ": noPKExec");
                title = localeText(errorTextKey);
                message = localeText(polkitMissingTextKey);
                break;
        }
        AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
                title, message, "", nullptr);
        clearAllFields();
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
