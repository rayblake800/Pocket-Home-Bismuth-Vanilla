#pragma once
#include "JuceHeader.h"
#include "Locale/TextUser.h"
#include "Widgets_BoundedLabel.h"
#include "Page_Component.h"

/**
 * @file RemovePasswordPage.h
 * 
 * This page allows the user to remove the Pocket-Home password.  To do so, the
 * user must enter in the existing Pocket-Home password, along with their root
 * password.
 */

class RemovePasswordPage : public Page::Component, public Locale::TextUser
{
public:
    RemovePasswordPage();

    virtual ~RemovePasswordPage() { }

private:

    class PageListener : public juce::Button::Listener
    {
    public:
        PageListener(RemovePasswordPage& passwordPage) 
                : passwordPage(passwordPage) { }

    private:
        /**
         * Attempts to delete the Pocket-Home password when deleteButton is pressed.
         * If this succeeds, the page will close after showing an AlertWindow.
         * 
         * @param button  This should always be deleteButton
         */
        void buttonClicked(juce::Button* button) override;

        RemovePasswordPage& passwordPage;
    };
    PageListener pageListener;

    //Title of the page
    Widgets::BoundedLabel titleLabel;

    //Text field for the current password
    Widgets::BoundedLabel curPwdLabel;
    juce::TextEditor curPassword;

    //Triggers the attempted password removal
    juce::TextButton deleteButton;

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RemovePasswordPage)
};
