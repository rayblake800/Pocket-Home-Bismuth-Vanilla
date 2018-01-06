/*
  ==============================================================================

    MainConfigFile.h
    Created: 5 Jan 2018 2:35:02pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "ConfigFile.h"

class MainConfigFile : public ConfigFile {
public:
    MainConfigFile();
    virtual ~MainConfigFile();

    //######################### String Data ###################################
    //string value keys
    static const String backgroundKey;
    static const String shutdownCommandKey;
    static const String restartCommandKey;
    static const String termLaunchCommandKey;
    Array<String> getStringKeys() const;
    //######################### Boolean Data ###################################
    //boolean value keys
    static const String showCursorKey;
    static const String showClockKey;
    static const String use24HrModeKey;
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
        DynamicObject * getDynamicObject();
        bool operator==(const AppItem& rhs)const;
    };
    /**
     * @return a list of AppItems to be pinned to the main column 
     * of the AppMenu
     */
    std::vector<AppItem> getFavorites();

    /**
     * Save new favorites data into config.
     * @param newFavorites a new list of favorite apps to be pinned to the
     * main column of the AppMenu
     */
    void setFavorites(std::vector<AppItem> newFavorites);

    //######################### Folder/Category Data ###########################

    /**
     * Represents an application folder
     * TODO:add sub-folders
     */
    struct AppFolder {
        AppFolder();
        AppFolder(var jsonObj);
        String name;
        std::vector<String> categories;
        String icon;
        std::vector<AppItem> pinnedApps;
        DynamicObject * getDynamicObject();
        bool operator==(const AppFolder& rhs) const;
    };
    /**
     * @return A list of folders to display in the AppMenu 
     */
    std::vector<AppFolder> getFolders();

    //######################### UI Component Data ##############################
    //Defines all component types managed in the config file

    /**
     * Represents a configurable UI element
     */
    class ComponentSettings {
    public:

        /**
         * Initializes the object with all empty/zero values
         */
        ComponentSettings();
        /**
         * Initializes from json data
         * @param jsonObj an object var containing json data
         */
        ComponentSettings(var jsonObj);
        DynamicObject * getDynamicObject();

        /**
         * @return the bounds of the component relative to the window,
         * measured in pixels
         */
        Rectangle<int> getBounds();

        /**
         * @return the list of configurable colors.
         */
        std::vector<Colour> getColours();
        /**
         * @return the list of component asset files.
         */
        std::vector<String> getAssetFiles();
        /**
         * Use these settings to position and size a component
         * @param component an active component
         */
        void applyBounds(Component * component);
        /**
         * Use these settings to re-size a component without adjusting position
         * @param component an active component
         */
        void applySize(Component * component);
        bool operator==(const ComponentSettings& rhs) const;
    private:
        //Position and size data is stored in terms of total screen size,
        //e.g. height=0.5 takes up half the screen height,
        //x=0.2 has a left border 1/5 of the way from the left of the screen, etc.
        float x;
        float y;
        float width;
        float height;
        std::vector<Colour> colours;
        std::vector<String> assetFiles;
    };
    /**
     * @param componentType a configurable UI component
     * @return properties defined for that component
     */
    ComponentSettings getComponentSettings(ComponentType componentType);

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

    std::vector<AppItem> favoriteApps;
    std::vector<AppFolder> categoryFolders;
    std::map<ComponentType, ComponentSettings> components;
    static constexpr const char* filenameConst = "config.json";
    static constexpr const char* FAVORITES_KEY = "favorites";
    static constexpr const char* FOLDERS_KEY = "folders";

    static std::map<String, ComponentType> setComponentKeys() {
        std::map<String, ComponentType> keymap;
        keymap["app menu buttons"] = APP_MENU_BUTTON;
        keymap["app menu"] = APP_MENU;
        keymap["menu frame"] = MENU_FRAME;
        keymap["battery"] = BATTERY;
        keymap["battery percent text"] = BATTERY_PERCENT;
        keymap["time"] = CLOCK;
        keymap["wifi"] = WIFI;
        keymap["power button"] = POWER;
        keymap["settings button"] = SETTINGS;
        return keymap;
    }
    static const std::map<String, ComponentType> componentKeys;
};