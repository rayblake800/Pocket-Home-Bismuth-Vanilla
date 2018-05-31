#pragma once
#include "JuceHeader.h"

/**
 * @file Display.h
 * 
 * @brief Contains functions related to display management and properties.
 */

namespace Display
{

    /**
     * Gets the current display brightness level.
     * 
     * @return display brightness, as an integer between 1 and 10.
     */
    int getBrightness();

    /**
     * Sets the display brightness level.
     * 
     * @param brightness   This should be an integer between 1 and 10. Values 
     *                      outside of this range will be rounded to the closest
     *                      valid integer.
     */
    void setBrightness(const int& brightness);
    
    /**
     * Gets the size of the active display.
     * 
     * @return the size of the display, measured in pixels.
     */
    Rectangle<int> getDisplaySize();

    /**
     * Gets the size of the single application window.
     * 
     * @return the size of the active window, measured in pixels.
     */
    Rectangle<int> getWindowSize();
}
