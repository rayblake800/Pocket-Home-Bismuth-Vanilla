#include "Page_InputSettings.h"

// Localized object class key:
static const juce::Identifier localeClassKey = "Page::InputSettings";

// Localized text value keys:
static const juce::Identifier inputSettingsTextKey = "inputSettings";
static const juce::Identifier showCursorTextKey    = "showCursor";

// Page layout constants:
static const constexpr int titleRowWeight     = 20;
static const constexpr int cursorRowWeight    = 10;
static const constexpr int gapRowWeight       = 30;
static const constexpr int calibrateRowWeight = 10;
static const constexpr int rowPaddingWeight   = 1;

static const constexpr int labelWeight    = 50;
static const constexpr int comboBoxWeight = 20;
static const constexpr int xPaddingWeight = 1;

static const constexpr float yMarginFraction = 0.06;

// Loads the page layout on construction.
Page::InputSettings::InputSettings() : Locale::TextUser(localeClassKey)
{
#if JUCE_DEBUG
    setName("Page::InputSettings");
#endif
    setBackButton(BackButtonType::left);
    using namespace Layout::Group;
    RelativeLayout layout({
        Row(titleRowWeight,
        {
            RowItem(&titleLabel)
        }),
        Row(cursorRowWeight,
        {
            RowItem(&cursorModeLabel, labelWeight),
            RowItem(&cursorModePicker, comboBoxWeight)
        }),
        Row(gapRowWeight),
        Row(calibrateRowWeight,
        {
            RowItem(&calibrateButton)
        })
    });
    layout.setYPaddingWeight(rowPaddingWeight);
    layout.setYMarginFraction(yMarginFraction);
    layout.setXPaddingWeight(xPaddingWeight);
    setLayout(layout);

    titleLabel.setText(localeText(inputSettingsTextKey),
            juce::NotificationType::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    cursorModeLabel.setText(localeText(showCursorTextKey),
            juce::NotificationType::dontSendNotification);
    addAndShowLayoutComponents();
}
