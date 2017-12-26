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

    /**
     *Represents an application pinned to the main menu
     */
    struct AppItem {
        String name;
        String icon;
        String shell;
    };

    /**
     * Represents an application folder
     * TODO:add sub-folders
     */
    struct AppFolder{
        String displayName;
        std::vector<String> categoryNames;
        String icon;
    };

    /**
     * Represents a configurable UI element
     * Position and size data is represented in terms of total screen size,
     * e.g. height=0.5 takes up half the screen height,
     * x=0.2 has a left border 1/5 of the way from the left of the screen, etc.
     */
    struct ComponentSettings {
        float x;
        float y;
        float width;
        float height;
        std::vector<Colour> colours;
        std::vector<String> assetFiles;
    };
    enum ComponentType {
        APP_MENU_BUTTON
    };
    virtual ~ConfigFile();
    static ConfigFile* getInstance();
    void writeChanges();
    std::vector<AppItem> getFavorites();
    std::vector<AppFolder> getFolders();
    ComponentSettings * getComponentSettings(ComponentType componentType);

    

    
private:
    ConfigFile();
    //Update a pocket-home marshmallow config file, saving the original as
    //marshmallowConfig.json
    void updateLegacyConfig(var jsonConfig);
    //Completely recreate the config file using default values.
    void loadDefaultConfig();
    //load in config data from json,replacing missing data with default values

    //Convert the different configFile objects to DynamicObject
    //so they can be converted back to JSON and written to the config file
    ScopedPointer<DynamicObject> getDynamicObject(AppItem appItem);
    ScopedPointer<DynamicObject> getDynamicObject(AppFolder appFolder);
    ScopedPointer<DynamicObject> getDynamicObject(ComponentSettings componentSettings);

    std::vector<AppItem> favorites;
    std::vector<AppFolder> categories;
    std::map<ComponentType, ComponentSettings> components;
    CriticalSection lock;
    
    //constants and default values: 
    static constexpr const char* CONFIG_PATH = "/.pocket-home/config.json";
    static constexpr const char* FAVORITES_KEY = "favorites";
    
    static std::map<ComponentType,String> setComponentKeys(){
        std::map<ComponentType,String> keymap;
        keymap[APP_MENU_BUTTON]="appMenuButtons";
        return keymap;
    }
    static const std::map<ComponentType,String> componentKeys;
    
};