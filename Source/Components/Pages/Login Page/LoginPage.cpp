#include "SetPasswordPage.h"
#include "AssetFiles.h"
#include "LoginPage.h"
#include "Password.h"

LoginPage::LoginPage(std::function<void () > loginCallback) :
Localized("LoginPage"),
PageComponent("LoginPage"),
ntcIcon("login/ntcbanner.png"),
passwordLabel("pass", localeText(password_label)),
passwordField("passwordField", 0x2022),
loginButton(localeText(log_in), "loginButton"),
hashedPassword("none"),
loginCallback(loginCallback),
foundPassword(false)
{

#    if JUCE_DEBUG
    setName("LoginPage");
#    endif
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(70, { RowItem(&ntcIcon) } ),
        Row(10, { RowItem(&passwordLabel) } ),
        Row(20, { RowItem(&passwordField) } ),
        Row(20, { RowItem(&loginButton) } )
    });
    layout.setXMarginFraction(0.3);
    layout.setYMarginFraction(0.1);
    layout.setYPaddingWeight(1);
    setLayout(layout);
    
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
