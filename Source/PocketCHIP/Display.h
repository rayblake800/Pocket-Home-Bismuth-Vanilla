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
}
