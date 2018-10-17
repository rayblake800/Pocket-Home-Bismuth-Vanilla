#pragma once
#include "JuceHeader.h"
/**
 * @file   ComponentSettings.h
 *
 * @brief  Defines position, size, colors, and/or image resources of a UI
 *         component.
 */
class ComponentSettings
{
public:

    ComponentSettings();

    /**
     * @brief  Initializes the settings from JSON data.
     * 
     * @param jsonObj  A dynamic object containing JSON data.
     */
    ComponentSettings(juce::DynamicObject* jsonObj);

    /**
     * @brief   Packages the component settings into a juce::DynamicObject.
     * 
     * @return  The converted component settings.
     */
    juce::DynamicObject * getDynamicObject() const;

    /**
     * @brief   Gets the component bounds defined by this object.
     *
     * @return  The bounds of the component relative to the window,
     *          measured in pixels.
     */
    juce::Rectangle<int> getBounds() const;

    /**
     * @brief   Gets the list of configurable color values.
     *
     * @return  The list of component color values.
     */
    const juce::Array<juce::Colour>& getColours() const;

    /**
     * @brief   Gets the list of configurable asset files.
     *
     * @return  The list of component asset files.
     */
    const juce::StringArray& getAssetFiles() const;

    /**
     * @brief  Applies position and size settings to a UI component.
     * 
     * @param component  Any component object.
     */
    void applyBounds(juce::Component * component) const;
    
    /**
     * @brief   Gets this component's x-coordinate as a fraction of the window's
     *          width.
     * 
     * @return  The x coordinate fraction, or -1 if the x-coordinate fraction
     *          is not defined.
     */
    float getXFraction() const;
     
    /**
     * @brief   Gets this component's y-coordinate as a fraction of the window's
     *          height.
     * 
     * @return  The y coordinate fraction, or -1 if the y-coordinate fraction
     *          is not defined.
     */
    float getYFraction() const;
          
    /**
     * @brief   Gets this component's width as a fraction of the window's width.
     * 
     * @return  The width fraction, or -1 if the width fraction is not 
     *          defined.
     */
    float getWidthFraction() const;      
          
    /**
     * @brief   Gets this component's height as a fraction of the window's 
     *          height.
     * 
     * @return  The height fraction, or -1 if the height fraction is not 
     *          defined.
     */
    float getHeightFraction() const;

    /**
     * @brief   Compares these settings with another settings object.
     *
     * @param rhs  Another settings object to compare with this one.
     *
     * @return  True iff both settings objects define the same position and
     *          size values, and have identical asset and color lists.
     */
    bool operator==(const ComponentSettings& rhs) const;

private:
    /* Position and size data is stored in terms of total screen size,
       e.g. height=0.5 takes up half the screen height,  x=0.2 has a left 
       border 1/5 of the way from the left of the screen, etc. */
    float x;
    float y;
    float width;
    float height;

    /* Custom colors for component image assets. */
    juce::Array<juce::Colour> colours;

    /* Component asset files.  This is mostly intended to define image 
       assets, but there's no reason it couldn't be used to define other
       assets.  */
    juce::StringArray assetFiles;
};


