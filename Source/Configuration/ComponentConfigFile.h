/**
 * @file ConfigFile.h
 * 
 * TODO: finish documentation
 */
#pragma once
#include "ConfigFile.h"

class ComponentConfigFile : public ConfigFile {
public:
    ComponentConfigFile();
    virtual ~ComponentConfigFile();

    //######################### String Data ####################################
    /**
     * Colour value string keys are stored here along with their corresponding
     * ColourId values
     */
    static const std::map<String,int> colourIds;
    
    /**
     * Find a Component ColourId value from its config key String
     * @param colourKey the key for a color value in colourIds
     * @return the corresponding ColourId value, or -1 if colourKey wasn't
     * found.
     */
    int getColourId(String colourKey);
    virtual Array<String> getStringKeys() const override;

    //######################### Boolean Data ###################################
    //boolean value keys
    static const String showClockKey;
    static const String use24HrModeKey;
    virtual Array<String> getBoolKeys() const override;

    //######################### UI Component Data ##############################
    //Defines all component types managed in the config file
    static const String appMenuButtonKey;
    static const String scrollingAppMenuKey;
    static const String pagedAppMenuKey;
    static const String menuFrameKey;
    static const String batteryIconKey;
    static const String batteryPercentKey;
    static const String clockLabelKey;
    static const String wifiIconKey;
    static const String powerButtonKey;
    static const String settingsButtonKey;
    static const String popupMenuKey;
    static const String pageLeftKey;
    static const String pageRightKey;
    

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
        Array<Colour> getColours();
        /**
         * @return the list of component asset files.
         */
        Array<String> getAssetFiles();
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
        Array<Colour> colours;
        Array<String> assetFiles;
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