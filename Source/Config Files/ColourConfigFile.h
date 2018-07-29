#pragma once
#include "ConfigFile.h"
#include "ResourceManager.h"

/**
 * @file ColourConfigFile.h
 * 
 * @brief gets and sets all UI color values defined in 
 *        ~/.pocket-home/colours.json.
 */

class ColourConfigFile : public ResourceManager
{
public:
    ColourConfigFile();

    virtual ~ColourConfigFile() { }
    
    /**
     * All Juce ColourId values used by this program are assigned to one of 
     * these categories.
     */
    enum UICategory
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

    /**
     * Look up the Colour value saved for a specific Juce ColourId.
     *  
     * @param colourId  The Id value representing the colour of a specific UI
     *                  element.
     * 
     * @return  The Colour value saved to the colours.json file for the given
     *          colourId.  If no Colour is set for that specific Id, the Colour
     *          for the UICategory associated with that Id will be returned. If
     *          colourId is not a valid Id, Colour() will be returned.
     */
    juce::Colour getColour(int colourId) const;
    
    /**
     * Gets the Colour value assigned as the default for all UI items in a
     * specific category.
     * 
     * @param category  One of the categories of UI elements.
     * 
     * @return  The default color value used by all items in that category that
     *          don't have a color value assigned to them specifically.
     */
    juce::Colour getColour(UICategory category) const;
    
    /**
     * Gets the colour value associated with a particular key string.
     * 
     * @param colourKey  The key of a Colour saved in the ColourConfigFile
     * 
     * @return  The saved Colour, or Colour() if the colour key isn't found.
     */
    juce::Colour getColour(juce::String colourKey) const;
       
    /**
     * Sets the saved colour value for a single UI element.
     * 
     * @param colourId   A Juce ColourId value tracked by the colour config
     *                   file.
     * 
     * @param newColour  The colour value to assign to the ColourId. 
     */
    void setColour(int colourId, juce::Colour newColour);

    /**
     * Sets the saved colour value for a category of UI elements.
     * 
     * @param category   The category to assign the new colour value.
     * 
     * @param newColour  The colour value to assign to the category.
     */
    void setColour(UICategory category, juce::Colour newColour);
    
    /**
     * Sets the saved colour value for a specific key string.
     * 
     * @param colourKey  A colour key string tracked by the ColourConfigFile.
     * 
     * @param newColour  The new colour value to save to the file.
     */
    void setColour(juce::String colourKey, juce::Colour newColour);
   
    /**
     * Gets all Juce ColourId values defined by the colour config file.
     * 
     * @return  All ColourIds defined in colours.json, either directly or
     *          through a UICategory. 
     */
    static juce::Array<int> getColourIds();
    
    /**
     * Gets all key strings used by the ColourConfigFile.
     * 
     * @return  All UICategory keys and ColourID keys. 
     */
    static juce::StringArray getColourKeys();
    
    class Listener : public ConfigFile::Listener
    {
    friend class ColourConfigFile;
    private:
        /**
         * Notify the Listener when a colour value it tracks is updated.  When 
         * generic UICategory colors are updated, this method will run for each 
         * ColourId in that category that doesn't have an explicit colour 
         * definition.
         * 
         * @param colourId    The Juce ColourId value being updated.
         * 
         * @param updatedKey  The key string of the updated config value.
         *  
         * @param newColour   The updated Colour value.
         */
        virtual void colourValueChanged
        (int colourId, juce::String updatedKey, juce::Colour newColour) = 0;
        
        /**
         * Notify the Listener whenever a non-color key value it tracks is
         * updated.
         * 
         * @param propertyKey  The key string of some property tracked by the
         *                     Listener that is not from the ColorConfigFile.
         */
        virtual void nonColorValueChanged(juce::String propertyKey) { }
        
        /**
         * Calls colourValueChanged for each Juce ColourId associated with
         * the updated value's key.
         * 
         * @param propertyKey  The key of an updated ColourId or UICategory.
         */
        virtual void configValueChanged(juce::String propertyKey) 
        final override;
        
        juce::Array<int> trackedColourIds;
    };

    /**
     * Adds a listener to track color setting changes.
     * 
     * @param listener    The object listening for color changes.
     * 
     * @param trackedIds  All Juce ColourId values the listener will track.
     */
    void addListener(Listener* listener, juce::Array<int> trackedIds);

private:
    /**
     * Gets a Colour string saved to the colour config file.
     * 
     * @param colourKey  The key to a colour value saved in the colours.json
     *                   file.
     * 
     * @return  The colour hex string mapped to that key, or the empty string
     *          if the key isn't found.
     */
    juce::String getColourString(juce::String colourKey) const;
    
    /**
     * Gets the UICategory assigned to a Juce ColourId value.
     * 
     * @param colourId  The ColourId value of some UI element.
     * 
     * @return  The corresponding UICategory type, or UICategory::none if
     *          no assigned UICategory was found. 
     */
    static UICategory getUICategory(int colourId);  
    
    
       
    //============ Lookup key values using key strings: ========================
    /**
     * Finds the Juce ColourId value of a UI element from its key string.
     * 
     * @param colourKey  The key string for a UI element colour.
     * 
     * @return  The corresponding ColourId value, or -1 if colourKey doesn't
     *          map to a UI element.
     */
    static int getColourId(juce::String colourKey);
    
    /**
     * Finds the UICategory type represented by a specific key string.
     * 
     * @param categoryKey  The string key of a UICategory value.
     * 
     * @return   The UICategory value associated with the key string, or
     *           UICategory::none if categoryKey is not a valid key string.
     */
    static UICategory getCategoryType(juce::String categoryKey);
     
    //================ Lookup key strings by value: ============================
    /**
     * Gets the key string used to store a specific Juce ColourId in the
     * colour config file.
     * 
     * @param colourId  The Juce ColourId value of a UI element.
     * 
     * @return  The key string for that colourId, or String() if colourId is not
     *          tracked in colours.json. 
     */
    static juce::String getColourKey(int colourId);
       
    /**
     * Finds the key string representing a UICategory value.
     * 
     * @param category  One of the default UI element types.
     * 
     * @return  The string key used to store this category's value in the
     *          json config file.
     */
    static juce::String getCategoryKey(UICategory category);
      

    class ConfigJson : public ConfigFile
    {
    public:
        ConfigJson();

        virtual ~ConfigJson() { }

    private:
        /**
         * @return the list of key Strings for each basic data value tracked in 
         * colours.json
         */
        virtual std::vector<DataKey> getDataKeys() const override;
    };
    
    /**
     * Keys for colour values assigned to UI element colour categories.  All
     * Juce ColourId values are grouped under one of these categories.  When
     * looking up colour values, if no value is explicitly assigned to a given
     * ColourId, the value assigned to the associated UICategory will be used.
     */
    static const juce::StringArray uiCategoryKeys;
    
    /**
     * For each Juce ColourId value tracked in colours.json, colourIds maps that
     * colour to its string key.
     */
    static const std::map<int, juce::String> colourIdKeys;
    
    /**
     * For each colour key string tracked in colours.json, colourIds maps that
     * key to its Juce ColourId value.
     */
    static const std::map<juce::String, int> colourIds;
    
    /**
     * Maps each Juce ColourId value to the UI element category used to select
     * its default value.
     */
    static const std::map<int, UICategory> idCategories;
    
    //Defines the component config file's name
    static constexpr const char * filenameConst = "colours.json";

    //ResourceManager shared object and lock;
    static juce::ScopedPointer<ResourceManager::SharedResource> sharedResource;
    static juce::ReadWriteLock configLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColourConfigFile)
};
