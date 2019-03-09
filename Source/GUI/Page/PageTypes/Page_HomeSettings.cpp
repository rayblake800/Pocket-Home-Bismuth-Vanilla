#include "Page_HomeSettings.h"
#include "Config_MainFile.h"
#include "Config_MainKeys.h"
#include "AppMenu_ConfigFile.h"
#include "AppMenu_Format.h"

/* Localized object class key */
static const juce::Identifier localeClassKey = "Page::HomeSettings";

/* Localized text value keys: */
namespace TextKey
{
    static const juce::Identifier title      = "title";
    static const juce::Identifier background = "background";
    static const juce::Identifier clockMode  = "clockMode";
}

/* Page layout constants: */
static const constexpr int titleRowWeight   = 30;
static const constexpr int contentRowWeight = 20;
static const constexpr int rowPaddingWeight = 3;

static const constexpr int labelWeight    = 10;
static const constexpr int comboBoxWeight = 10;

static const constexpr int counterLabelWeight = 20;
static const constexpr int counterWeight      = 10;

static const constexpr int xPaddingWeight = 1;
static const constexpr float yMarginFraction = 0.05;

/*
 * Initializes the page layout.
 */
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
    using namespace Layout::Group;
    RelativeLayout layout({
        Row(titleRowWeight,
        { 
            RowItem(&title)
        }),
        Row(contentRowWeight,
        {
            RowItem(&bgLabel, labelWeight),
            RowItem(&homeBGPicker, comboBoxWeight)
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
    setLayout(layout);

    title.setJustificationType(juce::Justification::centred);
    addAndShowLayoutComponents();
    menuController.updateForCurrentSettings();
}

/*
 * Updates AppMenu settings when the page closes.
 */
Page::HomeSettings::~HomeSettings()
{
    menuController.applySettingsChanges();
}
