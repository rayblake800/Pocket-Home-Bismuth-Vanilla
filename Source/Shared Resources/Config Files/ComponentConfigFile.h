#pragma once
#include "ConfigFile.h"
#include "ResourceManager.h"

/**
 * @file ComponentConfigFile.h
 * 
 * @brief gets and sets all component settings defined in 
 *        ~/.pocket-home/components.json.
 * 
 * This includes component color values, relative component size and position,
 * image asset filenames, and other miscellaneous data relevant to UI 
 * components.
 */

class ComponentConfigFile : public ResourceManager
{
public:
    ComponentConfigFile();

    virtual ~ComponentConfigFile() { }

    class ComponentSettings;

    static const StringArray defaultColourKeys;
    /**
     * Colour value string keys are stored here along with their corresponding
     * ColourId values
     */
    static const std::map<String, int> colourIds;

    enum DefaultColour
    {
        windowBackground = 0,
	widgetBackground,
	widgetOff,
	widgetOn,
	menuBackground,
	outline,
	focusedOutline,
	textField,
	text,
	highlightedTextField,
	highlightedText,
	none
    };

    DefaultColour getColourType(String colorKey);

    String getColourKey(DefaultColour colour);

    Colour getColour(DefaultColour colourType);

    Array<int> getColourIds(DefaultColour colourType);

    void setColour(DefaultColour colourType, Colour newColour);

    void setColour(String key, Colour newColour);

    /**
     * @return the keys to all Component color settings stored in
     * components.json
     */
    StringArray getColourKeys() const;

    /**
     * Find a Component ColourId value from its String key
     * 
     * @param colourKey the key for a color value in colourIds
     * @return the corresponding ColourId value, or -1 if colourKey wasn't
     * found.
     */
    int getColourId(String colourKey);

    /**
     * Get a color value from its String key.
     *  
     * @param colourKey the key for a color value in colourIds
     * @return the corresponding Colour value, or Colour() if colourKey wasn't
     * found.
     */
    Colour getColour(String colourKey);

    /**
     * Gets configured component settings from shared .json file data.
     * 
     * @param componentKey  A configurable UI component's key string.
     * 
     * @return              the properties defined for that component.
     */
    ComponentSettings getComponentSettings(String componentKey);

    /**
     * Add a listener to track component setting changes.
     * @see ConfigFile.h
     */
    void addListener(ConfigFile::Listener* listener, StringArray trackedKeys);

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
    static const String spinnerKey;
    static const String smallTextKey;
    static const String mediumTextKey;
    static const String largeTextKey;

    /**
     * Return the most appropriate font size for drawing text.
     * 
     * @param textBounds  The area in which the text will be drawn.
     * 
     * @param text        The actual text being drawn.
     * 
     * @return            Whichever font height (small, medium, or large) 
     *                     defined in components.json would best fit this text 
     *                     within its bounds. If even the small font size is 
     *                     too big to fit in textBounds, instead return whatever 
     *                     font height is small enough to fit.
     */
    int getFontHeight(Rectangle <int> textBounds, String text);

    /**
     * Represents a configurable UI element
     */
    class ComponentSettings
    {
    public:
        ComponentSettings();

        /**
         * Initializes from json data.
         * 
         * @param jsonObj  An object var containing json data.
         */
        ComponentSettings(var jsonObj);

        /**
         * Packages the object into a DynamicObject that can be written to a
         * json file.
         * 
         * @return  the converted component settings.
         */
        DynamicObject * getDynamicObject();

        /**
         * @return the bounds of the component relative to the window,
         * measured in pixels.
         */
        Rectangle<int> getBounds();

        /**
         * @return the list of configurable colors.
         */
        Array<Colour> getColours();

        /**
         * @return the list of component asset files.
         */
        StringArray getAssetFiles();

        /**
         * Use these settings to position and size a component.
         * 
         * @param component  Any component object.
         */
        void applyBounds(Component * component);


        bool operator==(const ComponentSettings& rhs) const;
    private:
        /*
         * Position and size data is stored in terms of total screen size,
         * e.g. height=0.5 takes up half the screen height,  x=0.2 has a left 
         * border 1/5 of the way from the left of the screen, etc.
         */
        float x;
        float y;
        float width;
        float height;

        //Custom colors for component image assets.
        Array<Colour> colours;

        //Component asset files.  This is mostly intended to define image 
        //assets, but there's no reason it couldn't be used to define other
        //assets.
        StringArray assetFiles;
    };


protected:

    /**
     * @return the list of all component keys.
     */
    static StringArray getComponentKeys();


private:

    class ConfigJson : public ConfigFile
    {
    public:
        ConfigJson();

        virtual ~ConfigJson() { }

        /**
         * Gets the configured settings for a particular component.
         * 
         * @param componentKey  A configurable UI component's key string.
         * 
         * @return              the properties defined for that component.
         */
        ComponentSettings getComponentSettings(String componentKey);

    private:
        /**
         * Read in this object's data from a json config object.
         * 
         * @param config         json data from ~/.pocket-home/filename.json.
         * 
         * @param defaultConfig  Default json config data from the 
         *                        filename.json in assets. If this value is void
         *                        and default data is needed, this  method will 
         *                        open it as the appropriate default config file
         *                        from assets.
         */
        void readDataFromJson(var& config, var& defaultConfig) override final;

        /**
         * Copy all config data to a json object
         * 
         * @param jsonObj
         */
        void copyDataToJson(DynamicObject::Ptr jsonObj) override final;

        //Stores all component settings loaded from the component config file
        std::map<String, ComponentSettings> components;

        /**
         * @return the list of key Strings for each basic data value tracked in 
         * components.json
         */
        virtual std::vector<DataKey> getDataKeys() const override;
    };

    //Defines the component config file's name
    static constexpr const char * filenameConst = "components.json";

    //ResourceManager shared object and lock;
    static ScopedPointer<ResourceManager::SharedResource> sharedResource;
    static CriticalSection configLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComponentConfigFile)
};
