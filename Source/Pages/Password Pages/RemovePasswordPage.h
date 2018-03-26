/**
 * @file RemovePasswordPage.h
 * 
 * This page allows the user to remove the Pocket-Home password.  To do so, the
 * user must enter in the existing Pocket-Home password, along with their root
 * password.
 */
#pragma once
#include "JuceHeader.h"
#include "Localized.h"
#include "ScalingLabel.h"
#include "PageComponent.h"

class RemovePasswordPage : public PageComponent, private Localized
{
public:
    RemovePasswordPage(ComponentConfigFile& config);

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
    static const constexpr char * root_password = "root_password";
    static const constexpr char * current_password = "current_password";
    static const constexpr char * remove_password = "remove_password";
    static const constexpr char * apply = "apply";
    static const constexpr char * wrong_password = "wrong_password";
    static const constexpr char * try_again = "try_again";
    static const constexpr char * confirm_btn = "confirm_btn";
    static const constexpr char * cant_change_password = "cant_change_password";
    static const constexpr char * error = "error";
    static const constexpr char * cant_remove_password = "cant_remove_password";
    static const constexpr char * success = "success";
    static const constexpr char * password_removed = "password_removed";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RemovePasswordPage)
};
