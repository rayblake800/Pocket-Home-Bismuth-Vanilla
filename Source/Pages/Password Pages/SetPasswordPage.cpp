#include <openssl/sha.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "Utils.h"
#include "SetPasswordPage.h"
#include "Password.h"

SetPasswordPage::SetPasswordPage(ComponentConfigFile& config) :
Localized("SetPasswordPage"),
PageComponent(config, "SetPasswordPage",{
    {2,
        {
            {&title, 1}
        }},
    {1,
        {
            {&curLabel, 2},
            {&curPassword, 3}
        }},
    {1,
        {
            {&newLabel, 2},
            {&newPassword, 3}
        }},
    {1,
        {
            {&confirmLabel, 2},
            {&confirmPassword, 3}
        }},
    {1,
        {
            {&setPassword, 1}
        }}
}),
title(config, "Title", localeText(change_password)),
curLabel(config, "CurLabel", localeText(current_password)),
curPassword("Current", 0x2022),
newLabel(config, "NewLabel", localeText(new_password)),
newPassword("New", 0x2022),
confirmLabel(config, "ConfLabel", localeText(retype_password)),
confirmPassword("Confirmation", 0x2022)
{

#    if JUCE_DEBUG
    setName("SetPasswordPage");
#    endif
    title.setJustificationType(Justification::centred);
    setPassword.setButtonText(localeText(apply));
    setPassword.addListener(this);
    bool passwordSet = Password::isPasswordSet();
    curLabel.setVisible(passwordSet);
    curPassword.setVisible(passwordSet);
    addAndShowLayoutComponents();
}

/**
 * If the setPassword button is clicked, attempts to set a new application
 * password.  The result of this operation will be displayed in a message box,
 * and all text fields on the page will be cleared.  If the password was
 * set successfully, the page will be closed.
 */
void SetPasswordPage::pageButtonClicked(Button* button)
{
    if (button != &setPassword)
    {
        DBG("SetPasswordPage::" << __func__ << ": button " << button->getName()
                << " should not be triggering this function!");
        clearAllFields();
        return;
    }
    else if (newPassword.getText() != confirmPassword.getText())
    {
        showErrorMessage(localeText(confirmation_failed),
                localeText(fields_dont_match));
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
                AlertWindow::showMessageBoxAsync(
                        AlertWindow::AlertIconType::InfoIcon,
                        localeText(success),
                        localeText(password_updated),
                        "",
                        nullptr,
                        ModalCallbackFunction::create([this](int i)
                        {
                            removeFromStack();
                        }));
                clearAllFields();
                return;
            case Password::missingNewPassword:
                title = localeText(missing_password);
                message = localeText(ask_to_enter_new);
                break;
            case Password::wrongPasswordError:
                title = localeText(Password::isPasswordSet() ? 
                    failed_set : failed_update);
                message = localeText(wrong_password);
                break;
            case Password::fileCreateFailed:
                title = localeText(failed_set);
                message = localeText(check_agent_and_root);
                break;
            case Password::fileWriteFailed:
                title = localeText(failed_update);
                message = localeText(check_agent_and_root);
                break;
            case Password::fileSecureFailed:
                title = localeText(error);
                message = localeText(securing_failed);
                break;
            case Password::noPasswordScript:
                title = localeText(error);
                message = localeText(files_missing);
                break;
            case Password::noPKExec:
                title = localeText(error);
                message = localeText(polkit_missing);
                break;
        }
        AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
                title, message, "", nullptr);
        clearAllFields();
    }
}

/**
 * Opens a message box to display an error message, and clears all text entry
 * fields on the page.
 */
void SetPasswordPage::showErrorMessage(String title, String error)
{

    AlertWindow::showMessageBoxAsync
            (AlertWindow::AlertIconType::WarningIcon, title, error, "");
    clearAllFields();
}

/**
 * Clears the text in all text entry fields on the page.
 */
void SetPasswordPage::clearAllFields()
{
    curPassword.clear();
    newPassword.clear();
    confirmPassword.clear();
}
