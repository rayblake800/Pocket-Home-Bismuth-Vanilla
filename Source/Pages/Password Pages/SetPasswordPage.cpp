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
PageComponent(config, "SetPasswordPage",{
    {2,
        {
            {&title, 1}
        }},
    {1,
        {
            {&rootLabel, 1},
            {&rootPassword, 3}
        }},
    {1,
        {
            {&curLabel, 1},
            {&curPassword, 3}
        }},
    {1,
        {
            {&newLabel, 1},
            {&newPassword, 3}
        }},
    {1,
        {
            {&confirmLabel, 1},
            {&confirmPassword, 3}
        }},
    {1,
        {
            {&setPassword, 1}
        }}
}),
rootLabel(config, "RootLab", "Sudo password"),
rootPassword("Root", 0x2022),
curLabel(config, "CurLabel", "Current password"),
curPassword("Current", 0x2022),
newLabel(config, "NewLabel", "New password"),
newPassword("New", 0x2022),
confirmLabel(config, "ConfLabel", "Retype password"),
confirmPassword("Confirmation", 0x2022),
title(config, "Title", "Change your password")
{

#    if JUCE_DEBUG
    setName("SetPasswordPage");
#    endif
    title.setJustificationType(Justification::centred);
    setPassword.setButtonText("Apply");
    setPassword.addListener(this);
    addAndShowLayoutComponents();
}

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
        showErrorMessage("Missing password",
                "Please enter a new password.");
    }
    else if (newPassword.getText() != confirmPassword.getText())
    {
        showErrorMessage("Password confirmation failed",
                "The new password fields don't match, try again.");
    }
    else if (!Password::checkPassword(
             Password::hashString(curPassword.getText())))
    {
        showErrorMessage("Wrong password",
                "The existing Pocket-Home password was incorrect.");
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
                showErrorMessage("Wrong password",
                        "Can't update your password,\n make sure your root password is correct.");
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
            showErrorMessage("Error",
                    "Failed to lock the password directory!");
            passwordFile.deleteFile();
            return;
        }
        AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::InfoIcon,
                "Success",
                "The Pocket-Home password has been updated.",
                "Ok",
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
 * 
 * @param title  The title to print on the message box.
 * 
 * @param error  The error message to print in the message box.
 */
void SetPasswordPage::showErrorMessage(String title, String error)
{

    AlertWindow::showMessageBoxAsync
            (AlertWindow::AlertIconType::WarningIcon, title, error, "Ok");
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
