/**
 * @file LoginPage.h
 * 
 * The LoginPage is shown whenever the system exits sleep mode, when sleep mode
 * is triggered by this application.  If a pocket-home password is set, 
 * LoginPage will also be shown when the application starts.
 * 
 * When a pocket-home password is set, the user has to enter the password to
 * close the LoginPage.  Otherwise, the page just functions as a lock screen,
 * and can be closed by pressing a button.
 */
#include "JuceHeader.h"
#include "Localized.h"
#include "ScalingLabel.h"
#include "DrawableImageComponent.h"
#include "PageComponent.h"
#pragma once

class LoginPage : public PageComponent, public TextEditor::Listener,
private Localized
{
public:

    /**
     * @param config         ComponentConfigFile reference required by all 
     *                        PageComponents.
     * 
     * @param loginCallback  A callback function to run when the user logs in
     *                        successfully.  This should close the LoginPage
     *                        and show normal application content.
     */
    LoginPage(ComponentConfigFile& config, 
            std::function<void () > loginCallback);

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
    virtual void pageButtonClicked(Button *button) override;

    /**
     * If the return key is pressed, handle it the same as clicking the login
     * button.
     * 
     * @param editor
     */
    virtual void textEditorReturnKeyPressed(TextEditor& editor) override;

    /**
     * Show an error message in a message box if logging in fails.
     */
    void displayError();

    std::function<void() > loginCallback;
    bool foundPassword;
    TextButton loginButton;
    ScalingLabel passwordLabel;
    TextEditor passwordField;
    String hashedPassword;
    DrawableImageComponent ntcIcon;


    //localized text keys;
    static const constexpr char * password_label = "password_label";
    static const constexpr char * log_in = "log_in";
    static const constexpr char * wrong_password = "wrong_password";
    static const constexpr char * wrong_password_retry = "wrong_password_retry";
    static const constexpr char * close_button_text = "close_button_text";


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoginPage)
};
