#pragma once
#include "JuceHeader.h"
#include "Localized.h"
#include "ScalingLabel.h"
#include "PageComponent.h"

/**
 * @file RemovePasswordPage.h
 * 
 * This page allows the user to remove the Pocket-Home password.  To do so, the
 * user must enter in the existing Pocket-Home password, along with their root
 * password.
 */

class RemovePasswordPage : public PageComponent, private Localized
{
public:
    RemovePasswordPage();

    virtual ~RemovePasswordPage() { }

private:

    /**
     * Attempts to delete the Pocket-Home password when deleteButton is pressed.
     * If this succeeds, the page will close after showing an AlertWindow.
     * 
     * @param button  This should always be deleteButton
     */
    void pageButtonClicked(Button* button) override;

    //Title of the page
    ScalingLabel titleLabel;

    //Text field for the current password
    ScalingLabel curPwdLabel;
    TextEditor curPassword;

    //Triggers the attempted password removal
    TextButton deleteButton;


    //localized text keys;
    static const constexpr char * current_password = "current_password";
    static const constexpr char * remove_password = "remove_password";
    static const constexpr char * apply = "apply";
    
    static const constexpr char * success = "success";
    static const constexpr char * password_removed = "password_removed";
    
    static const constexpr char * cant_remove_password = "cant_remove_password";
    static const constexpr char * wrong_password = "wrong_password";
    static const constexpr char * check_agent_and_root = "check_agent_and_root";
    
    static const constexpr char * error = "error";
    static const constexpr char * files_missing = "files_missing";
    static const constexpr char * polkit_missing = "polkit_missing";
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RemovePasswordPage)
};
