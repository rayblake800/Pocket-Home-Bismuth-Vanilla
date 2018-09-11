#pragma once
#include "ConfigFile.h"
#include "ColourJSON.h"
#include "ColourConfigKeys.h"

/**
 * @file ColourConfigFile.h
 * 
 * @brief Gets and sets all UI color values defined in 
 *        ~/.pocket-home/colours.json.
 */

class ColourConfigFile : public ConfigFile<ColourJSON>
{
public:
    ColourConfigFile() { }

    virtual ~ColourConfigFile() { }
    
    typedef ColourJSON::Listener Listener;

    typedef ColourConfigKeys::UICategory UICategory;

    /**
     * @brief  Looks up the Colour value saved for a specific Juce ColourId.
     *  
     * @param colourId  The id value representing the colour of a specific UI
     *                  element.
     * 
     * @return          The Colour value saved to the colours.json file for the 
     *                  given colourId.  If no Colour is set for that specific 
     *                  id, the Colour for the UICategory associated with that
     *                  id will be returned. If colourId is not a valid id, 
     *                  Colour() will be returned.
     */
    juce::Colour getColour(int colourId) const;
    
    /**
     * @brief  Gets the Colour value assigned as the default for all UI items in
     *         a specific category.
     * 
     * @param category  One of the categories of UI elements.
     * 
     * @return          The default color value used by all items in that 
     *                  category that don't have a color value assigned to them 
     *                  specifically.
     */
    juce::Colour getColour(UICategory category) const;
    
    /**
     * @brief  Gets the colour value associated with a particular key string.
     * 
     * @param colourKey  The key of a Colour saved in the ColourConfigFile
     * 
     * @return           The saved Colour, or Colour() if the colour key isn't
     *                   found.
     */
    juce::Colour getColour(juce::String colourKey) const;
       
    /**
     * @brief  Sets the saved colour value for a single UI element.
     * 
     * @param colourId   A Juce ColourId value tracked by the colour config
     *                   file.
     * 
     * @param newColour  The colour value to assign to the ColourId. 
     */
    void setColour(int colourId, juce::Colour newColour);

    /**
     * @brief  Sets the saved colour value for a category of UI elements.
     * 
     * @param category   The category to assign the new colour value.
     * 
     * @param newColour  The colour value to assign to the category.
     */
    void setColour(UICategory category, juce::Colour newColour);
    
    /**
     * @brief  Sets the saved colour value for a specific key string.
     * 
     * @param colourKey  A colour key string tracked by the ColourConfigFile.
     * 
     * @param newColour  The new colour value to save to the file.
     */
    void setColour(juce::String colourKey, juce::Colour newColour);

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColourConfigFile)
};
