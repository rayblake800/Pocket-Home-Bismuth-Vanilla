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
            {&rootLabel, 2},
            {&rootPassword, 3}
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
rootLabel(config, "RootLab", localeText(root_password)),
rootPassword("Root", 0x2022),
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
    if (newPassword.isEmpty())
    {
        showErrorMessage(localeText(missing_password),
                localeText(ask_to_enter_new));
    }
    else if (newPassword.getText() != confirmPassword.getText())
    {
        showErrorMessage(localeText(confirmation_failed),
                localeText(fields_dont_match));
    }
    else if (!Password::checkPassword(curPassword.getText()))
    {
        showErrorMessage(localeText(wrong_password),
                localeText(incorrect_app_password));
    }
    else
    {
        File passwordFile(Password::passwordPath);
        File passwordDir = passwordFile.getParentDirectory();
        String cmd_passwd = "echo \"" + rootPassword.getText() + "\" |";
        if (passwordFile.existsAsFile())
        {
            //Changing the owner of the password file (to be able to write inside)
            String user(getlogin());


            const char* unlockPwdDir
                    = (cmd_passwd + "sudo -kS chown -R " + user + ":" + user
                       + " " + passwordDir.getFullPathName()).toRawUTF8();

            if (system(unlockPwdDir) != 0)
            {
                showErrorMessage(localeText(wrong_password),
                        localeText(cant_update) + "\n"
                        + localeText(check_root_password));
                return;
            }
        }
        else
        {
            passwordFile.create();
        }
        passwordFile.replaceWithText(
                Password::hashString(newPassword.getText()), false);

        //After writing, we put back the owner (root)
        const char* lockPwdDir
                = (cmd_passwd + "sudo -kS chown -R root:root "
                   + passwordDir.getFullPathName()).toRawUTF8();

        if (system(lockPwdDir) != 0 || passwordDir.hasWriteAccess())
        {
            showErrorMessage(localeText(error),
                    localeText(cant_lock_dir) + "\n"
                    + localeText(password_removed));
            passwordFile.deleteFile();
            return;
        }
        AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::InfoIcon,
                localeText(success),
                localeText(password_updated),
                localeText(confirm_btn),
                nullptr,
                ModalCallbackFunction::create([this](int i)
                {
                    removeFromStack();
                }));

    }
}

/**
 * Opens a message box to display an error message, and clears all text entry
 * fields on the page.
 */
void SetPasswordPage::showErrorMessage(String title, String error)
{

    AlertWindow::showMessageBoxAsync
            (AlertWindow::AlertIconType::WarningIcon, title, error,
            localeText(confirm_btn));
    clearAllFields();
}

/**
 * Clears the text in all text entry fields on the page.
 */
void SetPasswordPage::clearAllFields()
{
    rootPassword.clear();
    curPassword.clear();
    newPassword.clear();
    confirmPassword.clear();
}
