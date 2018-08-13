#pragma once
#include "ConfigFile.h"
#include "ResourceManager.h"

/**
 * @file ComponentConfigFile.h
 * 
 * @brief gets and sets all component settings defined in 
 *        ~/.pocket-home/components.json.
 * 
 * This includes relative component size and position, image asset filenames, 
 * and other miscellaneous data relevant to UI components.
 */

class ComponentConfigFile : public ResourceManager
{
public:
    ComponentConfigFile();

    virtual ~ComponentConfigFile() { }

    class ComponentSettings;

    /**
     * Gets configured component settings from shared .json file data.
     * 
     * @param componentKey  A configurable UI component's key string.
     * 
     * @return  The properties defined for that component.
     */
    ComponentSettings getComponentSettings(juce::String componentKey);

    /**
     * Add a listener to track component setting changes.
     * @see ConfigFile.h
     */
    void addListener(ConfigFile::Listener* listener, juce::StringArray trackedKeys);
    //######################### Text Size Keys #################################
    static const juce::String smallTextKey;
    static const juce::String mediumTextKey;
    static const juce::String largeTextKey;

    //######################### UI Component Data ##############################
    //Defines all component types managed in the config file
    static const juce::String scrollingAppMenuKey;
    static const juce::String pagedAppMenuKey;
    static const juce::String menuFrameKey;
    static const juce::String batteryIconKey;
    static const juce::String batteryPercentKey;
    static const juce::String clockLabelKey;
    static const juce::String wifiIconKey;
    static const juce::String powerButtonKey;
    static const juce::String settingsButtonKey;
    static const juce::String popupMenuKey;
    static const juce::String pageLeftKey;
    static const juce::String pageRightKey;
    static const juce::String pageUpKey;
    static const juce::String pageDownKey;
    static const juce::String settingsListBtnKey;
    static const juce::String spinnerKey;

    /**
     * Represents the three main text size options.  The actual size of
     * each of these options is set in components.json, either as a fraction
     * of the window height (if textSize <= 1) or as a fixed height in pixels
     * (if textSize > 1).
     */
    enum TextSize
    {
        smallText,
        mediumText,
        largeText
    };

    /**
     * Return the most appropriate font size for drawing text.
     * 
     * @param textBounds  The area in which the text will be drawn.
     * 
     * @param text        The actual text being drawn.
     * 
     * @return   Whichever font height (small, medium, or large) defined in 
     *           components.json would best fit this text within its bounds. 
     *           If even the small font size is too big to fit in textBounds, 
     *           instead return whatever font height is small enough to fit.
     */
    int getFontHeight(juce::Rectangle <int> textBounds, juce::String text);

    /**
     * Gets the height in pixels of one of the three configured text sizes.
     *
     * @param sizeType  A text size type defined in the Component config file.
     *
     * @return  The height in pixels of that text size type.
     */
    int getFontHeight(TextSize sizeType);

    /**
     * Represents a configurable UI element
     */
    class ComponentSettings
    {
    public:
        ComponentSettings();

        /**
         * Initializes from JSON data.
         * 
         * @param jsonObj  An dynamic object containing JSON data.
         */
        ComponentSettings(juce::var jsonObj);

        /**
         * Packages the object into a DynamicObject that can be written to a
         * JSON file.
         * 
         * @return  the converted component settings.
         */
        juce::DynamicObject * getDynamicObject();

        /**
         * @return the bounds of the component relative to the window,
         * measured in pixels.
         */
        juce::Rectangle<int> getBounds();

        /**
         * @return the list of configurable colors.
         */
        juce::Array<juce::Colour> getColours();

        /**
         * @return the list of component asset files.
         */
        juce::StringArray getAssetFiles();

        /**
         * Use these settings to position and size a component.
         * 
         * @param component  Any component object.
         */
        void applyBounds(juce::Component * component);
        
        /**
         * Gets this component's x-coordinate as a fraction of the window's
         * width.
         * 
         * @return the x coordinate fraction, or -1 if the x-coordinate fraction
         *         is not defined.
         */
        float getXFraction();
         
        /**
         * Gets this component's y-coordinate as a fraction of the window's
         * height.
         * 
         * @return the y coordinate fraction, or -1 if the y-coordinate fraction
         *         is not defined.
         */
        float getYFraction();
              
        /**
         * Gets this component's width as a fraction of the window's width.
         * 
         * @return the width fraction, or -1 if the width fraction is not 
         *         defined.
         */
        float getWidthFraction();      
              
        /**
         * Gets this component's height as a fraction of the window's height.
         * 
         * @return the height fraction, or -1 if the height fraction is not 
         *         defined.
         */
        float getHeightFraction();


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
        juce::Array<juce::Colour> colours;

        //Component asset files.  This is mostly intended to define image 
        //assets, but there's no reason it couldn't be used to define other
        //assets.
        juce::StringArray assetFiles;
    };


protected:
    /**
     * Gets the list of all configurable component keys.
     * 
     * @return  The list of component keys.
     */
    static const juce::StringArray& getComponentKeys();

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
        ComponentSettings getComponentSettings(juce::String componentKey);

    private:
        /**
         * Copy all ComponentSettings data back to the JSON file.
         */
        virtual void writeDataToJSON() final override;

        /**
         * Gets the string key and data type for each basic value stored in
         * components.json.
         * 
         * @return  Key strings and data types for each bool, int, String, or
         *          double stored in components.json.
         */
        virtual const std::vector<DataKey>& getDataKeys() const final override;

        //Stores all component settings loaded from the component config file
        std::map<juce::String, ComponentSettings> components;
    };

    //Defines the component config file's name
    static constexpr const char * filenameConst = "components.json";

    //ResourceManager shared object and lock;
    static juce::ScopedPointer<ResourceManager::SharedResource> sharedResource;
    static juce::ReadWriteLock configLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComponentConfigFile)
};
