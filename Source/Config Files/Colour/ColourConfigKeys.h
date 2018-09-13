#pragma once
#include "JuceHeader.h"

/**
 * @file ColourConfigKeys.h
 *
 * @brief  Defines all UI color keys used in the colours.json configuration
 *         file.
 */
namespace ColourConfigKeys
{
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
    
    /* Represents an invalid color key */
    static const juce::Identifier invalidKey("");

    /**
     * @brief   Gets all Juce ColourId values defined by the colour config file.
     * 
     * @return  All ColourIds defined in colours.json, either directly or
     *          through a UICategory. 
     */
     const juce::Array<int>& getColourIds();
    
     /**
      * @brief  Gets all UI category color keys.
      *
      * @return  Keys for all generic UI element category colors.
      */
     const juce::Array<juce::Identifier>& getCategoryKeys();

    /**
     * @brief   Gets all keys used by the ColourConfigFile.
     * 
     * @return  All UICategory keys and ColourID keys. 
     */
     const juce::Array<juce::Identifier>& getColourKeys();
    
    /**
     * @brief  Gets the UICategory assigned to a Juce ColourId value.
     * 
     * @param colourId  The ColourId value of some UI element.
     * 
     * @return          The corresponding UICategory type, or UICategory::none 
     *                  if no assigned UICategory was found. 
     */
     UICategory getUICategory(const int colourId);  
    
    //=================== Lookup values using keys: ============================
    /**
     * @brief  Finds the Juce ColourId value of a UI element from its key.
     * 
     * @param colourKey  The key for a UI element colour.
     * 
     * @return           The corresponding ColourId value, or -1 if colourKey 
     *                   doesn't map to a UI element.
     */
    int getColourId(const juce::Identifier& colourKey);
    
    /**
     * @brief  Finds the UICategory type represented by a specific key.
     * 
     * @param categoryKey  The key of a UICategory value.
     * 
     * @return             The UICategory value associated with the key, or 
     *                     UICategory::none if categoryKey is not a valid key.
     */
    UICategory getCategoryType(const juce::Identifier& categoryKey);

     //====================== Lookup keys by value: ============================
    /**
     * @brief  Gets the key used to store a specific Juce ColourId in the colour 
     *         config file.
     * 
     * @param colourId  The Juce ColourId value of a UI element.
     * 
     * @return          The key for that colourId, or invalidKey if colourId is
     *                  not tracked in colours.json. 
     */
    const juce::Identifier& getColourKey(const int colourId);
       
    /**
     * Finds the key string representing a UICategory value.
     * 
     * @param category  One of the default UI element types.
     * 
     * @return          The key used to store this category's value in the
     *                  JSON config file.
     */
    const juce::Identifier& getCategoryKey(const UICategory category);
}
