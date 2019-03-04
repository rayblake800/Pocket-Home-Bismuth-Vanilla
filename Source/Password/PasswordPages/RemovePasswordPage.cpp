#include "RemovePasswordPage.h"
#include "Password.h"
#include "Layout_Component_ConfigFile.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "RemovePasswordPage::";
#endif

/* Localized object class key: */
static const juce::Identifier localeClassKey = "RemovePasswordPage";

/* Localized text value keys: */
namespace TextKey
{
    static const juce::Identifier currentPassword    = "currentPassword";
    static const juce::Identifier removePassword     = "removePassword";
    static const juce::Identifier apply              = "apply";

    static const juce::Identifier success            = "success";
    static const juce::Identifier passwordRemoved    = "passwordRemoved";

    static const juce::Identifier cantRemovePassword = "cantRemovePassword";
    static const juce::Identifier wrongPassword      = "wrongPassword";
    static const juce::Identifier checkAgent         = "checkAgent";

    static const juce::Identifier error              = "error";
    static const juce::Identifier filesMissing       = "filesMissing";
    static const juce::Identifier polkitMissing      = "polkitMissing";
}
    
RemovePasswordPage::RemovePasswordPage() :
Locale::TextUser(localeClassKey),
pageListener(*this),
curPwdLabel("CurLabel", localeText(TextKey::currentPassword)),
curPassword("Current", 0x2022),
titleLabel("Title", localeText(TextKey::removePassword))
{
#if JUCE_DEBUG
    setName("RemovePasswordPage");
#endif
    setBackButton(BackButtonType::left);

    using namespace Layout::Group;
    RelativeLayout layout({
        Row(20, { RowItem(&titleLabel) }),
        Row(10, {
                    RowItem(&curPwdLabel, 20),
                    RowItem(&curPassword, 30)
                }),
        Row(10, { RowItem(&deleteButton, 10) })
    });
    layout.setXMarginFraction(0.1);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(1);
    setLayout(layout);
    
    Layout::Component::ConfigFile config;
    curPassword.setFont(juce::Font(config.getFontHeight
            (Layout::Component::TextSize::smallText)));
    
    titleLabel.setJustificationType(juce::Justification::centred);
    deleteButton.setButtonText(localeText(TextKey::apply));
    deleteButton.addListener(&pageListener);
    addAndShowLayoutComponents();
}

/*
 * Attempts to delete the Pocket-Home password when deleteButton is pressed.
 * If this succeeds, the page will close after showing an AlertWindow.
 */
void RemovePasswordPage::PageListener::buttonClicked(juce::Button* button)
{
    using juce::AlertWindow;
    if (button != &passwordPage.deleteButton)
    {
        DBG(dbgPrefix << __func__ << ": button " << button->getName()
                << " should not be triggering this function!");
        passwordPage.curPassword.clear();
        return;
    }
    juce::String title, message;
    switch (Password::removePassword(passwordPage.curPassword.getText()))
    {
        case Password::missingNewPassword:
        case Password::passwordSetSuccess:
        case Password::fileCreateFailed:
        case Password::fileWriteFailed:
        case Password::fileSecureFailed:
            DBG(dbgPrefix << __func__ << ": Illegal result returned!");
            jassertfalse;
            return;
        case Password::passwordRemoveSuccess:
            AlertWindow::showMessageBoxAsync(
                    AlertWindow::AlertIconType::InfoIcon,
                    passwordPage.localeText(TextKey::success),
                    passwordPage.localeText(TextKey::passwordRemoved),
                    "",
                    nullptr,
                    juce::ModalCallbackFunction::create([this](int i)
                    {
                        passwordPage.removeFromStack();
                    }));
            passwordPage.curPassword.clear();
            return;
        case Password::wrongPasswordError:
            title = passwordPage.localeText(TextKey::cantRemovePassword);
            message = passwordPage.localeText(TextKey::wrongPassword);
            break;
        case Password::fileDeleteFailed:
            title = passwordPage.localeText(TextKey::cantRemovePassword);
            message = passwordPage.localeText(TextKey::checkAgent);
            break;
        case Password::noPasswordScript:
            title = passwordPage.localeText(TextKey::error);
            message = passwordPage.localeText(TextKey::filesMissing);
            break;
        case Password::noPKExec:
            title = passwordPage.localeText(TextKey::error);
            message = passwordPage.localeText(TextKey::polkitMissing);
            break;
    }
    AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
            title, message, "", nullptr);
}
