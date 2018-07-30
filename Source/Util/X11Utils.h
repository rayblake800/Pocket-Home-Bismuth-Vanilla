/**
 * @file X11Utils.h
 * 
 * @brief Interacts with the X Window System to find and manipulate windows.
 * 
 * This module searches for windows by window title, application name, or 
 * process id.  Once found, it can focus these windows, and raise them above
 * other windows on the display.
 * 
 * Most of the code in this module was adapted from xdotool, an extremely useful
 * utility for manipulating windows and automatically triggering mouse and 
 * keyboard events.  
 * 
 * @see https://github.com/jordansissel/xdotool
 */

#pragma once
#include <X11/Xlib.h>
#include "JuceHeader.h"

namespace X11Utils
{
    /**
     * A simple RAII wrapper for managing XLib Display objects.  Display
     * objects maintain a connection to the X Window server, and are required
     * by nearly all XLib functions.
     */
    class XDisplay
    {
    public:
        /**
         * Create a new interface to the X11 display manager.
         * 
         * @param displayName  The name of the display to access.  If null, the
         *                     default display set by the $DISPLAY environment
         *                     variable will be used.
         */
        XDisplay(const char* displayName = nullptr);
        
        ~XDisplay();
        
        /**
         * Gets the internal display object.
         */
        Display* operator*();
        
    private:
        Display* display = nullptr;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XDisplay)
    };
}
