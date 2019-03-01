#include "AppMenu_SettingsComponents.h"
#include "AppMenu_Format.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "AppMenu::SettingsComponents::";
#endif

/* Localized object class key */
static const juce::Identifier localeClassKey = "AppMenu::SettingsComponents";

/* Localized text value keys: */
namespace TextKeys
{
    static const juce::Identifier menuType      = "menuType";
    static const juce::Identifier scrollingMenu = "scrollingMenu";
    static const juce::Identifier pagedMenu     = "pagedMenu";
    static const juce::Identifier menuColumns   = "menuColumns";
    static const juce::Identifier menuRows      = "menuRows";
}

/* Smallest number of menu columns to allow: */
static const constexpr int minColumns = 1;
/* Largest number of menu columns to allow: */
static const constexpr int maxColumns = 15;

/* Smallest number of menu rows to allow: */
static const constexpr int minRows = 1;
/* Largest number of menu rows to allow: */
static const constexpr int maxRows = 15;

/* AppMenu format selection ComboBox selection IDs: */
static const constexpr int pagedMenuID     = 1;
static const constexpr int scrollingMenuID = 2;

/*
 * Initializes all components to match the current AppMenu settings.
 */
AppMenu::SettingsComponents::SettingsComponents() : 
Locale::TextUser(localeClassKey),
menuFormatLabel("menuFormatLabel", localeText(TextKeys::menuType)),
menuFormatPicker("menuFormatPicker"),
columnCountLabel("columnCountLabel", localeText(TextKeys::menuColumns)),
columnCounter(minColumns, minColumns, maxColumns),
rowCountLabel("rowCountLabel", localeText(TextKeys::menuRows)),
rowCounter(minRows, minRows, maxRows)
{
    menuFormatPicker.addItem(localeText(TextKeys::scrollingMenu),
            scrollingMenuID);
    menuFormatPicker.addItem(localeText(TextKeys::pagedMenu),
            pagedMenuID);
    menuFormatPicker.addListener(this);
    updateForCurrentSettings();
}

/*
 * Updates all settings components to match the current AppMenu settings.
 */
void AppMenu::SettingsComponents::updateForCurrentSettings()
{
    Format menuFormat = formatConfig.getMenuFormat();
    switch(menuFormat)
    {
        case Format::Scrolling:
            menuFormatPicker.setSelectedId(scrollingMenuID,
                    juce::NotificationType::dontSendNotification);
            break;
        case Format::Paged:
            menuFormatPicker.setSelectedId(pagedMenuID,
                    juce::NotificationType::dontSendNotification);
            break;
        case Format::Invalid:
            DBG(dbgPrefix << __func__ << ": Found invalid saved menu format.");
            jassertfalse;
    }
    updateCountersForSelectedFormat();
}

/*
 * Saves the settings selected by the SettingsComponents so they will be
 * applied to the AppMenu.
 */
void AppMenu::SettingsComponents::applySettingsChanges()
{
    const int selectedId = menuFormatPicker.getSelectedId();
    switch(selectedId)
    {
        case scrollingMenuID:
            formatConfig.setMenuFormat(Format::Scrolling);
            formatConfig.setScrollingMenuRows(rowCounter.getCount());
            break;
        case pagedMenuID:
            formatConfig.setMenuFormat(Format::Paged);
            formatConfig.setPagedMenuColumns(columnCounter.getCount());
            formatConfig.setPagedMenuRows(rowCounter.getCount());
            break;
        default:
            DBG(dbgPrefix << __func__ << ": Found invalid format selection ID "
                    << selectedId);
            jassertfalse;
    }
}

/*
 * Gets the label used to identify the AppMenu format selection component.
 */
juce::Component* AppMenu::SettingsComponents::getMenuFormatLabel()
{
    return &menuFormatLabel;
}

/*
 * Gets the ComboBox component used to set the AppMenu format.
 */
juce::Component* AppMenu::SettingsComponents::getMenuFormatPicker()
{
    return &menuFormatPicker;
}

/*
 * Gets the label used to identify the AppMenu column count component.
 */
juce::Component* AppMenu::SettingsComponents::getColumnCountLabel()
{
    return &columnCountLabel;
}

/*
 * Gets the counter used to set the number of application columns shown by the
 * AppMenu.
 */
juce::Component* AppMenu::SettingsComponents::getColumnCounter()
{
    return &columnCounter;
}

/*
 * Gets the label used to identify the AppMenu row count component.
 */
juce::Component* AppMenu::SettingsComponents::getRowCountLabel()
{
    return &rowCountLabel;
}

/*
 * Gets the counter used to set the number of application rows shown by the
 * AppMenu.
 */
juce::Component* AppMenu::SettingsComponents::getRowCounter()
{
    return &rowCounter;
}

/*
 * Updates counter components when the format selection box changes.
 */
void AppMenu::SettingsComponents::comboBoxChanged(juce::ComboBox* box)
{
    updateCountersForSelectedFormat();
}

/*
 * Updates the column and row counters for the current format shown by the
 * menuFormatPicker.
 */
void AppMenu::SettingsComponents::updateCountersForSelectedFormat()
{
    const int selectedId = menuFormatPicker.getSelectedId();
    switch(selectedId)
    {
        case scrollingMenuID:
            rowCounter.setCount(formatConfig.getScrollingMenuRows());
            columnCountLabel.setVisible(false);
            columnCounter.setVisible(false);
            break;
        case pagedMenuID:
            rowCounter.setCount(formatConfig.getPagedMenuRows());
            columnCounter.setCount(formatConfig.getPagedMenuColumns());
            columnCountLabel.setVisible(true);
            columnCounter.setVisible(true);
            break;
        default:
            DBG(dbgPrefix << __func__ << ": Found invalid format selection ID "
                    << selectedId);
            jassertfalse;
    }
}
