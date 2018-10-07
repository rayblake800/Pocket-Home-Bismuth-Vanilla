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
    juce::Array<AppMenuItem> getMenuItems(const juce::Array<int>& folderIndex);

    /**
     * @brief  Adds a new menu item to the list of items shown in a menu folder.
     * 
     * @param menuItem          The new menu item object.
     * 
     * @param index             The position to insert the new menu item.
     *
     * @param folderIndex       The index of the menu folder where the new item
     *                          should be inserted.
     * 
     * @param writeChangesNow   Sets if the change should be written to the 
     *                          config file immediately.  If omitted, changes
     *                          are written immediately by default.
     */
    void addMenuItem(
            const AppMenuItem& menuItem, 
            const int index,
            const juce::Array<int> folderIndex, 
            const bool writeChangesNow = true);
};
