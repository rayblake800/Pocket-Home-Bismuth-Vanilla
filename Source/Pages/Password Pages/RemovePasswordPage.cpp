#include "RemovePasswordPage.h"
#include "Password.h"

RemovePasswordPage::RemovePasswordPage() :
Localized("RemovePasswordPage"),
PageComponent("RemovePasswordPage",{
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
curPwdLabel("CurLabel", localeText(current_password)),
curPassword("Current", 0x2022),
titleLabel("Title", localeText(remove_password))
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
    String title, message;
    switch (Password::removePassword(curPassword.getText()))
    {
        case Password::missingNewPassword:
        case Password::paswordSetSuccess:
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
                    localeText(success),
                    localeText(password_removed),
                    "",
                    nullptr,
                    ModalCallbackFunction::create([this](int i)
                    {
                        removeFromStack();
                    }));
            curPassword.clear();
            return;
        case Password::wrongPasswordError:
            title = localeText(cant_remove_password);
            message = localeText(wrong_password);
            break;
        case Password::fileDeleteFailed:
            title = localeText(cant_remove_password);
            message = localeText(check_agent_and_root);
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
}
