#include "AppMenu_SettingsController.h"
#include "AppMenu_Format.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "AppMenu::SettingsController::";
#endif

// Localized object class key
static const juce::Identifier localeClassKey = "AppMenu::SettingsController";

/**
 * @brief  Creates an association between an AppMenu::Format value, its
 *         localized name text key, and its index in the menuFormatPicker
 *         ComboBox.  FormatKey objects are immutable.
 */
struct FormatKey
{
public:
    /**
     * @brief  Stores a menu format with its localized name key, setting its
     *         ComboBox ID to the next unused integer ID greater than zero.
     *
     * @param formatType  One of the AppMenu format options.
     *
     * @param localeKey   The key used to load the format's localized name.
     */
    FormatKey(const AppMenu::Format formatType, const juce::String& localeKey);

    // The AppMenu format type:
    const AppMenu::Format formatType;
    // The localized format name key: 
    const juce::Identifier localeKey;
    // The format's menuFormatPicker ComboBox ID:
    const int menuID;

    /**
     * @brief  Gets the number of available menu formats.
     *
     * @return  The number of menu formats that have been declared as FormatKey
     *          data. This assumes that each AppMenu::Format is only ever
     *          assigned to a single FormatKey.
     */
    static int getFormatCount();

private:
    // Stores the number of FormatKeys that have been constructed.
    static int idCount;
};
int FormatKey::idCount = 0;


//  Stores a menu format with its localized name key, setting its ComboBox ID to
//  the next unused integer ID greater than zero.
FormatKey::FormatKey
(const AppMenu::Format formatType, const juce::String& localeKey) :
formatType(formatType),
localeKey(localeKey),
menuID(idCount + 1)
{
    idCount++;
}


// Gets the number of available menu formats.
int FormatKey::getFormatCount()
{
    return idCount;
}


// Localized text value keys:
namespace TextKey
{
    // Menu format keys:
    const FormatKey formats[] =
    {
        FormatKey(AppMenu::Format::Scrolling, "scrollingMenu"),
        FormatKey(AppMenu::Format::Paged, "pagedMenu")
    };

    static const juce::Identifier menuType      = "menuType";
    static const juce::Identifier menuColumns   = "menuColumns";
    static const juce::Identifier menuRows      = "menuRows";
}

// A function that returns whether a given FormatKey matches some criteria.
typedef std::function<bool(const FormatKey&)> FormatKeyCheck;

/**
 * @brief  Finds the first menu FormatKey that is validated by a match function.
 *
 * @param checkAction  A function that returns true if its FormatKey parameter
 *                     meets its criteria.
 *
 * @return             The first FormatKey in the list that checkAction marks
 *                     as a match, or a null value if no match is found.
 */
static const FormatKey* findMatchingKey(const FormatKeyCheck checkAction)
{
    for (int i = 0; i < FormatKey::getFormatCount(); i++)
    {
        if (checkAction(TextKey::formats[i]))
        {
            return &TextKey::formats[i];
        }
    }
    return nullptr;
}


// Smallest number of menu columns to allow:
static const constexpr int minColumns = 1;
// Largest number of menu columns to allow:
static const constexpr int maxColumns = 15;

// Smallest number of menu rows to allow:
static const constexpr int minRows = 1;
// Largest number of menu rows to allow:
static const constexpr int maxRows = 15;

// AppMenu format ComboBox selection IDs:
// static const constexpr int pagedMenuID     = 1;
// static const constexpr int scrollingMenuID = 2;

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
        // Add format options:
        for (int i = 0; i < FormatKey::getFormatCount(); i++)
        {
            menuFormatPicker.addItem(localeText(TextKey::formats[i].localeKey),
                    TextKey::formats[i].menuID);
        }

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
    const FormatKey* activeKey;
    if ((activeKey = findMatchingKey([menuFormat](const FormatKey& key)
            { return key.formatType == menuFormat; })))
    {
        menuFormatPicker.setSelectedId(activeKey->menuID,
                juce::NotificationType::dontSendNotification);
    }
    else
    {
        DBG(dbgPrefix << __func__ 
                << ": Failed to find format key for current menu format.");
    }
    updateCountersForSelectedFormat();
}


// Saves the settings selected by the SettingsController so they will be
// applied to the AppMenu.
void AppMenu::SettingsController::applySettingsChanges()
{
    const int selectedId = menuFormatPicker.getSelectedId();
    const FormatKey* selectedKey;
    if ((selectedKey = findMatchingKey([selectedId](const FormatKey& key)
            { return key.menuID == selectedId; })))
    {
        formatConfig.setMenuFormat(selectedKey->formatType);
        if (selectedKey->formatType == AppMenu::Format::Scrolling)
        {
            formatConfig.setScrollingMenuRows(rowCounter.getCount());
        }
        else if (selectedKey->formatType == AppMenu::Format::Paged)
        {
            formatConfig.setPagedMenuColumns(columnCounter.getCount());
            formatConfig.setPagedMenuRows(rowCounter.getCount());
        }
        else
        {
            DBG(dbgPrefix << __func__ << ": Warning: unhandled menu format "
                    << selectedKey->localeKey.toString());
        }
    }
    else
    {
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
    const FormatKey* selectedKey;
    if ((selectedKey = findMatchingKey([selectedId](const FormatKey& key)
            { return key.menuID == selectedId; })))
    {
        if (selectedKey->formatType == Format::Scrolling)
        {
            rowCounter.setCount(formatConfig.getScrollingMenuRows());
            columnCountLabel.setVisible(false);
            columnCounter.setVisible(false);
        }
        else if (selectedKey->formatType == Format::Paged)
        {
            rowCounter.setCount(formatConfig.getPagedMenuRows());
            columnCounter.setCount(formatConfig.getPagedMenuColumns());
            columnCountLabel.setVisible(true);
            columnCounter.setVisible(true);
        }
        else
        {
            rowCountLabel.setVisible(false);
            rowCounter.setVisible(false);
            columnCountLabel.setVisible(false);
            columnCounter.setVisible(false);
            DBG(dbgPrefix << __func__ << ": Warning: unhandled menu format "
                    << selectedKey->localeKey.toString());
        }
    }
    else
    {
            DBG(dbgPrefix << __func__ << ": Found invalid format selection ID "
                    << selectedId);
            jassertfalse;
    }
}
