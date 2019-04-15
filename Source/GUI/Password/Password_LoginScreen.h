#pragma once
/**
 * @file  Password_LoginScreen.h
 *
 * @brief  Provides the application's login screen.
 */

#include "Widgets_LayoutContainer.h"
#include "Locale_TextUser.h"
#include "Widgets_BoundedLabel.h"
#include "Widgets_DrawableImage.h"
#include "Widgets_TextEditor.h"
#include "Theme_Image_ConfigFile.h"
#include "Layout_Group_Manager.h"
#include "JuceHeader.h"

namespace Password { class LoginScreen; }

/**
 * @brief  The screen shown whenever the system exits sleep mode, or when the
 *         application starts if the pocket-home password is set.
 *
 *  When a pocket-home password is set, the user has to enter the password to
 * close the LoginScreen. Otherwise, the page just functions as a lock screen,
 * and can be closed by pressing a button.
 */
class Password::LoginScreen : public Widgets::LayoutContainer,
    public Locale::TextUser,
    private juce::Button::Listener,
    private juce::TextEditor::Listener
{
public:
    /**
     * @brief  Creates the login screen, setting the action it should take when
     *         the user logs in.
     *
     * @param loginCallback  A callback function to run when the user logs in
     *                       successfully. This should close the LoginScreen
     *                       and show normal application content.
     */
    LoginScreen(const std::function<void()> loginCallback);

    virtual ~LoginScreen() { }

    /**
     * @brief  Grants keyboard focus to the password field, so it doesn't need
     *         to be clicked before the user can start typing their password.
     */
    void getKeyboardFocus();

private:
    /**
     * @brief  Shows an error message in a message box if logging in fails.
     */
    void displayError();

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
     * @param editor  The login page's password editor field
     */
    virtual void textEditorReturnKeyPressed
    (juce::TextEditor& editor) override;

    /**
     * @brief  Checks that an application password hasn't been recently added
     *         or removed whenever the component regains visibility.
     */
    virtual void visibilityChanged() override;

    // Callback function to run after successfully logging in:
    const std::function<void()> loginCallback;

    // Loads image asset paths, and ensures the JSON image path resource
    // remains available:
    const Theme::Image::ConfigFile imageConfig;

    // Page child components:
    juce::TextButton loginButton;
    Widgets::BoundedLabel passwordLabel;
    Widgets::TextEditor passwordField;
    Widgets::DrawableImage loginImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoginScreen)
};
