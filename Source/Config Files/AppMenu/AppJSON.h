#pragma once
#include "AppItem.h"
#include "AppFolder.h"
#include "ConfigJSON.h"

/**
 * @file AppJSON.h
 *
 * @brief  Reads application menu settings from the apps.json configuration 
 *         file.
 */

class AppJSON : public ConfigJSON
{
public:
    // SharedResource object key
    static const juce::Identifier resourceKey;

    AppJSON();

    virtual ~AppJSON() { }

    /**
     * Get the main list of application shortcuts.
     *
     * @return  A list of AppItems to be pinned to the main column 
     *          of the AppMenu.
     */
    juce::Array<AppItem> getShortcuts();

    /**
     * Add a new app to the list of pinned application shortcuts in the config
     * file.
     *
     * @param newApp           The new application shortcut data.
     *
     * @param index            Index to insert the shortcut into the list.
     *
     * @param writeChangesNow  Iff true, immediately write changes to the JSON
     *                         file.
     */
    void addShortcut(AppItem newApp, int index, bool writeChangesNow);

    /**
     * Remove an app from the list of application shortcuts.
     * 
     * @param index             The index of the shortcut to remove from the 
     *                          list.
     *
     * @param writeChangesNow   Iff true, immediately write changes to the JSON
     *                          file.
     */
    void removeShortcut(int index, bool writeChangesNow);

    /**
     * Find the index of an application shortcut in the list.
     * 
     * @param toFind  The application shortcut to search for in the list.
     *
     * @return  The index of toFind, or -1 if it was not found in the list.
     */
    int getFavoriteIndex(const AppItem& toFind);

    /**
     * @return A list of folders to display in the AppMenu.
     * @see AppConfigFile::getFolders
     */
    juce::Array<AppFolder> getFolders();

    /**
     * Add a new folder to the list of AppFolders in the config file.
     * @see AppConfigFile::addAppFolder
     */
    void addAppFolder
    (AppFolder newFolder, int index, bool writeChangesNow);

    /**
     * Remove a folder from the list of AppFolders.
     * @see AppConfigFile::removeAppFolder
     */
    void removeAppFolder(int index, bool writeChangesNow = true);

    /**
     * Find the index of an AppFolder in the list of folders.
     * @see AppConfigFile::getFolderIndex
     */
    int getFolderIndex(AppFolder toFind);

    private:

    /**
     * Copy all shortcuts and folders back to the JSON configuration file.
     */
    void writeDataToJSON() override final;

    /**
     * Gets all parameters with basic data types tracked by this ConfigFile.
     * 
     * @return  The empty list, as AppConfigFile doesn't track any ConfigKey
     *          variables, only its own custom data structures.
     */
    const std::vector<ConfigKey>& getDataKeys() const
    {
        static const std::vector<ConfigKey> keys = {};
        return keys;
    }

    //Stores application shortcuts
    juce::Array<AppItem> favoriteApps;

    //Stores application folders
    juce::Array<AppFolder> categoryFolders;
};

