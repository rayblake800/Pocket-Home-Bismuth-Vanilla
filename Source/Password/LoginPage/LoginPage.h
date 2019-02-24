#pragma once
/**
 * @file LoginPage.h
 * 
 * @brief  Provides the application's login page.
 */

#include "JuceHeader.h"
#include "Locale/TextUser.h"
#include "ScalingLabel.h"
#include "Theme_Image_Component.h"
#include "Page_Component.h"

/**
 * @brief  The page shown whenever the system exits sleep mode, or when the
 *         application starts if the pocket-home password is set.
 * 
 * When a pocket-home password is set, the user has to enter the password to
 * close the LoginPage. Otherwise, the page just functions as a lock screen,
 * and can be closed by pressing a button.
 */
class LoginPage : public Page::Component, public juce::TextEditor::Listener,
public Locale::TextUser
{
public:
    /**
     * @brief  Creates the login page, setting the action it should take when
     *         the user logs in.
     *
     * @param loginCallback  A callback function to run when the user logs in
     *                       successfully. This should close the LoginPage
     *                       and show normal application content.
     */
    LoginPage(const std::function<void()> loginCallback);

    virtual ~LoginPage() { }

    /**
     * @brief  Checks if an application password was set.
     * 
     * @return  Whether a valid password file was found.
     */
    virtual bool hasPassword();

    /**
     * @brief  Grants keyboard focus to the password field, so it doesn't need 
     *         to be clicked before the user can start typing their password.
     */
    virtual void textFocus();

private:
    /**
     * @brief  Shows an error message in a message box if logging in fails.
     */
    void displayError();

    /**
     * @brief  Listens for Button and TextEditor events within the power page.
     */
    class PageListener : public juce::Button::Listener, 
            public juce::TextEditor::Listener
    {
    public:
        /**
         * @brief  Connects the listener to its login page.
         *
         * @param loginPage  The LoginPage that owns this PageListener.
         */
        PageListener(LoginPage& loginPage) : loginPage(loginPage) { }

        virtual ~PageListener() { }

    private:
        /**
         * @brief  Attempts to login when the user clicks the login button.
         * 
         * @param button  The login page's login button.
         */
        virtual void buttonClicked(juce::Button* button) override;

        /**
         * @brief  Attempts to login when the return key is pressed while the
         *         password editor is focused.
         * 
         * @param editor  The login page's password editor
         */
        virtual void textEditorReturnKeyPressed
        (juce::TextEditor& editor) override;

        LoginPage& loginPage;
    };
    PageListener pageListener;

    /* Callback function to run after successfully logging in: */
    const std::function<void()> loginCallback;

    /* Whether a password is needed to log in: */
    bool foundPassword;

    /* Page child components: */
    juce::TextButton loginButton;
    ScalingLabel passwordLabel;
    juce::TextEditor passwordField;
    Theme::Image::Component<> loginImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoginPage)
};
