/**
 * @file SetPasswordPage.h
 * 
 * Allows the user to set or change a password for this application.  Honestly,
 * this isn't really much of a security measure, but it's enough to keep the
 * average person from using this particular application.
 */
#pragma once
#include "ScalingLabel.h"
#include "PageComponent.h"
#include "Localized.h"
#include "JuceHeader.h"

class SetPasswordPage : public PageComponent, private Localized
{
public:
    SetPasswordPage(ComponentConfigFile& config);

    virtual ~SetPasswordPage() { }

private:
    /**
     * If the setPassword button is clicked, attempts to set a new application
     * password.  The result of this operation will be displayed in a message box,
     * and all text fields on the page will be cleared.  If the password was
     * set successfully, the page will be closed.
     * 
     * @param button  Should only ever be the setPassword button.
     */
    void pageButtonClicked(Button* button) override;

    /**
     * Opens a message box to display an error message, and clears all text entry
     * fields on the page.
     * 
     * @param title  The title to print on the message box.
     * 
     * @param error  The error message to print in the message box.
     */
    void showErrorMessage(String title, String error);

    /**
     * Clears the text in all text entry fields on the page.
     */
    void clearAllFields();

    //Page title
    ScalingLabel title;
    
    //Text field for the current password (if there is one)
    ScalingLabel curLabel;
    TextEditor curPassword;

    //Text field for the new password
    ScalingLabel newLabel;
    TextEditor newPassword;

    //Text field for the confirmation of the new password
    ScalingLabel confirmLabel;
    TextEditor confirmPassword;

    //Button for applying the settings
    TextButton setPassword;
    
        
    //localized text keys;
    static const constexpr char * change_password = "change_password";
    static const constexpr char * current_password = "current_password";
    static const constexpr char * new_password = "new_password";
    static const constexpr char * retype_password = "retype_password";
    static const constexpr char * apply = "apply";
    
    static const constexpr char * confirmation_failed = "confirmation_failed";
    static const constexpr char * fields_dont_match = "fields_dont_match";
    
    static const constexpr char * success = "success";
    static const constexpr char * password_updated = "password_updated";
    
    static const constexpr char * missing_password = "missing_password";
    static const constexpr char * ask_to_enter_new = "ask_to_enter_new";
    
    static const constexpr char * failed_set = "failed_set";
    static const constexpr char * failed_update = "failed_update";
    static const constexpr char * wrong_password = "wrong_password";
    static const constexpr char * check_agent_and_root = "check_agent_and_root";
    
    static const constexpr char * error = "error";
    static const constexpr char * securing_failed = "securing_failed";
    static const constexpr char * files_missing = "files_missing";
    static const constexpr char * polkit_missing = "polkit_missing";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SetPasswordPage)
};
