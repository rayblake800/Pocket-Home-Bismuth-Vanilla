/**
 * @file AppConfigFile.h
 * 
 * TODO: finish documentation
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
        bool launchInTerminal=false;
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
     * @param writeChangesNow sets if the change should be written to the 
     * config file immediately.
     */
    void addFavoriteApp(AppItem newApp, int index,bool writeChangesNow=true);

    /**
     * Remove an app from the list of favorite applications
     * @param index position of the app to remove
     * @param writeChangesNow sets if the change should be written to the 
     * config file immediately.
     */
    void removeFavoriteApp(int index,bool writeChangesNow=true);
    
    /**
     * Find the index of an AppItem in favorites.
     * @param toFind will be searched for in the favorites list
     * @return the index of toFind, or -1 if it isn't in the list.
     */
    int getFavoriteIndex(AppItem toFind);

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
     * @param writeChangesNow sets if the change should be written to the 
     * config file immediately.
     */
    void addAppFolder
    (AppFolder newFolder, int index,bool writeChangesNow=true);
    
    /**
     * Remove a folder from the list of AppFolders
     * @param index position of the folder to remove
     * @param writeChangesNow sets if the change should be written to the 
     * config file immediately.
     */
    void removeAppFolder(int index,bool writeChangesNow=true);
    
    /**
     * Find the index of an AppFolder in the list of folders.
     * @param toFind will be searched for in the folder list
     * @return the index of toFind, or -1 if it isn't in the list.
     */
    int getFolderIndex(AppFolder toFind);

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
