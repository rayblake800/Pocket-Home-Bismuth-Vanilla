#include "Password_UpdateController.h"
#include "Layout_Component_ConfigFile.h"
#include "Layout_Component_TextSize.h"
#include "Password.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Password::UpdateController::";
#endif

/* Localized object class key: */
static const juce::Identifier localeClassKey = "Password::UpdateController";

/* Localized text value keys: */
namespace TextKey
{
    static const juce::Identifier confirmationFailed = "confirmationFailed";
    static const juce::Identifier fieldsDontMatch    = "fieldsDontMatch";

    static const juce::Identifier success            = "success";
    static const juce::Identifier passwordUpdated    = "passwordUpdated";

    static const juce::Identifier missingPassword    = "missingPassword";
    static const juce::Identifier askToEnterNew      = "askToEnterNew";

    static const juce::Identifier failedSet          = "failedSet";
    static const juce::Identifier failedUpdate       = "failedUpdate";
    static const juce::Identifier wrongPassword      = "wrongPassword";
    static const juce::Identifier checkAgentAndRoot  = "checkAgentAndRoot";

    static const juce::Identifier error              = "error";
    static const juce::Identifier securingFailed     = "securingFailed";
    static const juce::Identifier filesMissing       = "filesMissing";
    static const juce::Identifier polkitMissing      = "polkitMissing";
}

static const constexpr juce::juce_wchar passwordChar = 0x2022;

/*
 * Initialize all editor components, configuring them based on whether the
 * application password is set.
 */
Password::UpdateController::UpdateController(
        juce::TextEditor& currentPasswordField,
        juce::TextEditor& newPasswordField,
        juce::TextEditor& confirmPasswordField,
        juce::Button& updateButton,
        std::function<void()> onUpdate) :
Locale::TextUser(localeClassKey),
currentPasswordField(currentPasswordField),
newPasswordField(newPasswordField),
confirmPasswordField(confirmPasswordField),
updateButton(updateButton),
passwordChangeCallback(onUpdate)
{
    currentPasswordField.setPasswordCharacter(passwordChar);
    newPasswordField.setPasswordCharacter(passwordChar);
    confirmPasswordField.setPasswordCharacter(passwordChar);
    currentPasswordField.setEnabled(isPasswordSet());
    updateButton.addListener(this);
}

/*
 * Attempts to set a new password when the update button is clicked.
 */
void Password::UpdateController::buttonClicked(juce::Button* button)
{
    // Only the updateButton should trigger this method
    jassert(button == &updateButton);
    if(newPasswordField.getText() != confirmPasswordField.getText())
    {
        showErrorMessage(
                localeText(TextKey::confirmationFailed),
                localeText(TextKey::fieldsDontMatch));
    }
    else
    {
        juce::String title, message;
        const ChangeResult changeResult = changePassword(
                currentPasswordField.getText(),
                newPasswordField.getText());
        clearAllFields();
        switch(changeResult)
        {
            case Password::passwordRemoveSuccess:
            case Password::fileDeleteFailed:
                DBG(dbgPrefix << __func__ << ": fileDeleteFailed");
                jassertfalse;
                return;
            case Password::passwordSetSuccess:
                DBG(dbgPrefix << __func__ << ": passwordSetSuccess");
                juce::AlertWindow::showMessageBoxAsync(
                        juce::AlertWindow::AlertIconType::InfoIcon,
                        localeText(TextKey::success),
                        localeText(TextKey::passwordUpdated),
                        "",
                        nullptr,
                        juce::ModalCallbackFunction::create([this](int i)
                        {
                            if(passwordChangeCallback)
                            {
                                passwordChangeCallback();
                            }
                        }));
                return;
            case Password::missingNewPassword:
                DBG(dbgPrefix << __func__ << ": missingNewPassword");
                title = localeText(TextKey::missingPassword);
                message = localeText(TextKey::askToEnterNew);
                break;
            case Password::wrongPasswordError:
                DBG(dbgPrefix << __func__ << ": wrongPasswordError");
                title = localeText(Password::isPasswordSet() ?
                        TextKey::failedSet : TextKey::failedUpdate);
                message = localeText(TextKey::wrongPassword);
                break;
            case Password::fileCreateFailed:
                DBG(dbgPrefix << __func__ << ": fileCreateFailed");
                title = localeText(TextKey::failedSet);
                message = localeText(TextKey::checkAgentAndRoot);
                break;
            case Password::fileWriteFailed:
                DBG(dbgPrefix << __func__ << ": fileWriteFailed");
                title = localeText(TextKey::failedUpdate);
                message = localeText(TextKey::checkAgentAndRoot);
                break;
            case Password::fileSecureFailed:
                DBG(dbgPrefix << __func__ << ": fileSecureFailed");
                title = localeText(TextKey::error);
                message = localeText(TextKey::securingFailed);
                break;
            case Password::noPasswordScript:
                DBG(dbgPrefix << __func__ << ": noPasswordScript");
                title = localeText(TextKey::error);
                message = localeText(TextKey::filesMissing);
                break;
            case Password::noPKExec:
                DBG(dbgPrefix << __func__ << ": noPKExec");
                title = localeText(TextKey::error);
                message = localeText(TextKey::polkitMissing);
                break;
        }
        juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::AlertIconType::WarningIcon,
                title,
                message,
                "",
                nullptr);
    }
}


/*
 * Opens a message box to display an error message, and clears all text entry
 * fields on the page.
 */
void Password::UpdateController::showErrorMessage
(const juce::String title, const juce::String error)
{
    juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::AlertIconType::WarningIcon,
            title,
            error,
            "");
    clearAllFields();
}

/*
 * Clears the text in all text entry fields on the page.
 */
void Password::UpdateController::clearAllFields()
{
    currentPasswordField.clear();
    newPasswordField.clear();
    confirmPasswordField.clear();
}
