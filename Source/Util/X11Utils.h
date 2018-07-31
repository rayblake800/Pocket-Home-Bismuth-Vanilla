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
     *                           Window system.
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
            XDisplay& display,
            Window window, 
            juce::String& windowName,
            bool ignoreCase = true,
            bool allowPartialMatch = false);


    /**
     * Checks if a window's class or classname matches a particular string.
     * 
     * @param display            The display object used to interact with the X 
     *                           Window system.
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
            XDisplay& display,
            Window window, 
            juce::String& windowClass,
            bool ignoreCase = true,
            bool allowPartialMatch = false);

    /**
     * Gets the id of the process that created a window.
     * 
     * @param display            The display object used to interact with the X 
     *                           Window system.
     * 
     * @param window             Identifies the window being checked.
     * 
     * @return  The process id of the process that created the window.
     *
     */
    int getWindowPID(XDisplay& display, Window window);

    /**
     * Performs a breadth-first search of the entire window tree, returning
     * windows that fit some match function.
     *
     * @param display                 The display object used to interact with
     *                                the X Window server.
     *
     * @param verifyMatch             An arbitrary matching function that will
     *                                be used to determine which windows are 
     *                                matching.  When passed the display and a
     *                                window, iff verifyMatch returns true, the
     *                                window will be added to the array returned
     *                                by getMatchingWindows
     *
     * @param stopAtFirstMatchDepth   If true, once a matching window is found,
     *                                the search will not check any windows
     *                                that are deeper on the window tree.  If
     *                                false, the search will continue to search
     *                                further down on the window tree.
     *
     * @return                        All windows selected by the verifyMatch
     *                                function.
     */
    juce::Array<Window> getMatchingWindows(
                    XDisplay& display,
                    std::function<bool(XDisplay&, Window)> verifyMatch,
                    bool stopAtFirstMatchDepth = true);

    /**
     * Activates a window.  This will switch the active desktop to the one 
     * containing this window, bring the window to the front, and set it
     * as the focused window.
     *
     * @param display  The display object used to interact with the X Window
     *                 system.
     *
     * @param window   The window to activate.
     */
    void activateWindow(XDisplay& display, Window window);
    
    /**
     * Finds the current selected desktop index.
     *
     * @param display  The display object used to interact with the X Window
     *                 system.
     *
     * @return   The index of the current active desktop, or -1 if the system
     *           does not support multiple desktops
     */
    int getDesktop(XDisplay& display);

    /**
     * Sets the current active desktop index.  This will do nothing if the new
     * index is the same as the current index, the index is not a valid desktop 
     * index, or the system does not support multiple desktops.
     *
     * @param display       The display object used to interact with the X 
     *                      Window system.
     *
     * @param desktopIndex  The index of the desktop to set as active.
     */
    void setDesktop(XDisplay& display, int desktopIndex)

    /**
     * Gets the index of the desktop that contains a specific window.
     *
     * @param display  The display object used to interact with the X Window
     *                 system.
     *
     * @param window   The window to check.
     *
     * @return   The index of the desktop containing the window, or -1 if
     *           the window is invalid or the system does not support multiple
     *           desktops.
     */
    int getWindowDesktop(XDisplay& display, Window window);
}
