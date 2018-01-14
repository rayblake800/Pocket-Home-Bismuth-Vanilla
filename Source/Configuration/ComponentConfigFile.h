/*
  ==============================================================================

    ComponentConfigFile.h
    Created: 5 Jan 2018 7:08:00pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "ConfigFile.h"

class ComponentConfigFile : public ConfigFile {
public:
    ComponentConfigFile();
    virtual ~ComponentConfigFile();


    Array<String> getStringKeys() const;

    //######################### Boolean Data ###################################
    //boolean value keys
    static const String showClockKey;
    static const String use24HrModeKey;
    Array<String> getBoolKeys() const;

    //######################### UI Component Data ##############################
    //Defines all component types managed in the config file
    static const String appMenuButtonKey;
    static const String appMenuKey;
    static const String menuFrameKey;
    static const String batteryIconKey;
    static const String batteryPercentKey;
    static const String clockIconKey;
    static const String wifiIconKey;
    static const String powerButtonKey;
    static const String settingsButtonKey;
    static const String popupMenuKey;
    

    /**
     * @return the list of all component keys.
     */
    Array<String> getComponentKeys();

    /**
     * Represents a configurable UI element
     */
    class ComponentSettings {
    public:

        /**
         * Initializes the object with all empty/zero values
         */
        ComponentSettings();
        
        //copy constructor
        ComponentSettings(const ComponentSettings& copy);
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
     * @param componentKey a configurable UI component's key
     * @return properties defined for that component
     */
    ComponentSettings getComponentSettings(String componentKey);

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

    std::map<String, ComponentSettings> components;

    static constexpr const char* filenameConst = "components.json";
};