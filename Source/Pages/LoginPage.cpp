#include "SetPasswordPage.h"
#include "AssetFiles.h"
#include "LoginPage.h"
#include "Password.h"

LoginPage::LoginPage(ComponentConfigFile& config,
        std::function<void () > loginCallback) :
Localized("LoginPage"),
PageComponent(config, "LoginPage",{
    {7,
        {
            {&ntcIcon, 1}
        }},
    {2,
        {
            {nullptr, 1}
        }},
    {1,
        {
            {nullptr, 1},
            {&passwordLabel, 1},
            {nullptr, 1}
        }},
    {2,
        {
            {nullptr, 1},
            {&passwordField, 1},
            {nullptr, 1}
        }},
    {2,
        {
            {nullptr, 1},
            {&loginButton, 1},
            {nullptr, 1}
        }}
}, nullptr, false),
ntcIcon("login/ntcbanner.png"),
passwordLabel(config, "pass", localeText(password_label)),
passwordField("passwordField", 0x2022),
loginButton(localeText(log_in), "loginButton"),
hashedPassword("none"),
loginCallback(loginCallback),
foundPassword(false)
{

#    if JUCE_DEBUG
    setName("LoginPage");
#    endif
    setBackgroundImage(AssetFiles::loadImageAsset("login/background.png"));
    loginButton.addListener(this);
    passwordField.addListener(this);
    addAndShowLayoutComponents();
    if (!hasPassword())
    {
        passwordLabel.setVisible(false);
        passwordField.setVisible(false);
    }
}

/**
 * Checks if an application password was set.
 * 
 */
bool LoginPage::hasPassword()
{
    return Password::isPasswordSet();
}

/**
 * Grants keyboard focus to the password field, so it doesn't need to be
 * clicked before the user can start typing their password.
 */
void LoginPage::textFocus()
{
    passwordField.grabKeyboardFocus();
    passwordField.setWantsKeyboardFocus(true);
}

/**
 * Attempts to login when the user clicks the login button.
 */
void LoginPage::pageButtonClicked(Button *button)
{
    if (Password::checkPassword(passwordField.getText()))
    {
        passwordField.setText("");
        loginCallback();
    }
    else displayError();
}

/**
 * If the return key is pressed, handle it the same as clicking the login
 * button.
 */
void LoginPage::textEditorReturnKeyPressed(TextEditor& editor)
{
    pageButtonClicked(&loginButton);
}

/**
 * Show an error message in a message box if logging in fails.
 */
void LoginPage::displayError()
{
    AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
            localeText(wrong_password),
            localeText(wrong_password_retry),
            localeText(close_button_text));
}
