#pragma once
#include "MenuItemData.h"
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

class AppJSON : public ConfigJSON
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    AppJSON();

    virtual ~AppJSON() { }

    /**
     * @brief  Gets the number of menu items within a menu folder.
     *
     * @param folderIndex             The index of a folder in the menu tree.
     *
     * @param includeDesktopEntries   Whether menu items loaded from desktop
     *                                entry files should be included in the
     *                                count.
     *
     * @return                        The number of menu items in the folder, or
     *                                -1 if folderIndex doesn't specify a valid
     *                                folder index.
     */
    int getFolderSize(const MenuIndex& folderIndex,
            const bool includeDesktopEntries = true) const;

    /**
     * @brief  Gets all menu items within a folder in the application menu.
     *
     * Menu items may be defined as objects containing menu data, or as paths to
     * desktop entry files.
     *
     * @param folderIndex  The index of the folder within the menu tree.
     *
     * @return             A list of menu items to be pinned to the root folder 
     *                     of the application menu, or the empty list if an 
     *                     invalid folder index was given.
     */
    juce::Array<juce::var> getMenuItems(const MenuIndex& folderIndex) const;

    /**
     * @brief  Adds a new menu item to the list of menu items.
     *
     * @param newApp           The new menu item's data.
     *
     * @param index            Index to insert the menu item into the menu.
     *
     * @param writeChangesNow  Whether changes should immediately be written to
     *                         the JSON file.  By default, changes will be 
     *                         written immediately.
     */
    void addMenuItem(const juce::var& newItem, 
            const MenuIndex& index,
            const bool writeChangesNow = true);

    /**
     * @brief  Removes an item from the menu.
     * 
     * @param index             The index of the menu item within the menu.
     *
     * @param writeChangesNow   Whether changes should immediately be written to
     *                          the JSON file.  By default, changes will be 
     *                          written immediately.
     */
    void removeMenuItem(const MenuIndex& index, 
            const bool writeChangesNow = true);

    /**
     * @brief  Saves a menu item's data to the list of cached menu items.
     *
     * @param menuItem  A pointer to menu item data that should be saved.
     */
    void addCachedMenuItem(MenuItemData::Ptr menuItem);

    /**
     * @brief  Loads a cached menu item from the list of cached menu items.
     *
     * @param cacheIndex  The index of an item in the menu.
     *
     * @return            A pointer to cached menu data, or nullptr if the
     *                    requested menu item has not been cached.
     */
    MenuItemData::Ptr getCachedMenuItem(const MenuIndex& cacheIndex);

    /**
     * @brief  Removes all cached menu items.
     */
    void clearMenuItemCache();

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

    /**
     * @brief  Finds a menu item within the menu tree and returns its data.
     *
     * @param menuIndex  The index of a menu item within the menu tree.
     *
     * @return           The menu item's JSON data, or an empty var if no folder
     *                   exists at the given index.
     */
    juce::var getItemVar(const MenuIndex& menuIndex) const;
    
    /**
     * @brief  Finds a menu item within the menu tree and return its parent's
     *         data.
     *
     * @param menuIndex  The index of a folder item in the menu.
     *
     * @return           The data of the menu item directly above the index, or
     *                   an empty var if the parent item does not exist.
     */
    juce::var getParentVar(const MenuIndex& menuIndex) const;

    /**
     * @brief  Gets a menu item's folder data.
     *
     * @param folderIndex  The index of a folder menu item.
     *
     * @return             A var array of menu item var objects, or an empty var
     *                     if no folder exists at the given index.
     */
    juce::var getFolderVar(const MenuIndex& folderIndex) const;
    
    /**
     * @brief  Update cached item indices when a menu item is inserted or
     *         removed.
     *
     * @param changePoint   The index where a new menu item was inserted or 
     *                      deleted.
     *
     * @param itemInserted  True if a new item was inserted, false if an
     *                      existing item was removed.
     */
    void updateCacheIndices(const MenuIndex& changePoint,
            const bool itemInserted);

    /* Holds all JSON menu data */
    juce::var menuItems;

    /* Holds cached menu item data objects */
    std::map<MenuIndex, MenuItemData::Ptr> menuItemCache;
};

