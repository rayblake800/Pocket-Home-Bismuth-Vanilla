#ifndef APPMENU_IMPLEMENTATION
  #error File included outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file   ConfigKeys.h
 *
 * @brief  Defines the data keys used to store the selected menu format and all
 *         other menu layout options.
 */

#include "AppMenu_Format.h"
#include "Config/DataKey.h"
#include "JuceHeader.h"
#include <map>

namespace AppMenu
{
    namespace ConfigKeys
    {
        //###################### Integer value keys: ###########################
        /* Sets the maximum number of AppMenu::Scrolling rows per screen.*/
        static const Config::DataKey scrollingMenuRowsKey
            ("scrolling AppMenu max rows", Config::DataKey::intType);

        /* Sets the width in menu items of the paged AppMenu's grid of visible
           menu items. */
        static const Config::DataKey pagedMenuColumnsKey
            ("paged AppMenu max columns", Config::DataKey::intType);

        /* Sets the height in menu items of the paged AppMenu's grid of visible
           menu items. */
        static const Config::DataKey pagedMenuRowsKey
            ("paged AppMenu max rows", Config::DataKey::intType);
        
        //####################### String value keys: ###########################
        /* Sets the AppMenuComponent type to use on the home page. */
        static const Config::DataKey menuFormatKey
            ("AppMenu format", Config::DataKey::stringType);

        /* All valid values of menuFormatKey */
        static const std::map<Format, juce::String> formatStrings = 
        {
            {Format::Scrolling, "Scrolling"},
            {Format::Paged,     "Paged"},
            {Format::Invalid,   "Invalid"}
        };

        /* Keys to all basic data values: */
        static const std::vector<Config::DataKey> allKeys =
        {
            scrollingMenuRowsKey,
            pagedMenuColumnsKey,
            pagedMenuRowsKey,
            menuFormatKey,
        };
    }
}
