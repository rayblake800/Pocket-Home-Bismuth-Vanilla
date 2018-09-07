#pragma once
#include "ConfigFile.h"
#include "AppJSON.h"
#include "AppItem.h"
#include "AppFolder.h"

/**
 * @file AppConfigFile.h
 * 
 * @brief Loads the pinned application links and folders displayed in the 
 *        AppMenuComponent.
 * 
 * @see AppMenuComponent.h
 */

class AppConfigFile : public ConfigFile<AppJSON>
{
public:
    AppConfigFile() { }

    virtual ~AppConfigFile() { }

    /**
     * Gets the main list of application shortcuts. 
     * 
     * @return  A list of AppItems to be pinned to the main folder
     *          of the AppMenu.
     */
    juce::Array<AppItem> getFavorites();

    /**
     * Add a new app to the list of pinned favorite apps in the config file.
     * 
     * @param newApp            The new application data object.
     * 
     * @param index             The position to insert the new application.
     * 
     * @param writeChangesNow   Sets if the change should be written to the 
     *                           config file immediately.
     */
    void addFavoriteApp(AppItem newApp, int index, bool writeChangesNow = true);

    /**
     * Remove an app from the list of favorite applications
     * 
     * @param index            The position of the app to remove.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                         config file immediately.
     */
    void removeFavoriteApp(int index, bool writeChangesNow = true);

    /**
     * Find the index of an AppItem in favorites.
     * 
     * @param toFind  The folder to search for in the favorites list/
     * 
     * @return  The index of toFind, or -1 if it isn't in the list.
     */
    int getFavoriteIndex(AppItem toFind);
    
    
    /**
     * Gets the list of application menu folders.
     *
     * @return A list of folders to display in the AppMenu.
     */
    juce::Array<AppFolder> getFolders();

    /**
     * Add a new folder to the list of AppFolders in the config file.
     * 
     * @param newFolder        The new application folder.
     * 
     * @param index            Where to insert the new folder.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                         JSON config file immediately.
     */
    void addAppFolder
    (AppFolder newFolder, int index, bool writeChangesNow = true);

    /**
     * Remove a folder from the list of AppFolders.
     * 
     * @param index            The position of the folder to remove.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                         config file immediately.
     */
    void removeAppFolder(int index, bool writeChangesNow = true);

    /**
     * Find the index of an AppFolder in the list of folders.
     * 
     * @param toFind   The folder to search for in the folder list.
     * 
     * @return  The index of toFind, or -1 if it isn't in the list.
     */
    int getFolderIndex(AppFolder toFind);
};
