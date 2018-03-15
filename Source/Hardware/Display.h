/**
 * @file Display.h
 * 
 * Gets and sets display brightness.
 */

#pragma once
namespace Display{
    /**
     * @return display brightness, as an integer between 1 and 10
     */
    int getBrightness();
    
    /**
     * Set display brightness
     * @param brightness should be an integer between 1 and 10. Values outside
     * of this range will be rounded to the closest valid integer.
     */
    void setBrightness(int brightness);
}
