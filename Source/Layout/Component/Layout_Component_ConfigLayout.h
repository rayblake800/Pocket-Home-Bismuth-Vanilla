#pragma once
/**
 * @file   Layout_Config_ComponentLayout.h
 *
 * @brief  Defines the position and size of a single UI component, relative to
 *         the application window bounds.
 */

#include "JuceHeader.h"

namespace Layout { namespace Component { class ConfigLayout; } }

class Layout::Component::ConfigLayout
{
public:
    /**
     * @brief  Creates an empty layout object.
     */
    ConfigLayout();

    /**
     * @brief  Initializes the layout from JSON data.
     * 
     * @param jsonObj  A dynamic object containing JSON data.
     */
    ConfigLayout(juce::DynamicObject* jsonObj);

    /**
     * @brief   Packages the component layout into a juce::DynamicObject.
     * 
     * @return  The converted component layout.
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
     * @brief  Applies position and size settings to a UI component.
     * 
     * @param component  Any component object.
     */
    void applyBounds(juce::Component * component) const;
    
    /**
     * @brief   Gets the component's x-coordinate as a fraction of the window's
     *          width.
     * 
     * @return  The x coordinate fraction, or -1 if the x-coordinate fraction
     *          is not defined.
     */
    float getXFraction() const;
     
    /**
     * @brief   Gets the component's y-coordinate as a fraction of the window's
     *          height.
     * 
     * @return  The y coordinate fraction, or -1 if the y-coordinate fraction
     *          is not defined.
     */
    float getYFraction() const;
          
    /**
     * @brief   Gets the component's width as a fraction of the window's width.
     * 
     * @return  The width fraction, or -1 if the width fraction is not defined.
     */
    float getWidthFraction() const;      
          
    /**
     * @brief   Gets the component's height as a fraction of the window's 
     *          height.
     * 
     * @return  The height fraction, or -1 if the height fraction is not 
     *          defined.
     */
    float getHeightFraction() const;

    /**
     * @brief   Compares this layout with another layout object.
     *
     * @param rhs  Another component layout object to compare with this one.
     *
     * @return     Whether both settings objects define the same position and
     *             size values.
     */
    bool operator==(const ConfigLayout& rhs) const;

private:
    /* Position and size data is stored in terms of total screen size,
       e.g. height=0.5 takes up half the screen height,  x=0.2 has a left 
       border 1/5 of the way from the left of the screen, etc. */
    float x;
    float y;
    float width;
    float height;
};


