#include "SetPasswordPage.h"
#include "AssetFiles.h"
#include "LoginPage.h"
#include "Password.h"

LoginPage::LoginPage(std::function<void () > loginCallback) :
Localized("LoginPage"),
PageComponent("LoginPage"),
ntcIcon("login/chipBismuth.png"),
passwordLabel("pass", localeText(password_label)),
passwordField("passwordField", 0x2022),
loginButton(localeText(log_in), "loginButton"),
hashedPassword("none"),
loginCallback(loginCallback),
foundPassword(false)
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("LoginPage");
#    endif
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(70, { RowItem(&ntcIcon) } ),
        Row(12, 
        {   
            RowItem(),
            RowItem(&passwordLabel),
            RowItem()
        }),
        Row(15,
        {   
            RowItem(),
            RowItem(&passwordField),
            RowItem()
        }),
        Row(15, 
        {   
            RowItem(),
            RowItem(&loginButton),
            RowItem()
        })
    });
    layout.setXMarginFraction(0.1);
    layout.setYMarginFraction(0.1);
    layout.setYPaddingWeight(2);
    setLayout(layout);
    
    setBackgroundImage(AssetFiles::loadImageAsset("login/background.png"));
    loginButton.addListener(this);   
    
    ComponentConfigFile config;
    passwordField.setFont(Font(config.getFontHeight
            (ComponentConfigFile::smallText)));
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
void LoginPage::pageButtonClicked(juce::Button *button)
{
    using namespace juce;
    String password = passwordField.getText();
    passwordField.setText("");
    if (Password::checkPassword(password))
    {
        loginCallback();
    }
    else displayError();
}

/**
 * If the return key is pressed, handle it the same as clicking the login
 * button.
 */
void LoginPage::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    pageButtonClicked(&loginButton);
}

/**
 * Show an error message in a message box if logging in fails.
 */
void LoginPage::displayError()
{
    using namespace juce;
    AlertWindow::showMessageBoxAsync(
            AlertWindow::AlertIconType::WarningIcon,
            localeText(wrong_password),
            localeText(wrong_password_retry),
            localeText(close_button_text));
}
