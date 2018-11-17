#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Implementation.h"
#include "AppMenu/Components/MainComponent.h"
#include "AppMenu/Data/JSON/MenuFile.h"
#include "AppMenu/Data/JSON/MenuKeys.h"
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
    MenuFile appConfig;
    return stringToFormat(appConfig.getConfigValue<juce::String>
            (MenuKeys::menuFormatKey));
}

/*
 * Saves the new menu format selection.
 */
void AppMenu::Settings::setMenuFormat(const AppMenu::Format newFormat)
{
    if(newFormat != Format::Invalid)
    {
        MenuFile appConfig;
        appConfig.setConfigValue<juce::String>(MenuKeys::menuFormatKey, 
                formatToString(newFormat));
    }
}

/*
 * Gets the string representation of an AppMenu::Format value.
 */
juce::String AppMenu::Settings::formatToString(const AppMenu::Format format)
{
    return MenuKeys::formatStrings.at(format);
}

/*
 * Gets an AppMenu::Format value from its string representation.
 */
AppMenu::Format AppMenu::Settings::stringToFormat
(const juce::String formatString)
{
    for(auto iter = MenuKeys::formatStrings.begin();
            iter != MenuKeys::formatStrings.end(); iter++)
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
    MenuFile appConfig;
    return std::max(1,
            appConfig.getConfigValue<int>(MenuKeys::pagedMenuColumnsKey));
}

/*
 * Sets the number of menu item columns the Paged menu displays at one time.
 */
void AppMenu::Settings::setPagedMenuColumns(const int numColumns)
{
    MenuFile appConfig;
    appConfig.setConfigValue<int>(MenuKeys::pagedMenuColumnsKey, numColumns);
}

/*
 * Gets the number of menu item rows displayed at one time by the Paged AppMenu 
 * format.
 */
int AppMenu::Settings::getPagedMenuRows()
{
    MenuFile appConfig;
    return std::max(1,
            appConfig.getConfigValue<int>(MenuKeys::pagedMenuRowsKey));
}

/*
 * Sets the number of menu item rows the Paged menu displays at one time.
 */
void AppMenu::Settings::setPagedMenuRows(const int numRows)
{
    MenuFile appConfig;
    appConfig.setConfigValue<int>(MenuKeys::pagedMenuRowsKey, numRows);
}

/*
 * Gets the number of menu item rows displayed at one time by the Scrolling 
 * AppMenu format.
 */
int AppMenu::Settings::getScrollingMenuRows()
{
    MenuFile appConfig;
    return std::max(1,
            appConfig.getConfigValue<int>(MenuKeys::scrollingMenuRowsKey));
}

/*
 * Sets the number of menu item rows the Scrolling menu displays at one time.
 */
void AppMenu::Settings::setScrollingMenuRows(const int numRows)
{
    MenuFile appConfig;
    appConfig.setConfigValue<int>(MenuKeys::scrollingMenuRowsKey, numRows);
}
