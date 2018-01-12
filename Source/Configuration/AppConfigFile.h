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
        int index;
        String folder;
        DynamicObject * getDynamicObject();
        bool operator==(const AppItem& rhs)const;
    };
    /**
     * @return a list of AppItems to be pinned to the main column 
     * of the AppMenu
     */
    Array<AppItem> getFavorites();

    /**
     * Save new favorites data into config.
     * @param newFavorites a new list of favorite apps to be pinned to the
     * main column of the AppMenu
     */
    void setFavorites(Array<AppItem> newFavorites);

    //######################### Folder/Category Data ###########################

    /**
     * Represents an application folder
     * TODO:add sub-folders
     */
    struct AppFolder {
        AppFolder();
        AppFolder(var jsonObj);
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
