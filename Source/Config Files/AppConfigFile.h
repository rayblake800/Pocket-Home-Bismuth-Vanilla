#pragma once
#include "ConfigFile.h"

/**
 * @file AppConfigFile.h
 * 
 * @brief Loads the pinned application links and folders displayed in the 
 *        AppMenuComponent.
 * 
 * @see AppMenuComponent.h
 */

class AppConfigFile : public ResourceManager
{
public:
    AppConfigFile();

    virtual ~AppConfigFile() { }

    //############ Application Shortcut Methods: ############
    class AppItem;
    /**
     * @return a list of AppItems to be pinned to the main column 
     * of the AppMenu
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
     *                          config file immediately.
     */
    void removeFavoriteApp(int index, bool writeChangesNow = true);

    /**
     * Find the index of an AppItem in favorites.
     * 
     * @param toFind  will be searched for in the favorites list
     * 
     * @return the index of toFind, or -1 if it isn't in the list.
     */
    int getFavoriteIndex(AppItem toFind);
    
    
    //############ Application Folder Methods: ############
    class AppFolder;
    /**
     * @return A list of folders to display in the AppMenu.
     */
    juce::Array<AppFolder> getFolders();

    /**
     * Add a new folder to the list of AppFolders in the config file.
     * 
     * @param newFolder        Defines the new folder.
     * 
     * @param index            Where to insert the new folder.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                          config file immediately.
     */
    void addAppFolder
    (AppFolder newFolder, int index, bool writeChangesNow = true);

    /**
     * Remove a folder from the list of AppFolders.
     * 
     * @param index            The position of the folder to remove.
     * 
     * @param writeChangesNow  Sets if the change should be written to the 
     *                          config file immediately.
     */
    void removeAppFolder(int index, bool writeChangesNow = true);

    /**
     * Find the index of an AppFolder in the list of folders.
     * 
     * @param toFind   This will be searched for in the folder list.
     * 
     * @return the index of toFind, or -1 if it isn't in the list.
     */
    int getFolderIndex(AppFolder toFind);

    /**
     *Represents an application pinned to the main menu
     */
    struct AppItem
    {

        AppItem() { };

        /**
         * Load an AppItem from json file data.
         * 
         * @param jsonObj
         */
        AppItem(juce::var jsonObj);

        /**
         * @return AppItem data stored as a DynamicObject* that can be written
         *          to json.
         */
        juce::DynamicObject* getDynamicObject();

        bool operator==(const AppItem& rhs)const;

        //Application display name
        juce::String name;
        //Application icon name or path
        juce::String icon;
        //Application launch command
        juce::String shell;
        //Sets if the application launches in a terminal window
        bool launchInTerminal = false;
    };

    /**
     * Represents an application folder
     * TODO:add sub-folders
     */
    struct AppFolder
    {

        AppFolder() { };

        /**
         * Load folder information from json data.
         * 
         * @param jsonObj
         */
        AppFolder(juce::var jsonObj);

        /**
         * @return folder data as a DynamicObject* ready to be written to a 
         *          json file.
         */
        juce::DynamicObject * getDynamicObject();

        //Folder display name
        juce::String name;
        //Application categories held in the folder.
        juce::StringArray categories;
        //The application icon path or file name
        juce::String icon;

        bool operator==(const AppFolder& rhs) const;
    };

private:
    /**
     * Holds the shared json file data resources.
     */
    class AppJson : public ConfigFile
    {
    public:
        AppJson();

        virtual ~AppJson() { }

        /**
         * @return a list of AppItems to be pinned to the main column 
         * of the AppMenu
         */
        juce::Array<AppItem> getFavorites();

        /**
         * Add a new app to the list of pinned favorite apps in the config file.
         * @see AppConfigFile::addFavoriteApp
         */
        void addFavoriteApp(AppItem newApp, int index, bool writeChangesNow);

        /**
         * Remove an app from the list of favorite applications
         * @see AppConfigFile::removeFavoriteApp
         */
        void removeFavoriteApp(int index, bool writeChangesNow);

        /**
         * Find the index of an AppItem in favorites.
         * @see AppConfigFile::getFavoriteIndex
         */
        int getFavoriteIndex(AppItem toFind);

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
         * Read in this object's data from a json config object
         * 
         * @param config         json data from ~/.pocket-home/filename.json.
         * 
         * @param defaultConfig  Default json config data from the filename.json
         *                        in assets. If this value is void and default data 
         *                        is needed, this method will open it as the 
         *                        appropriate default config file from assets.
         */
        void readDataFromJson
        (juce::var& config, juce::var& defaultConfig) override final;

        /**
         * Copy all config data to a json object
         * 
         * @param jsonObj
         */
        void copyDataToJson(juce::DynamicObject::Ptr jsonObj) override final;

        /**
         * @return the empty list, as AppConfigFile doesn't track any DataKey
         * variables, only its own custom data structures.
         */
        std::vector<DataKey> getDataKeys() const
        {
            return {};
        }

        //Stores application shortcuts
        juce::Array<AppItem> favoriteApps;

        //Stores application folders
        juce::Array<AppFolder> categoryFolders;

        //Application configuration file name
        static constexpr const char* filenameConst = "apps.json";
        //json key holding application shortcuts
        static constexpr const char* favoritesKey = "favorites";
        //json key holding application folders
        static constexpr const char* foldersKey = "folders";
    };

    //ResourceManager shared object and lock;
    static juce::ScopedPointer<ResourceManager::SharedResource> sharedResource;
    static juce::ReadWriteLock configLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppConfigFile)
};
