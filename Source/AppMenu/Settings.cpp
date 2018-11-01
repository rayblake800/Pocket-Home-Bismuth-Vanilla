#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Data/ConfigFile.h"
#include "AppMenu/Data/ConfigKeys.h"
#include "AppMenu/Settings.h"

/*
 * Gets the selected menu format.
 */
AppMenu::Format AppMenu::Settings::getMenuFormat()
{
    using juce::String;
    ConfigFile appConfig;
    return stringToFormat(appConfig.getConfigValue<String>
            (ConfigKeys::menuFormatKey));
}

/*
 * Saves the new menu format selection.
 */
void AppMenu::Settings::setMenuFormat(const AppMenu::Format newFormat)
{
    using juce::String;
    ConfigFile appConfig;
    appConfig.setConfigValue<String>(ConfigKeys::menuFormatKey, 
            formatToString(newFormat));
}

/*
 * Gets the string representation of an AppMenu::Format value.
 */
juce::String AppMenu::Settings::formatToString(const AppMenu::Format format)
{
    return ConfigKeys::formatStrings.at(format);
}

/*
 * Gets an AppMenu::Format value from its string representation.
 */
AppMenu::Format AppMenu::Settings::stringToFormat
(const juce::String formatString)
{
    for(auto iter = ConfigKeys::formatStrings.begin();
            iter != ConfigKeys::formatStrings.end(); iter++)
    {
        if(formatString.equalsIgnoreCase(iter->second))
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
int AppMenu::Settings::getPagedMenuColumns()
{
    ConfigFile appConfig;
    return appConfig.getConfigValue<int>(ConfigKeys::pagedMenuColumnsKey);
}

/*
 * Sets the number of menu item columns the Paged menu displays at one time.
 */
void AppMenu::Settings::setPagedMenuColumns(const int numColumns)
{
    ConfigFile appConfig;
    appConfig.setConfigValue<int>(ConfigKeys::pagedMenuColumnsKey, numColumns);
}

/*
 * Gets the number of menu item rows displayed at one time by the Paged AppMenu 
 * format.
 */
int AppMenu::Settings::getPagedMenuRows()
{
    ConfigFile appConfig;
    return appConfig.getConfigValue<int>(ConfigKeys::pagedMenuRowsKey);
}

/*
 * Sets the number of menu item rows the Paged menu displays at one time.
 */
void AppMenu::Settings::setPagedMenuRows(const int numRows)
{
    ConfigFile appConfig;
    appConfig.setConfigValue<int>(ConfigKeys::pagedMenuRowsKey, numRows);
}

/*
 * Gets the number of menu item rows displayed at one time by the Scrolling 
 * AppMenu format.
 */
int AppMenu::Settings::getScrollingMenuRows()
{
    ConfigFile appConfig;
    return appConfig.getConfigValue<int>(ConfigKeys::scrollingMenuRowsKey);
}

/*
 * Sets the number of menu item rows the Scrolling menu displays at one time.
 */
void AppMenu::Settings::setScrollingMenuRows(const int numRows)
{
    ConfigFile appConfig;
    appConfig.setConfigValue<int>(ConfigKeys::scrollingMenuRowsKey, numRows);
}
