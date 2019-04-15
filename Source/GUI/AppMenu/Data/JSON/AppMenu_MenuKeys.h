#ifndef APPMENU_IMPLEMENTATION
    #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file   AppMenu_MenuKeys.h
 *
 * @brief  Defines the data keys used to store menu item data in the
 *         appMenu.json configuration file.
 */

#include "JuceHeader.h"

namespace AppMenu
{
    namespace MenuKeys
    {
        //#####################    Menu Data keys:  ############################
        // The menu item's printed title.
        static const juce::Identifier titleKey("name");

        // The menu item's icon name or path.
        static const juce::Identifier iconKey("icon");

        //######## Application shortcut menu item data: ########
        // The menu item's application launch command.
        static const juce::Identifier commandKey("command");

        // Whether the menu item launches an application in a new terminal
        // window.
        static const juce::Identifier launchInTermKey("launch in terminal");

        //############## Folder menu item data: #################
        // The list of application categories used to select desktop entry
        // folder items.
        static const juce::Identifier categoryKey("categories");

        // The list of configuration-defined folder items.
        static const juce::Identifier folderItemKey("folder items");
    }
}
