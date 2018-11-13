#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Implementation.h"
#include "AppMenu/Components/MainComponent.h"
#include "AppMenu/Data/JSON/ConfigFile.h"
#include "AppMenu/Data/JSON/ConfigKeys.h"
#include "AppMenu/AppMenu.h"

/*
 * Creates an AppMenu::MainComponent, initialized with the menu format saved 
 * through AppMenu::Settings.
 */
juce::Component* AppMenu::createAppMenu()
{
    return new MainComponent();
}

/*
 * Gets the selected menu format.
 */
AppMenu::Format AppMenu::Settings::getMenuFormat()
{
    ConfigFile appConfig;
    return stringToFormat(appConfig.getConfigValue<juce::String>
            (ConfigKeys::menuFormatKey));
}

/*
 * Saves the new menu format selection.
 */
void AppMenu::Settings::setMenuFormat(const AppMenu::Format newFormat)
{
    if(newFormat != Format::Invalid)
    {
        ConfigFile appConfig;
        appConfig.setConfigValue<juce::String>(ConfigKeys::menuFormatKey, 
                formatToString(newFormat));
    }
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
int AppMenu::Settings::getPagedMenuColumns()
{
    ConfigFile appConfig;
    return std::max(1,
            appConfig.getConfigValue<int>(ConfigKeys::pagedMenuColumnsKey));
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
    return std::max(1,
            appConfig.getConfigValue<int>(ConfigKeys::pagedMenuRowsKey));
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
    return std::max(1,
            appConfig.getConfigValue<int>(ConfigKeys::scrollingMenuRowsKey));
}

/*
 * Sets the number of menu item rows the Scrolling menu displays at one time.
 */
void AppMenu::Settings::setScrollingMenuRows(const int numRows)
{
    ConfigFile appConfig;
    appConfig.setConfigValue<int>(ConfigKeys::scrollingMenuRowsKey, numRows);
}
