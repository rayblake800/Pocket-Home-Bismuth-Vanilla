#pragma once
#include "JuceHeader.h"
#include "Locale/TextUser.h"
#include "ScalingLabel.h"
#include "DrawableImageComponent.h"
#include "PageComponent.h"

/**
 * @file LoginPage.h
 * 
 * @brief  The page shown whenever the system exits sleep mode, or when the
 *         application starts if the pocket-home password is set.
 * 
 * When a pocket-home password is set, the user has to enter the password to
 * close the LoginPage.  Otherwise, the page just functions as a lock screen,
 * and can be closed by pressing a button.
 */

class LoginPage : public PageComponent, public juce::TextEditor::Listener,
public Locale::TextUser
{
public:

    /**
     * @brief  Creates the login page, setting the action it should take when
     *         the user logs in.
     *
     * @param loginCallback  A callback function to run when the user logs in
     *                       successfully.  This should close the LoginPage
     *                       and show normal application content.
     */
    LoginPage(std::function<void () > loginCallback);

    virtual ~LoginPage() { }

    /**
     * Checks if an application password was set.
     * 
     * @return true iff a valid password file was found.
     */
    virtual bool hasPassword();

    /**
     * Grants keyboard focus to the password field, so it doesn't need to be
     * clicked before the user can start typing their password.
     */
    virtual void textFocus();

private:
    /**
     * Attempts to login when the user clicks the login button.
     * 
     * @param button  This should always be the loginButton.
     */
    virtual void pageButtonClicked(juce::Button *button) override;

    /**
     * If the return key is pressed, handle it the same as clicking the login
     * button.
     * 
     * @param editor
     */
    virtual void textEditorReturnKeyPressed(juce::TextEditor& editor) override;

    /**
     * Show an error message in a message box if logging in fails.
     */
    void displayError();

    std::function<void() > loginCallback;
    bool foundPassword;
    juce::TextButton loginButton;
    ScalingLabel passwordLabel;
    juce::TextEditor passwordField;
    juce::String hashedPassword;
    DrawableImageComponent ntcIcon;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoginPage)
};
