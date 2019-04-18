#ifndef APPMENU_IMPLEMENTATION
    #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file   ConfigKeys.h
 *
 * @brief  Defines the data keys used to store the selected menu format and all
 *         other menu layout options.
 */

#include "AppMenu_Format.h"
#include "Config_DataKey.h"
#include "JuceHeader.h"
#include <map>

namespace AppMenu
{
    namespace ConfigKeys
    {
        //###################### Integer value keys: ###########################
        // Sets the maximum number of AppMenu::Scrolling rows per screen.
        static const Config::DataKey scrollingMenuRows
            ("scrolling AppMenu max rows", Config::DataKey::intType);

        // Sets the width in menu items of the paged AppMenu's grid of visible
        // menu items.
        static const Config::DataKey pagedMenuColumns
            ("paged AppMenu max columns", Config::DataKey::intType);

        // Sets the height in menu items of the paged AppMenu's grid of visible
        // menu items.
        static const Config::DataKey pagedMenuRows
            ("paged AppMenu max rows", Config::DataKey::intType);

        // Sets the amount of time in milliseconds to wait before assuming an
        // application isn't going to open and focus a new window.
        static const Config::DataKey timeoutPeriod
            ("application launch timeout", Config::DataKey::intType);

        //####################### String value keys: ###########################
        // Sets the AppMenuComponent type to use on the home page.
        static const Config::DataKey menuFormat
            ("AppMenu format", Config::DataKey::stringType);

        // All valid values of menuFormat
        static const std::map<Format, juce::String> formatStrings =
        {
            {Format::Scrolling, "Scrolling"},
            {Format::Paged,     "Paged"},
            {Format::Invalid,   "Invalid"}
        };

        // Keys to all basic data values:
        static const std::vector<Config::DataKey> allKeys =
        {
            scrollingMenuRows,
            pagedMenuColumns,
            pagedMenuRows,
            timeoutPeriod,
            menuFormat,
        };
    }
}
