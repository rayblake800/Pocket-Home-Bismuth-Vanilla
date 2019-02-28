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
    static const juce::Identifier title           = "title";
    static const juce::Identifier background      = "background";
}

/*
 * Initializes the page layout.
 */
Page::HomeSettings::HomeSettings() :
Locale::TextUser(localeClassKey),
title("personalizeTitle", localeText(TextKey::title)),
bgLabel("bgLabel", localeText(TextKey::background))
{
#if JUCE_DEBUG
    setName("HomeSettingsPage");
#endif
    setBackButton(BackButtonType::left);
    using namespace Layout::Group;
    RelativeLayout layout({
        Row(30,
        { 
            RowItem(&title)
        }),
        Row(20,
        {
            RowItem(&bgLabel, 10),
            RowItem(&homeBGPicker, 10)
        }),
        Row(20,
        {
            RowItem(menuComponents.getMenuFormatLabel(), 10),
            RowItem(menuComponents.getMenuFormatPicker(), 10)
        }),
        Row(20,
        {
            RowItem(menuComponents.getRowCountLabel(), 20),
            RowItem(menuComponents.getRowCounter(), 10)
        }),
        Row(20,
        {
            RowItem(menuComponents.getColumnCountLabel(), 20),
            RowItem(menuComponents.getColumnCounter(), 10)
        })
    });
    layout.setYMarginFraction(0.05);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(3);
    setLayout(layout);

    title.setJustificationType(juce::Justification::centred);
    addAndShowLayoutComponents();
    menuComponents.updateForCurrentSettings();
}

/*
 * Updates AppMenu settings when the page closes.
 */
Page::HomeSettings::~HomeSettings()
{
    menuComponents.applySettingsChanges();
}
