/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "JuceHeader.h"

/**
 * @file   ConfigKeys.h
 *
 * @brief  Defines the data keys used to store menu item data in JSON objects.
 */

namespace AppMenu
{
    /* The menu item's printed title. */
    static const juce::Identifier titleKey("name");

    /* The menu item's icon name or path. */
    static const juce::Identifier iconKey("icon");

    //######## Application shortcut menu item data: ########
    /* The menu item's application launch command. */
    static const juce::Identifier commandKey("command");

    /* Whether the menu item launches an application in a new terminal window.*/
    static const juce::Identifier launchInTermKey("launch in terminal");

    //######## Folder menu item data: ########
    /* The list of application categories used to select desktop entry folder 
      items. */
    static const juce::Identifier categoryKey("categories");

    /* The list of configuration-defined folder items. */
    static const juce::Identifier folderItemKey("folder items");
}

/* Only include this file directly in the AppMenu implementation! */
#endif
