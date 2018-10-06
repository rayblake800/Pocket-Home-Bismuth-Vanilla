#pragma once
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

    int getFolderSize(const juce::Array<int> 

    /**
     * @brief  Gets all menu items within a folder in the application menu.
     *
     * Menu items may be defined as objects containing menu data, or as paths to
     * desktop entry files.
     *
     * @param folderIndex  The index of the folder within the menu tree, or the
     *                     root folder by default.
     *
     * @return             A list of menu items to be pinned to the root folder 
     *                     of the application menu, or the empty list if an 
     *                     invalid folder index was given.
     */
    juce::Array<juce::var> getMenuItems
    (const juce::Array<int> folderIndex = {}) const;

    /**
     * @brief  Adds a new menu item to the list of menu items.
     *
     * @param newApp           The new menu item's data.
     *
     * @param index            Index to insert the menu item into the folder.
     *                         If this index is invalid, the closest valid index
     *                         will be used.
     *
     * @param folderIndex      The index of the folder where the menu item will
     *                         be added within the menu tree.  This selects the
     *                         root menu folder by default.  If this is not a
     *                         valid folder index, the closest valid index will
     *                         be used.
     *
     * @param writeChangesNow  Whether changes should immediately be written to
     *                         the JSON file.  By default, changes will be 
     *                         written immediately.
     */
    void addMenuItem(const juce::var& newItem, 
            const int index,
            const juce::Array<int> folderIndex = {},
            const bool writeChangesNow = true);

    /**
     * @brief  Removes an item from the menu.
     * 
     * @param index             The index of the menu item within its folder.
     *
     * @param folderIndex       The index of the menu item's folder within the
     *                          menu tree.  If not provided, the root folder is
     *                          used by default.
     *
     * @param writeChangesNow   Whether changes should immediately be written to
     *                          the JSON file.  By default, changes will be 
     *                          written immediately.
     */
    void removeMenuItem(const int index, 
            const juce::Array<int> folderIndex = {},
            const bool writeChangesNow = true);

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

    /* Holds all JSON menu data */
    juce::var menuItems;
};

