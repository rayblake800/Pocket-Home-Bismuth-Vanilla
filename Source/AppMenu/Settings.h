#pragma once
#include "AppMenu/AppMenu.h"


/**
 * @file  AppMenu/Settings.h
 *
 * @brief  Gets and sets user-configurable menu settings such as selected menu
 *         format and menu dimensions.
 */
namespace AppMenu
{
    namespace Settings
    {
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
         *           at once.
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
         *           once.
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
         *           show at once.
         */
        int getScrollingMenuRows();

        /**
         * @brief  Sets the number of menu item rows the Scrolling menu displays 
         *         at one time.
         *
         * @param numRows  The new row count to save to the configuration file.
         */
        void setScrollingMenuRows(const int numRows);
    }
}
