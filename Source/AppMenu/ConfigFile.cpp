#define APPMENU_IMPLEMENTATION
#include "Utils.h"
#include "AppMenu/Data/JSON/ConfigJSON.h"
#include "AppMenu/Data/JSON/ConfigKeys.h"
#include "AppMenu/ConfigFile.h"

AppMenu::ConfigFile::ConfigFile() { }

AppMenu::ConfigFile::Listener::Listener() { } 

/*
 * Gets the selected menu format.
 */
AppMenu::Format AppMenu::ConfigFile::getMenuFormat()
{
    return stringToFormat(getConfigValue<juce::String>
            (ConfigKeys::menuFormatKey));
}

/*
 * Saves the new menu format selection.
 */
void AppMenu::ConfigFile::setMenuFormat(const AppMenu::Format newFormat)
{
    if(newFormat != Format::Invalid)
    {
        setConfigValue<juce::String>(ConfigKeys::menuFormatKey, 
                formatToString(newFormat));
    }
}

/*
 * Gets the string representation of an AppMenu::Format value.
 */
juce::String AppMenu::ConfigFile::formatToString(const AppMenu::Format format)
{
    return ConfigKeys::formatStrings.at(format);
}

/*
 * Gets an AppMenu::Format value from its string representation.
 */
AppMenu::Format AppMenu::ConfigFile::stringToFormat
(const juce::String formatString)
{
    for(auto iter = ConfigKeys::formatStrings.begin();
            iter != ConfigKeys::formatStrings.end(); iter++)
    {
        if(formatString == iter->second)
        {
             return iter->first;
        }
    }
    return Format::Invalid;
}

/*
 * Gets the number of menu item columns displayed at one time by the Paged 
 * AppMenu format.
 */
int AppMenu::ConfigFile::getPagedMenuColumns()
{
    return std::max(1,
            getConfigValue<int>(ConfigKeys::pagedMenuColumnsKey));
}

/*
 * Sets the number of menu item columns the Paged menu displays at one time.
 */
void AppMenu::ConfigFile::setPagedMenuColumns(const int numColumns)
{
    setConfigValue<int>(ConfigKeys::pagedMenuColumnsKey, numColumns);
}

/*
 * Gets the number of menu item rows displayed at one time by the Paged AppMenu 
 * format.
 */
int AppMenu::ConfigFile::getPagedMenuRows()
{
    return std::max(1,
            getConfigValue<int>(ConfigKeys::pagedMenuRowsKey));
}

/*
 * Sets the number of menu item rows the Paged menu displays at one time.
 */
void AppMenu::ConfigFile::setPagedMenuRows(const int numRows)
{
    setConfigValue<int>(ConfigKeys::pagedMenuRowsKey, numRows);
}

/*
 * Gets the number of menu item rows displayed at one time by the Scrolling 
 * AppMenu format.
 */
int AppMenu::ConfigFile::getScrollingMenuRows()
{
    return std::max(1,
            getConfigValue<int>(ConfigKeys::scrollingMenuRowsKey));
}

/*
 * Sets the number of menu item rows the Scrolling menu displays at one time.
 */
void AppMenu::ConfigFile::setScrollingMenuRows(const int numRows)
{
    setConfigValue<int>(ConfigKeys::scrollingMenuRowsKey, numRows);
}
