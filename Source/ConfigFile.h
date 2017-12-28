/*
  ==============================================================================

    ConfigFile.h
    Created: 26 Dec 2017 10:31:17am
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include <map>
#include "../JuceLibraryCode/JuceHeader.h"

class ConfigFile {
public:
    virtual ~ConfigFile();
    /**
     * @return the single ConfigFile access objects
     */
    static ConfigFile* getInstance();

    //######################### String Data ####################################

    /**
     * Maps all string values stored in the config file.
     */
    enum ConfigString {
        BACKGROUND, //Background image or color
        SHUTDOWN_COMMAND,
        RESTART_COMMAND,
        TIME_FORMAT//ampm or 24h
    };

    /**
     * Gets a string value
     * @param configString the string you need
     * @return the string value from the config file
     */
    String getConfigString(ConfigString configString);

    /**
     *Sets a string value, writing it to the config file if the value has
     * changed
     * @param configString the string variable to access
     * @param newValue the new value for the string
     */
    void setConfigString(ConfigString configString, String newValue);

    //######################### Boolean Data ####################################

    /**
     *Maps all boolean values stored in the config file
     */
    enum ConfigBool {
        SHOW_CURSOR,
        SHOW_CLOCK
    };
    /**
     * Gets a boolean value
     * @param configBool the boolean value you need
     * @return the bool value from the config file
     */
    bool getConfigBool(ConfigBool configBool);

    /**
     *Sets a boolean value, writing it to the config file if the value has
     * changed
     * @param configBool the boolean variable to access
     * @param newValue the new value for the bool
     */
    void setConfigBool(ConfigBool configBool, bool newValue);

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
    };
    /**
     * @return A list of folders to display in the AppMenu 
     */
    std::vector<AppFolder> getFolders();

    //######################### UI Component Data ##############################
    //Defines all component types managed in the config file

    enum ComponentType {
        APP_MENU_BUTTON,
        APP_MENU,
        MENU_FRAME,
        BATTERY,
        BATTERY_PERCENT,
        CLOCK,
        WIFI,
        POWER,
        SETTINGS
    };

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
     * Reads in all data from the config value, copying any missing values
     * from the default config.json in assets.
     * If a marshmallow pocket-home config.json file is found, it will be backed
     * up to .pocket-home/marshmallowConfig.json
     */
    ConfigFile();

    /**
     * Re-writes all data back to the config file.
     */
    void writeChanges();

    std::map<String, String> stringValues;
    std::map<String, bool> boolValues;
    std::vector<AppItem> favoriteApps;
    std::vector<AppFolder> categoryFolders;
    std::map<ComponentType, ComponentSettings> components;
    CriticalSection lock;

    //constants and default values: 
    static constexpr const char* CONFIG_PATH = "/.pocket-home/config.json";
    static constexpr const char* FAVORITES_KEY = "favorites";
    static constexpr const char* FOLDERS_KEY = "folders";

    //component data keys

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

    //string data keys

    static std::map<ConfigString, String> setStringKeys() {
        std::map<ConfigString, String> keymap;
        keymap[BACKGROUND] = "background";
        keymap[SHUTDOWN_COMMAND] = "shutdown command";
        keymap[RESTART_COMMAND] = "restart command";
        keymap[TIME_FORMAT] = "timeformat";
        return keymap;
    }
    static const std::map<ConfigString, String> stringKeys;

    //boolean data keys

    static std::map<ConfigBool, String> setBoolKeys() {
        std::map<ConfigBool, String> keymap;
        keymap[SHOW_CURSOR] = "cursor";
        keymap[SHOW_CLOCK] = "showclock";
        return keymap;
    }
    static const std::map<ConfigBool, String> boolKeys;




};