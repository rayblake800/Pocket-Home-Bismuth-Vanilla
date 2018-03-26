#include <stdlib.h>
#include <unistd.h>
#include "Utils.h"
#include "RemovePasswordPage.h"
#include "Password.h"

RemovePasswordPage::RemovePasswordPage(ComponentConfigFile& config) :
Localized("RemovePasswordPage"),
PageComponent(config, "RemovePasswordPage",{
    {2,
        {
            {&titleLabel, 1}
        }},
    {1,
        {
            {&rootLabel, 2},
            {&rootPassword, 3}
        }},
    {1,
        {
            {&curPwdLabel, 2},
            {&curPassword, 3}
        }},
    {1,
        {
            {&deleteButton, 1}
        }}
}),
rootLabel(config, "RootLab", localeText(root_password)),
rootPassword("Root", 0x2022),
curPwdLabel(config, "CurLabel", localeText(current_password)),
curPassword("Current", 0x2022),
titleLabel(config, "Title", localeText(remove_password))
{

#    if JUCE_DEBUG
    setName("RemovePasswordPage");
#    endif
    titleLabel.setJustificationType(Justification::centred);
    deleteButton.setButtonText(localeText(apply));
    deleteButton.addListener(this);
    addAndShowLayoutComponents();
}

/**
 * Attempts to delete the Pocket-Home password when deleteButton is pressed.
 * If this succeeds, the page will close after showing an AlertWindow.
 */
void RemovePasswordPage::pageButtonClicked(Button* button)
{
    if (button != &deleteButton)
    {
        DBG("RemovePasswordPage::" << __func__ << ": button " 
                << button->getName()
                << " should not be triggering this function!");
        clearAllFields();
        return;
    }
    if (!Password::checkPassword(curPassword.getText()))
    {
        AlertWindow::showMessageBoxAsync(
                AlertWindow::AlertIconType::WarningIcon,
                localeText(wrong_password),
                localeText(try_again),
                localeText(confirm_btn));
        clearAllFields();
        return;
    }
    String user(getlogin());
    String root_pass = rootPassword.getText();
    File passwordFile(Password::passwordPath);
    File passwordDir = passwordFile.getParentDirectory();

    //Changing the owner of the password directory so it can be deleted
    String cmd_passwd = "echo \"" + root_pass + "\" | ";
    const char* unlockCommand
            = (cmd_passwd + "sudo -kS chown -R " + user + ":" + user + " "
               + passwordFile.getParentDirectory().getFullPathName())
            .toRawUTF8();
    if (system(unlockCommand) != 0 || !passwordFile.deleteFile())
    {
        AlertWindow::showMessageBoxAsync(
                AlertWindow::AlertIconType::WarningIcon,
                localeText(wrong_password),
                localeText(cant_change_password),
                localeText(confirm_btn));
        return;
    }
    if(!passwordDir.deleteFile())
    {
        AlertWindow::showMessageBoxAsync(
                AlertWindow::AlertIconType::WarningIcon,
                localeText(error),
                localeText(cant_remove_password),
                localeText(confirm_btn));
        return;
    }
    AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::InfoIcon,
            localeText(success),
            localeText(password_removed),
            localeText(confirm_btn),
            nullptr,
            ModalCallbackFunction::create([this](int i)
            {
                removeFromStack();
            }));
    clearAllFields();
}

/**
 * Clears all TextEditor fields on the page.
 */
void RemovePasswordPage::clearAllFields()
{
    rootPassword.clear();
    curPassword.clear();
}