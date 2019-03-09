#include "Password_LoginScreen.h"
#include "Password.h"
#include "Assets.h"
#include "Theme_Image_JSONKeys.h"
#include "Theme_Image_AssetList.h"
#include "Layout_Component_ConfigFile.h"

/* Class localized text key: */
static const juce::Identifier localeClassKey("Password::LoginScreen");

/* Localized text value keys: */
namespace TextKey
{
    static const juce::Identifier passwordLabel = "passwordLabel";
    static const juce::Identifier logIn         = "logIn";
    static const juce::Identifier wrongPassword = "wrongPassword";
    static const juce::Identifier retryPassword = "retryPassword";
    static const juce::Identifier closeButton   = "closeButton";
}

/* Component layout constants: */
static const constexpr int imageRowWeight     = 70;
static const constexpr int labelRowWeight     = 12;
static const constexpr int textFieldRowWeight = 15;
static const constexpr int buttonRowWeight    = 15;
static const constexpr int rowPaddingWeight   = 2;

static const constexpr float xMarginFraction  = 0.1;
static const constexpr float yMarginFraction  = 0.1;

/* Extra horizontal margin space below the login image: */
static const constexpr int controlMarginWeight  = 2;
static const constexpr int controlContentWeight = 3;

/* Masks password field input: */
static const constexpr juce::juce_wchar passwordChar = 0x2022;

/*
 * Creates the login screen, setting the action it should take when the user 
 * logs in.
 */
Password::LoginScreen::LoginScreen(std::function<void()> loginCallback) :
Locale::TextUser(localeClassKey),
passwordLabel("passwordLabel", localeText(TextKey::passwordLabel)),
passwordField("passwordField", passwordChar),
loginButton(localeText(TextKey::logIn), "loginButton"),
loginCallback(loginCallback)
{
#if JUCE_DEBUG
    setName("Password::LoginScreen");
#endif
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(imageRowWeight, 
        { 
            RowItem(&loginImage) 
        }),
        Row(labelRowWeight, 
        {   
            RowItem(controlMarginWeight),
            RowItem(&passwordLabel, controlContentWeight),
            RowItem(controlMarginWeight)
        }),
        Row(textFieldRowWeight,
        {   
            RowItem(controlMarginWeight),
            RowItem(&passwordField, controlContentWeight),
            RowItem(controlMarginWeight)
        }),
        Row(buttonRowWeight, 
        {   
            RowItem(controlMarginWeight),
            RowItem(&loginButton, controlContentWeight),
            RowItem(controlMarginWeight),
        })
    });
    layout.setXMarginFraction(xMarginFraction);
    layout.setYMarginFraction(yMarginFraction);
    layout.setYPaddingWeight(rowPaddingWeight);
    setLayout(layout);
    
    const juce::String bgImagePath = imageConfig.getLoginBackgroundPath();
    setBackgroundImage(Assets::loadImageAsset(bgImagePath));
    const juce::String loginImagePath = imageConfig.getLoginImagePath();
    loginImage.setImage(loginImagePath);

    loginButton.addListener(this);   
    
    passwordField.addListener(this);
    addAndShowLayoutComponents();
    if (!isPasswordSet())
    {
        passwordLabel.setVisible(false);
        passwordField.setVisible(false);
    }
}

/*
 * Grants keyboard focus to the password field, so it doesn't need to be clicked
 * before the user can start typing their password.
 */
void Password::LoginScreen::getKeyboardFocus()
{
    passwordField.grabKeyboardFocus();
    passwordField.setWantsKeyboardFocus(true);
}

/*
 * Attempts to login when the user clicks the login button.
 */
void Password::LoginScreen::buttonClicked(juce::Button *button)
{
    juce::String password = passwordField.getText();
    passwordField.setText("");
    if (Password::checkPassword(password))
    {
        loginCallback();
    }
    else displayError();
}

/*
 * If the return key is pressed, handle it the same as clicking the login
 * button.
 */
void Password::LoginScreen::textEditorReturnKeyPressed
(juce::TextEditor& editor)
{
    buttonClicked(&loginButton);
}

/*
 * Checks that an application password hasn't been recently added or removed 
 * whenever the component regains visibility.
 */
void Password::LoginScreen::visibilityChanged()
{
    if(isVisible())
    {
        bool hasPassword = isPasswordSet();
        passwordLabel.setVisible(hasPassword);
        passwordField.setVisible(hasPassword);
    }
}

/*
 * Shows an error message in a message box if logging in fails.
 */
void Password::LoginScreen::displayError()
{
    using juce::AlertWindow;
    AlertWindow::showMessageBoxAsync(
            AlertWindow::AlertIconType::WarningIcon,
            localeText(TextKey::wrongPassword),
            localeText(TextKey::retryPassword),
            localeText(TextKey::closeButton));
}
