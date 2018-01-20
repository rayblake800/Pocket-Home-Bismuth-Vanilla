/*
  ==============================================================================

    AppConfigFile.h
    Created: 5 Jan 2018 9:53:32pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "ConfigFile.h"

class AppConfigFile : public ConfigFile {
public:
    AppConfigFile();
    virtual ~AppConfigFile();

    Array<String> getStringKeys() const;
    Array<String> getBoolKeys() const;
    //######################### Application Data ###############################

    /**
     *Represents an application pinned to the main menu
     */
    struct AppItem {
        AppItem();
        AppItem(var jsonObj);
        String name;
        String icon;
        String shell;
        bool launchInTerminal;
        int index;
        int folderIndex;
        DynamicObject * getDynamicObject();
        bool operator==(const AppItem& rhs)const;
    };
    
    /**
     * @return a list of AppItems to be pinned to the main column 
     * of the AppMenu
     */
    Array<AppItem> getFavorites();

    /**
     * Add a new app to the list of pinned favorite apps in the config file
     * @param newApp new application information
     * @param index position to insert the new application
     */
    void addFavoriteApp(AppItem newApp, int index);

    /**
     * Remove an app from the list of favorite applications
     * @param index position of the app to remove
     */
    void removeFavoriteApp(int index);

    //######################### Folder/Category Data ###########################

    /**
     * Represents an application folder
     * TODO:add sub-folders
     */
    struct AppFolder {
        AppFolder();
        AppFolder(var jsonObj,int index);
        String name;
        Array<String> categories;
        String icon;
        Array<AppItem> pinnedApps;
        int index;
        DynamicObject * getDynamicObject();
        bool operator==(const AppFolder& rhs) const;
    };
    
    /**
     * @return A list of folders to display in the AppMenu 
     */
    Array<AppFolder> getFolders();

    /**
     * Add a new folder to the list of AppFolders in the config file
     * @param newFolder defines the new folder
     * @param index where to insert the new folder
     */
    void addAppFolder(AppFolder newFolder, int index);
    
    /**
     * Remove a folder from the list of AppFolders
     * @param index position of the folder to remove
     */
    void removeAppFolder(int index);
    
    /**
     * Add a new app to the top of an AppFolder
     * @param newApp defines the new application
     * @param folderIndex which folder holds the new pinned app
     * @param appIndex index of the new app in the folder's pinned apps
     */
    void addPinnedApp(AppItem newApp, int folderIndex, int appIndex);

    /**
     * Remove a pinned app from an AppFolder
     * @param folderIndex the position of the folder
     * @param appIndex the position of the pinned app in the folder
     */
    void removePinnedApp(int folderIndex, int appIndex);

private:

    /**
     * Read in this object's data from a json config object
     * 
     * @param config json data from ~/.pocket-home/filename.json
     * 
     * @param defaultConfig default json config data from the filename.json
     * in assets. If this value is var::null and default data is needed, this 
     * method will open it as the appropriate default config file from assets
     */
    void readDataFromJson(var& config, var& defaultConfig) override final;

    /**
     * Copy all config data to a json object
     */
    void copyDataToJson(DynamicObject::Ptr jsonObj) override final;

    Array<AppItem> favoriteApps;
    Array<AppFolder> categoryFolders;
    static constexpr const char* filenameConst = "apps.json";
    static constexpr const char* FAVORITES_KEY = "favorites";
    static constexpr const char* FOLDERS_KEY = "folders";

};
