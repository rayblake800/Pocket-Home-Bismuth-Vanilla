#include "../PocketHomeApplication.h"
#include "SetPasswordPage.h"
#include "LoginPage.h"

LoginPage::LoginPage(std::function<void () > loginCallback) :
PageComponent("LoginPage",{
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
}),
ntcIcon("login/ntcbanner.png"),
passwordLabel("pass", "Password :"),
passwordField("passwordField", 0x2022),
loginButton("login", "Log In"),
hashedPassword("none"),
loginCallback(loginCallback),
foundPassword(false)
{
    setBackgroundImage(createImageFromFile(assetFile("login/background.png")));
    loginButton.addListener(this);
    passwordField.addListener(this);
    addAndShowLayoutComponents();
    checkForPassword();
}

LoginPage::~LoginPage() { }

bool LoginPage::hasPassword()
{
    checkForPassword();
    return foundPassword;
}

void LoginPage::checkForPassword()
{
    char* home_str = getenv("HOME");
    String home(home_str);
    File passwd(home + "/.pocket-home/.passwd/passwd");
    foundPassword = false;
    if (passwd.exists())
    {
        String content = passwd.loadFileAsString();
        content = content.removeCharacters("\n");
        if (content != String("none"))
        {
            hashedPassword = content;
            foundPassword = true;
        }
    }
    passwordLabel.setVisible(foundPassword);
    passwordField.setVisible(foundPassword);

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
    String pass_tmp = passwordField.getText();
    String hashed = SetPasswordPage::hashString(pass_tmp);
    if (hashedPassword == "none" || hashed == hashedPassword)
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

