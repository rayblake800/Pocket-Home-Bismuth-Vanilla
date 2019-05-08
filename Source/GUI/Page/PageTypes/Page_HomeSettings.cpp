#include "Page_HomeSettings.h"
#include "Config_MainFile.h"
#include "Config_MainKeys.h"
#include "AppMenu_ConfigFile.h"
#include "AppMenu_Format.h"
#include "Windows_Info.h"

// Localized object class key
static const juce::Identifier localeClassKey = "Page::HomeSettings";

// Localized text value keys:
namespace TextKey
{
    static const juce::Identifier title      = "title";
    static const juce::Identifier background = "background";
    static const juce::Identifier clockMode  = "clockMode";
}


// Page layout constants:
static const constexpr int titleRowWeight   = 30;
static const constexpr int contentRowWeight = 20;
static const constexpr int dividedRowWeight = 10;
static const constexpr int rowPaddingWeight = 3;

static const constexpr int labelWeight    = 10;
static const constexpr int comboBoxWeight = 10;

static const constexpr int counterLabelWeight = 20;
static const constexpr int counterWeight      = 10;

static const constexpr int xPaddingWeight = 1;
static const constexpr float yMarginFraction = 0.05;

// Initializes the page layout.
Page::HomeSettings::HomeSettings() :
Locale::TextUser(localeClassKey),
title("personalizeTitle", localeText(TextKey::title)),
bgLabel("bgLabel", localeText(TextKey::background)),
clockLabel("clockLabel", localeText(TextKey::clockMode)),
menuController(menuFormatLabel, menuFormatPicker, columnCountLabel,
        columnCounter, rowCountLabel, rowCounter)
{
#if JUCE_DEBUG
    setName("HomeSettingsPage");
#endif
    setBackButton(BackButtonType::left);
    if (Windows::Info::inPortraitMode())
    {
        setLayout(getPortraitLayout());
    }
    else
    {
        setLayout(getLandscapeLayout());
    }

    title.setJustificationType(juce::Justification::centred);
    addAndShowLayoutComponents();
    menuController.updateForCurrentSettings();
}


// Updates AppMenu settings when the page closes.
Page::HomeSettings::~HomeSettings()
{
    menuController.applySettingsChanges();
}


// Gets the base page layout used to construct both the landscape and portrait
// mode page layouts.
Layout::Group::RelativeLayout Page::HomeSettings::getBaseLayout()
{
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(titleRowWeight,
        {
            RowItem(&title)
        }),
        Row(contentRowWeight,
        {
            RowItem(&clockLabel, labelWeight),
            RowItem(&clockModePicker, comboBoxWeight)
        }),
        Row(contentRowWeight,
        {
            RowItem(&menuFormatLabel, labelWeight),
            RowItem(&menuFormatPicker, comboBoxWeight)
        }),
        Row(contentRowWeight,
        {
            RowItem(&rowCountLabel, counterLabelWeight),
            RowItem(&rowCounter, counterWeight)
        }),
        Row(contentRowWeight,
        {
            RowItem(&columnCountLabel, counterLabelWeight),
            RowItem(&columnCounter, counterWeight)
        })
    });
    layout.setYMarginFraction(yMarginFraction);
    layout.setXPaddingWeight(xPaddingWeight);
    layout.setYPaddingWeight(rowPaddingWeight);
    return layout;
}


// Gets the landscape mode page layout.
Layout::Group::RelativeLayout Page::HomeSettings::getLandscapeLayout()
{
    using namespace Layout::Group;
    RelativeLayout layout = getBaseLayout();
    layout.insertRow(1, Row(contentRowWeight,
            {
                RowItem(&bgLabel, labelWeight),
                RowItem(&homeBGPicker, comboBoxWeight)
            }));
    return layout;
}


// Gets the portrait mode page layout.
Layout::Group::RelativeLayout Page::HomeSettings::getPortraitLayout()
{
    using namespace Layout::Group;
    RelativeLayout layout = getBaseLayout();
    layout.insertRow(1, Row(dividedRowWeight, { RowItem(&bgLabel) }));
    layout.insertRow(2, Row(dividedRowWeight, { RowItem(&homeBGPicker) }));
    return layout;
}
