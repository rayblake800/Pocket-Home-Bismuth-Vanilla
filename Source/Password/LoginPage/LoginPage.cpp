#include "LoginPage.h"
#include "SetPasswordPage.h"
#include "AssetFiles.h"
#include "Password.h"
#include "Theme_Image_JSONKeys.h"
#include "Theme_Image_ConfigFile.h"
#include "Theme_Image_AssetList.h"
#include "Layout_Component_ConfigFile.h"

/* Class localized text key: */
static const juce::Identifier localeClassKey("LoginPage");

/* Localized text value keys: */
namespace TextKey
{
    static const juce::Identifier passwordLabel     = "passwordLabel";
    static const juce::Identifier logIn             = "logIn";
    static const juce::Identifier wrongPassword     = "wrongPassword";
    static const juce::Identifier retryPassword     = "retryPassword";
    static const juce::Identifier closeButton       = "closeButton";
}

/*
 * Creates the login page, setting the action it should take when the user logs 
 * in.
 */
LoginPage::LoginPage(std::function<void () > loginCallback) :
Locale::TextUser(localeClassKey),
pageListener(*this),
loginImage(Theme::Image::JSONKeys::loginImage),
passwordLabel("passwordLabel", localeText(TextKey::passwordLabel)),
passwordField("passwordField", 0x2022),
loginButton(localeText(TextKey::logIn), "loginButton"),
loginCallback(loginCallback),
foundPassword(false)
{
#if JUCE_DEBUG
    setName("LoginPage");
#endif
    using namespace Layout::Group;
    RelativeLayout layout({
        Row(70, { RowItem(&loginImage) } ),
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
    
    Theme::Image::ConfigFile imageConfig;
    juce::String bgImagePath 
            = imageConfig.getAssetList(Theme::Image::JSONKeys::loginPage)
            .getImageFiles()[0];
    setBackgroundImage(AssetFiles::loadImageAsset(bgImagePath));
    loginButton.addListener(&pageListener);   
    
    Layout::Component::ConfigFile config;
    passwordField.setFont(juce::Font(config.getFontHeight
            (Layout::Component::TextSize::smallText)));
    passwordField.addListener(&pageListener);
    addAndShowLayoutComponents();
    if (!hasPassword())
    {
        passwordLabel.setVisible(false);
        passwordField.setVisible(false);
    }
}

/*
 * Checks if an application password was set.
 */
bool LoginPage::hasPassword()
{
    return Password::isPasswordSet();
}

/*
 * Grants keyboard focus to the password field, so it doesn't need to be clicked
 * before the user can start typing their password.
 */
void LoginPage::textFocus()
{
    passwordField.grabKeyboardFocus();
    passwordField.setWantsKeyboardFocus(true);
}

/*
 * Attempts to login when the user clicks the login button.
 */
void LoginPage::PageListener::buttonClicked(juce::Button *button)
{
    juce::String password = loginPage.passwordField.getText();
    loginPage.passwordField.setText("");
    if (Password::checkPassword(password))
    {
        loginPage.loginCallback();
    }
    else loginPage.displayError();
}

/*
 * If the return key is pressed, handle it the same as clicking the login
 * button.
 */
void LoginPage::PageListener::textEditorReturnKeyPressed
(juce::TextEditor& editor)
{
    buttonClicked(&loginPage.loginButton);
}

/*
 * Shows an error message in a message box if logging in fails.
 */
void LoginPage::displayError()
{
    using juce::AlertWindow;
    AlertWindow::showMessageBoxAsync(
            AlertWindow::AlertIconType::WarningIcon,
            localeText(TextKey::wrongPassword),
            localeText(TextKey::retryPassword),
            localeText(TextKey::closeButton));
}
