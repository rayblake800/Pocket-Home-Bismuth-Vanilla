#include "Password_RemovalController.h"
#include "Password.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Password::RemovalController::";
#endif

// Localized object class key:
static const juce::Identifier localeClassKey = "Password::RemovalController";

// Character used to mask current password input:
static const constexpr juce::juce_wchar passwordChar = 0x2022;

// Localized text value keys:
namespace TextKey
{
    static const juce::Identifier success            = "success";
    static const juce::Identifier passwordRemoved    = "passwordRemoved";

    static const juce::Identifier cantRemovePassword = "cantRemovePassword";
    static const juce::Identifier wrongPassword      = "wrongPassword";
    static const juce::Identifier checkAgent         = "checkAgent";

    static const juce::Identifier error              = "error";
    static const juce::Identifier filesMissing       = "filesMissing";
    static const juce::Identifier polkitMissing      = "polkitMissing";
}


// Connects the controller to the UI components it will control, and sets the
// action it should take after the password is removed.
Password::RemovalController::RemovalController(
        juce::TextEditor& passwordField,
        juce::Button& removerButton,
        const std::function<void()> onRemove) :
Locale::TextUser(localeClassKey),
passwordField(passwordField),
removerButton(removerButton),
onRemoveCallback(onRemove)
{
    removerButton.addListener(this);
    passwordField.setPasswordCharacter(passwordChar);
}


// Attempts to delete the Pocket-Home password when the remove button is
// pressed.
void Password::RemovalController::buttonClicked(juce::Button* button)
{
    using juce::AlertWindow;
    jassert(button == &removerButton);
    juce::String title, message;
    const ChangeResult result = removePassword(passwordField.getText());
    passwordField.clear();
    switch(result)
    {
        case missingNewPassword:
        case passwordSetSuccess:
        case fileCreateFailed:
        case fileWriteFailed:
        case fileSecureFailed:
            DBG(dbgPrefix << __func__ << ": Illegal result returned!");
            jassertfalse;
            return;
        case passwordRemoveSuccess:
            AlertWindow::showMessageBoxAsync(
                    AlertWindow::AlertIconType::InfoIcon,
                    localeText(TextKey::success),
                    localeText(TextKey::passwordRemoved),
                    "",
                    nullptr,
                    juce::ModalCallbackFunction::create([this](int i)
                    {
                        if (onRemoveCallback)
                        {
                            onRemoveCallback();
                        }
                    }));
            return;
        case wrongPasswordError:
            title = localeText(TextKey::cantRemovePassword);
            message = localeText(TextKey::wrongPassword);
            break;
        case fileDeleteFailed:
            title = localeText(TextKey::cantRemovePassword);
            message = localeText(TextKey::checkAgent);
            break;
        case noPasswordScript:
            title = localeText(TextKey::error);
            message = localeText(TextKey::filesMissing);
            break;
        case noPKExec:
            title = localeText(TextKey::error);
            message = localeText(TextKey::polkitMissing);
            break;
    }
    AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
            title, message, "", nullptr);
}
