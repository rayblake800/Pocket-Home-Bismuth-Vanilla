#pragma once
#include "JuceHeader.h"
#include "Locale/TextUser.h"
#include "ScalingLabel.h"
#include "PageComponent.h"

/**
 * @file RemovePasswordPage.h
 * 
 * This page allows the user to remove the Pocket-Home password.  To do so, the
 * user must enter in the existing Pocket-Home password, along with their root
 * password.
 */

class RemovePasswordPage : public PageComponent, public Locale::TextUser
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
    void pageButtonClicked(juce::Button* button) override;

    //Title of the page
    ScalingLabel titleLabel;

    //Text field for the current password
    ScalingLabel curPwdLabel;
    juce::TextEditor curPassword;

    //Triggers the attempted password removal
    juce::TextButton deleteButton;

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RemovePasswordPage)
};
