/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include <map>
#include "JuceHeader.h"
#include "Config/DataKey.h"
#include "AppMenu/AppMenu.h"

/**
 * @file   ConfigKeys.h
 *
 * @brief  Defines the data keys used to store menu item data in JSON objects.
 */

namespace AppMenu
{
    namespace ConfigKeys
    {
        //#####################    Menu Data keys:  ############################
        /* The menu item's printed title. */
        static const juce::Identifier titleKey("name");

        /* The menu item's icon name or path. */
        static const juce::Identifier iconKey("icon");

        //######## Application shortcut menu item data: ########
        /* The menu item's application launch command. */
        static const juce::Identifier commandKey("command");

        /* Whether the menu item launches an application in a new terminal 
           window. */
        static const juce::Identifier launchInTermKey("launch in terminal");

        //######## Folder menu item data: ########
        /* The list of application categories used to select desktop entry 
           folder items. */
        static const juce::Identifier categoryKey("categories");

        /* The list of configuration-defined folder items. */
        static const juce::Identifier folderItemKey("folder items");
        
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

        static const std::vector<Config::DataKey> allKeys =
        {
            scrollingMenuRowsKey,
            pagedMenuColumnsKey,
            pagedMenuRowsKey,
            menuFormatKey,
        };
    }
}

/* Only include this file directly in the AppMenu implementation! */
#endif
