#pragma once
#include "AppMenuItem.h"
#include "ConfigJSON.h"

/**
 * @file AppJSON.h
 *
 * @brief  Reads application menu settings from the apps.json configuration 
 *         file.
 *
 * AppJSON defines the class of the singleton SharedResource object that
 * accesses apps.json, the configuration file where application menu settings
 * are stored.  It reads in the JSON data used to create AppShortcut and
 * AppFolder objects, and writes any changes to those objects back to apps.json
 * as JSON data.  Only AppConfigFile objects are allowed to access the AppJSON
 * object.
 *
 * @see AppConfigFile.h
 */

class AppJSON : public ConfigJSON, private AppMenuItem::Editor

{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    AppJSON();

    virtual ~AppJSON() { }

    /**
     * @brief  Gets a menu item representing the root folder of the application
     *         menu.
     *
     * @return  The root folder menu item.
     */
    AppMenuItem getRootFolderItem() const;

    /**
     * @brief  Attempts to insert a new item into the application menu.
     *
     * This will fail if the folder is null, the new menu item is null, or the
     * insertion index isn't between zero and the folder's last movable child
     * index, inclusive.
     *
     * @param folder   The folder that will contain the new menu item.
     *
     * @param index    The index in the folder where the menu item will be
     *                 inserted.
     *
     * @param newItem  The new menu item to insert.
     *
     * @return  Whether the insertion was successful.
     */
    bool insertMenuItem(AppMenuItem& folder, const int index, 
            const AppMenuItem& newItem);

    /**
     * @brief  Replaces an item in the application menu.
     *
     * This will fail if either menu item is null, or if toReplace is not
     * in the menu at an index within its folder's movable child indices.
     *
     * @param toReplace     The menu item to replace.
     *
     * @param replacement   The new menu item to place in the menu at the old
     *                      item's position.
     *
     * @return  Whether the replacement was successful.
     */
    bool replaceMenuItem(AppMenuItem& toReplace, AppMenuItem& replacement);

    /**
     * @brief  Removes a menu item from the application menu.
     *
     * @param toRemove  The menu item to remove.
     *
     * @return  True if the menu item was removed, false if it was null or
     *          not located in the menu already.
     */
    bool removeMenuItem(AppMenuItem& toRemove);
    
    /**
     * @brief  Swaps the positions of two menu items in the same folder.
     *
     * This will fail if the menu items are not both within the folder's movable
     * child items.
     *
     * @param folder      The folder containing both menu items.
     *
     * @param folderIdx1  The index of the first menu item within the folder.
     *
     * @param folderIdx2  The index of the second menu item within the folder.
     *
     * @return   Whether two menu items were successfully swapped.
     */
    bool swapMenuItems(AppMenuItem& folder,
            const int folderIdx1, const int folderIdx2);

private:
    /**
     * @brief  Copies all menu data back to the JSON configuration file.
     */
    void writeDataToJSON() override final;

    /**
     * @brief   Gets all parameters with basic data types tracked by this
     *          ConfigFile.
     * 
     * @return  The empty list, as AppConfigFile doesn't track any ConfigKey
     *          variables, only its own custom data structures.
     */
    virtual const std::vector<ConfigKey>& getConfigKeys() const final override
    {
        static const std::vector<ConfigKey> keys = {};
        return keys;
    }
};

