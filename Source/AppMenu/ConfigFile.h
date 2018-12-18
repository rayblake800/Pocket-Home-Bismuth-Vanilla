#pragma once
/**
 * @file  AppMenu/MenuFile.h
 * 
 * @brief  Reads and edits the selected AppMenu format and menu layout 
 *         preferences.
 */ 

#include "Config/FileHandler.h"
#include "Config/Listener.h"

namespace AppMenu { class ConfigFile; }
namespace AppMenu { class ConfigJSON; }
namespace AppMenu { enum class Format; }

/**
 *  AppMenu::ConfigFile provides convenient methods to read and write values
 * defined in the menuOptions.json file. This file holds the selected 
 * AppMenu::Format, along with other menu format options, as defined in 
 * AppMenu::ConfigKeys.  
 * 
 * @see  AppMenu/AppMenu.h
 *       AppMenu/Data/JSON/ConfigKeys.h
 *       AppMenu/Data/JSON/MenuJSON.h
 */
class AppMenu::ConfigFile : private Config::FileHandler<AppMenu::ConfigJSON>
{
public:
    ConfigFile(); 

    virtual ~ConfigFile() { }
        
    /**
     * @brief  Gets the selected menu format.
     *
     * @return  The current AppMenu format selection.
     */
    AppMenu::Format getMenuFormat();

    /**
     * @brief  Saves the new menu format selection.
     *
     * @param newFormat  The new menu format to save to configuration files.
     */
    void setMenuFormat(const AppMenu::Format newFormat);

    /**
     * @brief  Gets the string representation of an AppMenu::Format value.
     *
     * @param format  An AppMenu format type.
     *
     * @return        The format's string representation.
     */
    juce::String formatToString(const AppMenu::Format format);

    /**
     * @brief  Gets an AppMenu::Format value from its string representation.
     *
     * @param formatString  An AppMenu format string.
     *
     * @return              The corresponding format value, or
     *                      Format::Invalid if formatString is not a valid
     *                      format string.
     */
    AppMenu::Format stringToFormat(const juce::String formatString);

    /* Paged menu format settings: */

    /**
     * @brief  Gets the number of menu item columns displayed at one time
     *         by the Paged AppMenu format.
     *
     * @return   The maximum number of menu columns the Paged menu will show
     *           at once. This value will never be less than one.
     */
    int getPagedMenuColumns();

    /**
     * @brief  Sets the number of menu item columns the Paged menu displays
     *         at one time.
     *
     * @param numColumns  The new column count to save to the configuration
     *                    file.
     */
    void setPagedMenuColumns(const int numColumns);

    /**
     * @brief  Gets the number of menu item rows displayed at one time by 
     *         the Paged AppMenu format.
     *
     * @return   The maximum number of menu rows the Paged menu will show at
     *           once.  This value will never be less than one.
     */
    int getPagedMenuRows();

    /**
     * @brief  Sets the number of menu item rows the Paged menu displays at 
     *         one time.
     *
     * @param numRows  The new row count to save to the configuration file.
     */
    void setPagedMenuRows(const int numRows);

    /* Scrolling menu format settings: */

    /**
     * @brief  Gets the number of menu item rows displayed at one time by 
     *         the Scrolling AppMenu format.
     *
     * @return   The maximum number of menu rows the Scrolling menu will 
     *           show at once.  This value will never be less than one.
     */
    int getScrollingMenuRows();

    /**
     * @brief  Sets the number of menu item rows the Scrolling menu displays 
     *         at one time.
     *
     * @param numRows  The new row count to save to the configuration file.
     */
    void setScrollingMenuRows(const int numRows);
    
    /* Listens for changes to AppMenu::MenuFile's basic data values */
    class Listener : public Config::Listener<ConfigJSON>
    {
    public:
        Listener();

        virtual ~Listener() { }
    };
};
