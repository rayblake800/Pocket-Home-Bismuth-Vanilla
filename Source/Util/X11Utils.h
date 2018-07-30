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
    
    /**
     * Checks if a window's name matches a particular string.
     * 
     * @param display            The display object used to interact with the X 
     *                           Window server.
     * 
     * @param window             Identifies the window being checked.
     * 
     * @param windowName         The string being checked against the window 
     *                           title.
     * 
     * @param ignoreCase         If true, the comparison will ignore case, 
     *                           treating capital letters as identical to their 
     *                           lowercase equivalents.
     * 
     * @param allowPartialMatch  If true, the window's title and windowName will
     *                           be considered matching not only when both 
     *                           strings are equivalent, but also when one 
     *                           string contains the other.
     * 
     * @return   True iff the window's title matches the windowName string. 
     *
     */
    bool windowNameMatches(
            X11Utils::XDisplay& display,
            Window window, 
            juce::String& windowName,
            bool ignoreCase = true,
            bool allowPartialMatch = false);


    /**
     * Checks if a window's class or classname matches a particular string.
     * 
     * @param display            The display object used to interact with the X 
     *                           Window server.
     * 
     * @param window             Identifies the window being checked.
     * 
     * @param windowClass        The string being checked against the window's 
     *                           class and classname.
     * 
     * @param ignoreCase         If true, the comparison will ignore case, 
     *                           treating capital letters as identical to their 
     *                           lowercase equivalents.
     * 
     * @param allowPartialMatch  If true, the window's class or classname and 
     *                           windowClass will be considered matching not 
     *                           only when both strings are equivalent, but also
     *                           when one string contains the other.
     * 
     * @return   True iff the window's class or classname strings match the 
     *           windowClass string.
     *
     */
    bool windowClassMatches(
            X11Utils::XDisplay& display,
            Window window, 
            juce::String& windowClass,
            bool ignoreCase = true,
            bool allowPartialMatch = false);

    /**
     * Gets the id of the process that created a window.
     * 
     * @param display            The display object used to interact with the X 
     *                           Window server.
     * 
     * @param window             Identifies the window being checked.
     * 
     * @return  The process id of the process that created the window.
     *
     */
    int getWindowPID(X11Utils::XDisplay& display, Window window);
}
