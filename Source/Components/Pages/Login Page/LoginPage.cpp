#include "SetPasswordPage.h"
#include "AssetFiles.h"
#include "LoginPage.h"
#include "Password.h"

/* Class localized text key: */
static const juce::Identifier localeClassKey("LoginPage");

/* Localized text value keys: */
static const juce::Identifier passwordLabelKey     = "passwordLabel";
static const juce::Identifier logInTextKey         = "logIn";
static const juce::Identifier wrongPasswordTextKey = "wrongPassword";
static const juce::Identifier retryPasswordTextKey = "retryPassword";
static const juce::Identifier closeButtonTextKey   = "closeButton";

// TODO: set these images in configuration files somewhere.
static const constexpr char* backgroundImagePath = "login/background.png";
static const constexpr char* iconImagePath       = "login/ntcbanner.png";

/*
 * Creates the login page, setting the action it should take when the user logs 
 * in.
 */
LoginPage::LoginPage(std::function<void () > loginCallback) :
Locale::TextUser(localeClassKey),
PageComponent("LoginPage"),
ntcIcon(iconImagePath),
passwordLabel("pass", localeText(passwordLabelKey)),
passwordField("passwordField", 0x2022),
loginButton(localeText(logInTextKey), "loginButton"),
hashedPassword("none"),
loginCallback(loginCallback),
foundPassword(false)
{
#if JUCE_DEBUG
    setName("LoginPage");
#endif
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
    
    setBackgroundImage(AssetFiles::loadImageAsset(backgroundImagePath));
    loginButton.addListener(this);   
    
    ComponentConfigFile config;
    passwordField.setFont(juce::Font(config.getFontHeight
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
            localeText(wrongPasswordTextKey),
            localeText(retryPasswordTextKey),
            localeText(closeButtonTextKey));
}
