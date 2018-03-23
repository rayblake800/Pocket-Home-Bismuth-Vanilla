#include "SetPasswordPage.h"
#include "AssetFiles.h"
#include "LoginPage.h"
#include "Password.h"

LoginPage::LoginPage(ComponentConfigFile& config,
        std::function<void () > loginCallback) :
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
},nullptr,false),
ntcIcon("login/ntcbanner.png"),
passwordLabel(config, "pass", "Password :"),
passwordField("passwordField", 0x2022),
loginButton("login", "Log In"),
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

bool LoginPage::hasPassword()
{
    return Password::isPasswordSet();
}


void LoginPage::displayError()
{
    AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
            "Wrong password",
            "Wrong password, try again",
            "Ok");
}

void LoginPage::textFocus()
{
    passwordField.grabKeyboardFocus();
    passwordField.setWantsKeyboardFocus(true);
}

void LoginPage::pageButtonClicked(Button *button)
{
    if (Password::checkPassword(Password::hashString(passwordField.getText())))
    {
        passwordField.setText("");
        loginCallback();
    }
    else displayError();
}

void LoginPage::textEditorReturnKeyPressed(TextEditor& te)
{
    pageButtonClicked(&loginButton);
}

