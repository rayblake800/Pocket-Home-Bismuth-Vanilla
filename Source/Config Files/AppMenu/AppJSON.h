#pragma once
#include "ConfigItemData.h"
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
     * @brief   Gets the main list of application shortcuts.
     *
     * @return  A list of shortcuts to be pinned to the main column 
     *          of the AppMenu.
     */
    juce::Array<AppShortcut> getShortcuts() const;

    /**
     * @brief  Adds a new shortcut to the list of pinned application shortcuts.
     *
     * @param newApp           The new application shortcut data.
     *
     * @param index            Index to insert the shortcut into the list.
     *
     * @param writeChangesNow  Iff true, immediately write changes to the JSON
     *                         file.
     */
    void addShortcut(const AppShortcut& newShortcut, const int index,
            const bool writeChangesNow);

    /**
     * @brief  Removes a shortcut from the list of application shortcuts.
     * 
     * @param index             The index of the shortcut to remove from the 
     *                          list.
     *
     * @param writeChangesNow   Iff true, immediately write changes to the JSON
     *                          file.
     */
    void removeShortcut(const int index, const bool writeChangesNow);

    /**
     * @brief  Finds the index of an application shortcut in the list.
     * 
     * @param toFind  The application shortcut to search for in the list.
     *
     * @return        The index of toFind, or -1 if it was not found in the 
     *                list.
     */
    int getShortcutIndex(const AppShortcut& toFind) const;

    /**
     * @brief   Gets the list of application folders.
     *
     * @return  A list of folders to display in the AppMenu.
     */
    juce::Array<AppFolder> getFolders() const;

    /**
     * @brief  Adds a new folder to the list of application folders.
     * 
     * @param newFolder        The new folder data.
     *
     * @param index            The index where the new folder will be inserted.
     *
     * @param writeChangesNow  Iff true, immediately write the changes to the
     *                         JSON configuration file.
     */
    void addAppFolder
    (const AppFolder& newFolder, const int index, const bool writeChangesNow);

    /**
     * @brief  Removes a folder from the list of application folders.
     * 
     * @param index            The index of the folder to remove.
     *
     * @param writeChangesNow  Iff true, immediately write changes to the JSON
     *                         configuration file.
     */
    void removeAppFolder(const int index, const bool writeChangesNow = true);

    /**
     * @brief   Finds the index of an AppFolder in the list of folders.
     * 
     * @param toFind  The folder object to search for in the list.
     *
     * @return  The index of the folder object, or -1 if it was not found.
     */
    int getFolderIndex(const AppFolder& toFind) const;

    private:
    /**
     * @brief  Copies all shortcuts and folders back to the JSON configuration 
     *         file.
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

    /* Stores application shortcuts */
    juce::Array<AppShortcut> shortcuts;

    /* Stores application folders */
    juce::Array<AppFolder> folders;
};

