#pragma once
/**
 * @file SetPasswordPage.h
 * 
 * @brief  Allows the user to set or change a password for this application.
 */
#include "Widgets_BoundedLabel.h"
#include "Page_Component.h"
#include "Locale_TextUser.h"
#include "JuceHeader.h"

class SetPasswordPage : public Page::Component, public Locale::TextUser
{
public:
    SetPasswordPage();

    virtual ~SetPasswordPage() { }

private:
    /**
     * Opens a message box to display an error message, and clears all text entry
     * fields on the page.
     * 
     * @param title  The title to print on the message box.
     * 
     * @param error  The error message to print in the message box.
     */
    void showErrorMessage(juce::String title, juce::String error);

    /**
     * Clears the text in all text entry fields on the page.
     */
    void clearAllFields();

    class PageListener : public juce::Button::Listener
    {
    public:
        PageListener(SetPasswordPage& passwordPage) 
                : passwordPage(passwordPage) { }

    private:
        /**
         * If the setPassword button is clicked, attempts to set a new 
         * application password. The result of this operation will be displayed
         * in a message box, and all text fields on the page will be cleared.
         * If the password was set successfully, the page will be closed.
         * 
         * @param button  Should only ever be the setPassword button.
         */
        void buttonClicked(juce::Button* button) override;

        SetPasswordPage& passwordPage;
    };
    PageListener pageListener;

    //Page title
    Widgets::BoundedLabel title;
    
    //Text field for the current password (if there is one)
    Widgets::BoundedLabel curLabel;
    juce::TextEditor curPassword;

    //Text field for the new password
    Widgets::BoundedLabel newLabel;
    juce::TextEditor newPassword;

    //Text field for the confirmation of the new password
    Widgets::BoundedLabel confirmLabel;
    juce::TextEditor confirmPassword;

    //Button for applying the settings
    juce::TextButton setPassword;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SetPasswordPage)
};
