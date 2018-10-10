#pragma once
#include "ConfigFile.h"
#include "AppJSON.h"
#include "AppMenuItem.h"

/**
 * @file AppConfigFile.h
 * 
 * @brief  Reads and edits the tree of pinned application shortcuts and folders 
 *         displayed in the AppMenuComponent.
 *
 * The apps.json file defines a tree of menu items.  Each menu item can hold 
 * either an application launch command, or a list of other menu items and
 * application categories to show in a new menu folder.  All menu items have a
 * displayed title and icon.
 *
 * AppConfigFile uses this JSON menu data to construct the AppMenuItem objects
 * used to build the AppMenuComponent.  It also provides an interface for 
 * adding, editing, or removing menu data from the JSON file.
 * 
 * @see  AppMenuComponent.h
 */

class AppConfigFile : public ConfigFile<AppJSON>
{
public:
    AppConfigFile() { }

    virtual ~AppConfigFile() { }
    
    /**
     * @brief  Gets a menu item representing the root folder of the application
     *         menu.
     *
     * @return  A folder menu item holding the top level of the application
     *          menu tree.
     */
    AppMenuItem getRootMenuItem();

    /**
     * @brief  Gets all menu items within a menu folder.
     *
     * @param folderIndex  The index of the menu folder that should be accessed.
     *
     * @return             All menu items within the menu folder, or an empty
     *                     array if the folder index is invalid.
     */
    juce::Array<AppMenuItem> getMenuItems(const MenuIndex& folderIndex);

    /**
     * @brief  Adds a new menu item to the list of menu items.
     *
     * @param title            The title to print on the menu item.
     *
     * @param icon             The name or path of the menu item's icon.
     *
     * @param command          The menu item's application launch command, or 
     *                         the empty string if the menu item does not launch 
     *                         an application.
     *
     * @param launchInTerm     Whether the menu item launches an application
     *                         within a new terminal window.
     * 
     * @param categories       A list of application categories associated with
     *                         the menu item.
     *
     * @param index            The index where the menu item will be inserted
     *                         into the menu.
     *
     * @param writeChangesNow  Whether changes should immediately be written to
     *                         the JSON file.  By default, changes will be 
     *                         written immediately.
     */
    void addMenuItem(
            const juce::String& title, 
            const juce::String& icon,
            const juce::String& command,
            const bool launchInTerm,
            const juce::StringArray& categories,
            const MenuIndex& index,
            const bool writeChangesNow = true);

    /**
     * @brief  Removes all cached menu items.
     */
    void clearMenuItemCache();
};
