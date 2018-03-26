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
            {&curPwdLabel, 2},
            {&curPassword, 3}
        }},
    {1,
        {
            {&deleteButton, 1}
        }}
}),
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
        curPassword.clear();
        return;
    }
    if (!Password::checkPassword(curPassword.getText()))
    {
        AlertWindow::showMessageBoxAsync(
                AlertWindow::AlertIconType::WarningIcon,
                localeText(wrong_password),
                localeText(try_again),
                localeText(confirm_btn));
        curPassword.clear();
        return;
    }

    if (!Password::changePassword(curPassword.getText(),""));
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
    curPassword.clear();
}
