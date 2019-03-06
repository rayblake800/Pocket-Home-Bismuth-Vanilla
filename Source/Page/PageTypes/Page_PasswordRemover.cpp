#include "Page_PasswordRemover.h"
#include "Password.h"
#include "Layout_Component_ConfigFile.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Page::PasswordRemover::";
#endif

/* Localized object class key: */
static const juce::Identifier localeClassKey = "Page::PasswordRemover";

/* Localized text value keys: */
namespace TextKey
{
    static const juce::Identifier currentPassword    = "currentPassword";
    static const juce::Identifier removePassword     = "removePassword";
    static const juce::Identifier apply              = "apply";
}

/* Page layout constants: */
static const constexpr int titleRowWeight     = 20;
static const constexpr int upperGapWeight     = 25;
static const constexpr int textFieldRowWeight = 10;
static const constexpr int lowerGapWeight     = 5;
static const constexpr int buttonRowWeight    = 10;

static const constexpr int labelWeight       = 20;
static const constexpr int textFieldWeight   = 30;
static const constexpr int xPaddingWeight    = 1;
static const constexpr float xMarginFraction = 0.1;
static const constexpr float yMarginFraction = 0.1;

static const constexpr Layout::Component::TextSize maxLabelSize
        = Layout::Component::TextSize::smallText;
    
Page::PasswordRemover::PasswordRemover() :
Locale::TextUser(localeClassKey),
removalController(currentPasswordField, removerButton,
        [this]() { removeFromStack(); })
{
#if JUCE_DEBUG
    setName("Page::PasswordRemover");
#endif
    setBackButton(BackButtonType::left);

    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(titleRowWeight, 
        { 
            RowItem(&titleLabel) 
        }),
        Row(upperGapWeight),
        Row(textFieldRowWeight, 
        {
            RowItem(&currentPasswordLabel, labelWeight),
            RowItem(&currentPasswordField, textFieldWeight)
        }),
        Row(lowerGapWeight),
        Row(buttonRowWeight, 
        { 
            RowItem(&removerButton) 
        })
    });
    layout.setXMarginFraction(xMarginFraction);
    layout.setYMarginFraction(yMarginFraction);
    layout.setXPaddingWeight(xPaddingWeight);
    setLayout(layout);
    
    Layout::Component::ConfigFile config;
    currentPasswordField.setFont(juce::Font(config.getFontHeight
            (Layout::Component::TextSize::smallText)));
    
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setText(localeText(TextKey::removePassword),
            juce::NotificationType::dontSendNotification);
    titleLabel.setMaximumTextSize(maxLabelSize);
    currentPasswordLabel.setText(localeText(TextKey::currentPassword),
            juce::NotificationType::dontSendNotification);
    removerButton.setButtonText(localeText(TextKey::apply));
    addAndShowLayoutComponents();
}
