#include "SetPasswordPage.h"
#include "Password.h"

SetPasswordPage::SetPasswordPage() :
Localized("SetPasswordPage"),
PageComponent("SetPasswordPage"),
title("Title", localeText(change_password)),
curLabel("CurLabel", localeText(current_password)),
curPassword("Current", 0x2022),
newLabel("NewLabel", localeText(new_password)),
newPassword("New", 0x2022),
confirmLabel("ConfLabel", localeText(retype_password)),
confirmPassword("Confirmation", 0x2022)
{

#    if JUCE_DEBUG
    setName("SetPasswordPage");
#    endif
    
    using RowItem = RelativeLayoutManager::ComponentLayout;
    RelativeLayoutManager::Layout layout({
        {
	    .weight = 20, .rowItems = { RowItem(&title) }
        },
        {
            .weight = 10, .rowItems = 
            {
                RowItem(&curLabel, 20),
                RowItem(&curPassword, 30)
            }
	},
        {
            .weight = 10, .rowItems = 
            {
                RowItem(&newLabel, 20),
                RowItem(&newPassword, 30)
            }
	},
        {
            .weight = 10, .rowItems = 
            {
                RowItem(&confirmLabel, 20),
                RowItem(&confirmPassword, 30)
            }
	},
        {
            .weight = 10, .rowItems = { RowItem(&setPassword, 10) }
	}
    });
    layout.setXMarginFraction(0.1);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(1);
    setLayout(layout);
    
    title.setJustificationType(Justification::centred);
    setPassword.setButtonText(localeText(apply));
    setPassword.addListener(this);
    bool passwordSet = Password::isPasswordSet();
    addAndShowLayoutComponents();
    curLabel.setVisible(passwordSet);
    curPassword.setVisible(passwordSet);
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
                DBG("SetPasswordPage::" << __func__ << ": paswordSetSuccess");
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
                DBG("SetPasswordPage::" << __func__ << ": missingNewPassword");
                title = localeText(missing_password);
                message = localeText(ask_to_enter_new);
                break;
            case Password::wrongPasswordError:           
                DBG("SetPasswordPage::" << __func__ << ": wrongPasswordError");
                title = localeText(Password::isPasswordSet() ? 
                    failed_set : failed_update);
                message = localeText(wrong_password);
                break;
            case Password::fileCreateFailed:           
                DBG("SetPasswordPage::" << __func__ << ": fileCreateFailed");
                title = localeText(failed_set);
                message = localeText(check_agent_and_root);
                break;
            case Password::fileWriteFailed:           
                DBG("SetPasswordPage::" << __func__ << ": fileWriteFailed");
                title = localeText(failed_update);
                message = localeText(check_agent_and_root);
                break;
            case Password::fileSecureFailed:           
                DBG("SetPasswordPage::" << __func__ << ": fileSecureFailed");
                title = localeText(error);
                message = localeText(securing_failed);
                break;
            case Password::noPasswordScript:           
                DBG("SetPasswordPage::" << __func__ << ": noPasswordScript");
                title = localeText(error);
                message = localeText(files_missing);
                break;
            case Password::noPKExec:           
                DBG("SetPasswordPage::" << __func__ << ": noPKExec");
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
