#include "Page_PasswordEditor.h"
#include "Layout_Component_ConfigFile.h"

// Localized object class key:
static const juce::Identifier localeClassKey = "Page::PasswordEditor";

// Localized text value keys:
namespace TextKey
{
    static const juce::Identifier title           = "changePassword";
    static const juce::Identifier currentPassword = "currentPassword";
    static const juce::Identifier newPassword     = "newPassword";
    static const juce::Identifier retypePassword  = "retypePassword";
    static const juce::Identifier update          = "update";
}


// Page layout constants:
static const constexpr int titleRowWeight   = 20;
static const constexpr int contentRowWeight = 10;
static const constexpr int labelWeight      = 20;
static const constexpr int textFieldWeight  = 30;
static const constexpr int paddingWeight    = 3;

static const constexpr float yMarginFraction = 0.05;

static const constexpr Layout::Component::TextSize maxLabelSize
        = Layout::Component::TextSize::smallText;

Page::PasswordEditor::PasswordEditor() :
Locale::TextUser(localeClassKey),
updateController(currentPasswordField, newPasswordField, confirmPasswordField,
        updateButton, [this]() { removeFromStack(); })
{
#if JUCE_DEBUG
    setName("Page::PasswordEditor");
#endif
    setBackButton(BackButtonType::left);
    using namespace Layout::Group;
    RelativeLayout layout({
        Row(titleRowWeight,
        {
            RowItem(&titleLabel)
        }),
        Row(contentRowWeight,
        {
            RowItem(&currentPasswordLabel, labelWeight),
            RowItem(&currentPasswordField, textFieldWeight)
        }),
        Row(contentRowWeight,
        {
            RowItem(&newPasswordLabel, labelWeight),
            RowItem(&newPasswordField, textFieldWeight)
        }),
        Row(contentRowWeight,
        {
            RowItem(&confirmPasswordLabel, labelWeight),
            RowItem(&confirmPasswordField, textFieldWeight)
        }),
        Row(contentRowWeight,
        {
            RowItem(&updateButton)
        })
    });
    layout.setYMarginFraction(yMarginFraction);
    layout.setXPaddingWeight(paddingWeight);
    layout.setYPaddingWeight(paddingWeight);
    setLayout(layout);

    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setText(localeText(TextKey::title),
            juce::NotificationType::dontSendNotification);

    Layout::Component::ConfigFile config;
    const auto initLabel = [this, &config]
        (Widgets::BoundedLabel& label, const juce::Identifier& key)
    {
        label.setText(localeText(key),
                juce::NotificationType::dontSendNotification);
        label.setMaximumTextSize(maxLabelSize);
    };
    initLabel(currentPasswordLabel, TextKey::currentPassword);
    initLabel(newPasswordLabel, TextKey::newPassword);
    initLabel(confirmPasswordLabel, TextKey::retypePassword);
    updateButton.setButtonText(localeText(TextKey::update));
    addAndShowLayoutComponents();

    const bool checkCurrentPassword = currentPasswordField.isEnabled();
    currentPasswordLabel.setVisible(checkCurrentPassword);
    currentPasswordField.setVisible(checkCurrentPassword);
}
