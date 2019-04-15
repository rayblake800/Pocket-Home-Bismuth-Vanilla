#define APPMENU_IMPLEMENTATION
#include "AppMenu_ConfigFile.h"
#include "AppMenu_ConfigJSON.h"
#include "AppMenu_ConfigKeys.h"

AppMenu::ConfigFile::ConfigFile() { }

AppMenu::ConfigFile::Listener::Listener() { }


// Gets the selected menu format.
AppMenu::Format AppMenu::ConfigFile::getMenuFormat()
{
    return stringToFormat(getConfigValue<juce::String>(
                ConfigKeys::menuFormat));
}


// Saves the new menu format selection.
void AppMenu::ConfigFile::setMenuFormat(const AppMenu::Format newFormat)
{
    if (newFormat != Format::Invalid)
    {
        setConfigValue<juce::String>(ConfigKeys::menuFormat,
                formatToString(newFormat));
    }
}


// Gets the string representation of an AppMenu::Format value.
juce::String AppMenu::ConfigFile::formatToString(const AppMenu::Format format)
{
    return ConfigKeys::formatStrings.at(format);
}


// Gets an AppMenu::Format value from its string representation.
AppMenu::Format AppMenu::ConfigFile::stringToFormat
(const juce::String formatString)
{
    for (auto iter = ConfigKeys::formatStrings.begin();
            iter != ConfigKeys::formatStrings.end(); iter++)
    {
        if (formatString == iter->second)
        {
            return iter->first;
        }
    }
    return Format::Invalid;
}


// Gets the number of menu item columns displayed at one time by the Paged
// AppMenu format.
int AppMenu::ConfigFile::getPagedMenuColumns()
{
    return std::max(1, getConfigValue<int>(ConfigKeys::pagedMenuColumns));
}


// Sets the number of menu item columns the Paged menu displays at one time.
void AppMenu::ConfigFile::setPagedMenuColumns(const int numColumns)
{
    setConfigValue<int>(ConfigKeys::pagedMenuColumns, numColumns);
}


// Gets the number of menu item rows displayed at one time by the Paged AppMenu
// format.
int AppMenu::ConfigFile::getPagedMenuRows()
{
    return std::max(1, getConfigValue<int>(ConfigKeys::pagedMenuRows));
}


// Sets the number of menu item rows the Paged menu displays at one time.
void AppMenu::ConfigFile::setPagedMenuRows(const int numRows)
{
    setConfigValue<int>(ConfigKeys::pagedMenuRows, numRows);
}


// Gets the number of menu item rows displayed at one time by the Scrolling
// AppMenu format.
int AppMenu::ConfigFile::getScrollingMenuRows()
{
    return std::max(1, getConfigValue<int>(ConfigKeys::scrollingMenuRows));
}


// Sets the number of menu item rows the Scrolling menu displays at one time.
void AppMenu::ConfigFile::setScrollingMenuRows(const int numRows)
{
    setConfigValue<int>(ConfigKeys::scrollingMenuRows, numRows);
}
