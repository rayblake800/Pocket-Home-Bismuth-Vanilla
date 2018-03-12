/**
 * @file ComponentConfigFile.h
 * 
 * ComponentConfigFile gets and sets all component settings defined in 
 * ~/.pocket-home/components.json.
 * 
 * This includes component color values, relative component size and position,
 * image asset filenames, and other miscellaneous data relevant to UI 
 * components.
 */
#pragma once
#include "ConfigFile.h"

class ComponentConfigFile : public ConfigFile {
public:
    ComponentConfigFile();
    virtual ~ComponentConfigFile();

    /**
     * Colour value string keys are stored here along with their corresponding
     * ColourId values
     */
    static const std::map<String, int> colourIds;

    /**
     * Find a Component ColourId value from its String key
     * 
     * @param colourKey the key for a color value in colourIds
     * @return the corresponding ColourId value, or -1 if colourKey wasn't
     * found.
     */
    int getColourId(String colourKey);
    
    /**
     * @return the keys to all Component color settings stored in
     * components.json
     */
    Array<String> getColourKeys() const;
    

    //#### Boolean value keys #######
    static const String showClockKey;
    static const String use24HrModeKey;

    //######################### UI Component Data ##############################
    //Defines all component types managed in the config file
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
    static const String pageUpKey;
    static const String pageDownKey;
    static const String smallTextKey;
    static const String mediumTextKey;
    static const String largeTextKey;

    /**
     * Return the most appropriate font size for drawing text
     * @param textBounds the area in which the text will be drawn
     * @param text the actual text being drawn
     * @return whichever font height (small, medium, or large) defined in 
     * components.json would best fit this text within its bounds.  If even the
     * small font size is too big to fit in textBounds, instead return whatever
     * font height will fit.
     */
    int getFontHeight(Rectangle <int> textBounds,String text);
    
    /**
     * Represents a configurable UI element
     */
    class ComponentSettings {
    public:
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
    
protected:
        
    /**
     * @return the list of all component keys.
     */
    Array<String> getComponentKeys();
    
    /**
     * @return the list of key Strings for each basic data value tracked in 
     * components.json
     */
    virtual std::vector<DataKey> getDataKeys() const override;
    
private:

    /**
     * Read in this object's data from a json config object
     * 
     * @param config json data from ~/.pocket-home/filename.json
     * 
     * @param defaultConfig default json config data from the filename.json
     * in assets. If this value is void and default data is needed, this 
     * method will open it as the appropriate default config file from assets
     */
    void readDataFromJson(var& config, var& defaultConfig) override final;

    /**
     * Copy all config data to a json object
     */
    void copyDataToJson(DynamicObject::Ptr jsonObj) override final;

    static CriticalSection componentLock;
    static std::map<String, ComponentSettings> components;
    static constexpr const char* filenameConst = "components.json";
};