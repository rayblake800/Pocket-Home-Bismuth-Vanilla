#include "AppMenu_SettingsController.h"
#include "AppMenu_Format.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "AppMenu::SettingsController::";
#endif

// Localized object class key
static const juce::Identifier localeClassKey = "AppMenu::SettingsController";

// Localized text value keys:
namespace TextKey
{
    static const juce::Identifier menuType      = "menuType";
    static const juce::Identifier scrollingMenu = "scrollingMenu";
    static const juce::Identifier pagedMenu     = "pagedMenu";
    static const juce::Identifier menuColumns   = "menuColumns";
    static const juce::Identifier menuRows      = "menuRows";
}


// Smallest number of menu columns to allow:
static const constexpr int minColumns = 1;
// Largest number of menu columns to allow:
static const constexpr int maxColumns = 15;

// Smallest number of menu rows to allow:
static const constexpr int minRows = 1;
// Largest number of menu rows to allow:
static const constexpr int maxRows = 15;

// AppMenu format selection ComboBox selection IDs:
static const constexpr int pagedMenuID     = 1;
static const constexpr int scrollingMenuID = 2;

// Initializes all components to match the current AppMenu settings.
AppMenu::SettingsController::SettingsController(
        Widgets::BoundedLabel& menuFormatLabel,
        juce::ComboBox& menuFormatPicker,
        Widgets::BoundedLabel& columnCountLabel,
        Widgets::Counter& columnCounter,
        Widgets::BoundedLabel& rowCountLabel,
        Widgets::Counter& rowCounter) :
Locale::TextUser(localeClassKey),
menuFormatLabel(menuFormatLabel),
menuFormatPicker(menuFormatPicker),
columnCountLabel(columnCountLabel),
columnCounter(columnCounter),
rowCountLabel(rowCountLabel),
rowCounter(rowCounter) { }


// Updates all settings components to match the current AppMenu settings.
void AppMenu::SettingsController::updateForCurrentSettings()
{
    if (!initialized)
    {
        menuFormatLabel.setText(localeText(TextKey::menuType),
                juce::NotificationType::dontSendNotification);
        menuFormatPicker.addItem(localeText(TextKey::scrollingMenu),
                scrollingMenuID);
        menuFormatPicker.addItem(localeText(TextKey::pagedMenu),
                pagedMenuID);
        columnCountLabel.setText(localeText(TextKey::menuColumns),
                juce::NotificationType::dontSendNotification);
        columnCounter.setMinimum(minColumns);
        columnCounter.setMaximum(maxColumns);
        rowCountLabel.setText(localeText(TextKey::menuRows),
                juce::NotificationType::dontSendNotification);
        rowCounter.setMinimum(minRows);
        rowCounter.setMaximum(maxRows);
        menuFormatPicker.addListener(this);
        initialized = true;
    }
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


// Saves the settings selected by the SettingsController so they will be
// applied to the AppMenu.
void AppMenu::SettingsController::applySettingsChanges()
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


// Updates counter components when the format selection box changes.
void AppMenu::SettingsController::comboBoxChanged(juce::ComboBox* box)
{
    updateCountersForSelectedFormat();
}


// Updates the column and row counters for the current format shown by the
// menuFormatPicker.
void AppMenu::SettingsController::updateCountersForSelectedFormat()
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
