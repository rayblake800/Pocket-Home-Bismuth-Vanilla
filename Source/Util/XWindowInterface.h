/**
 * @file XWindowInterface.h
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
#include <X11/Xutil.h>
#include "JuceHeader.h"

class XWindowInterface
{
public:
   /**
    * Create a new interface to the X11 display manager.
    * 
    * @param displayName  The name of the display to access.  If null, the
    *                     default display set by the $DISPLAY environment
    *                     variable will be used.
    */
    XWindowInterface(const char* displayName = nullptr);
        
    ~XWindowInterface();
 
    /**
     * Gets the XLib window object that represents the pocket-home application
     * window.
     *
     * @return  The main pocket-home window used by this application.
     */
    Window getPocketHomeWindow();    
    /**
     * Checks if a window's name matches a particular string.
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
            Window window, 
            juce::String& windowName,
            bool ignoreCase = true,
            bool allowPartialMatch = false);

    /**
     * Checks if a window's class or classname matches a particular string.
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
            Window window, 
            juce::String& windowClass,
            bool ignoreCase = true,
            bool allowPartialMatch = false);

    /**
     * Gets the id of the process that created a window.
     * 
     * @param window             Identifies the window being checked.
     * 
     * @return  The process id of the process that created the window.
     *
     */
    int getWindowPID(Window window);

    /**
     * Performs a breadth-first search of the entire window tree, returning
     * windows that fit some match function.
     *
     * @param verifyMatch             An arbitrary matching function that will
     *                                be used to determine which windows are 
     *                                matching.  When called passing in a Window
     *                                parameter, iff verifyMatch returns true, 
     *                                the window will be added to the array 
     *                                returned by getMatchingWindows.
     *
     * @param stopAtFirstMatchDepth   If true, once a matching window is found,
     *                                the search will not check any windows
     *                                that are deeper in the window tree.  If
     *                                false, the search will continue to search
     *                                further down on the window tree.
     *
     * @return                        All windows selected by the verifyMatch
     *                                function.
     */
    juce::Array<Window> getMatchingWindows(
                    std::function<bool(Window)> verifyMatch,
                    bool stopAtFirstMatchDepth = true);

    /**
     * Activates a window.  This will switch the active desktop to the one 
     * containing this window, bring the window to the front, and set it
     * as the focused window.
     *
     * @param window   The window to activate.
     */
    void activateWindow(Window window);
    
    /**
     * Finds the current selected desktop index.
     *
     * @return   The index of the current active desktop, or -1 if the system
     *           does not support multiple desktops
     */
    int getDesktopIndex();

    /**
     * Sets the current active desktop index.  This will do nothing if the new
     * index is the same as the current index, the index is not a valid desktop 
     * index, or the system does not support multiple desktops.
     *
     * @param desktopIndex  The index of the desktop to set as active.
     */
    void setDesktopIndex(int desktopIndex);

    /**
     * Gets the index of the desktop that contains a specific window.
     *
     * @param window   The window to check.
     *
     * @return   The index of the desktop containing the window, or -1 if
     *           the window is invalid or the system does not support multiple
     *           desktops.
     */
    int getWindowDesktop(Window window);
    
private:
 
    //holds arbitrary window property data.
    struct WindowProperty
    {
        unsigned char* data = nullptr;
        Atom type = 0;
        int size = 0;
        unsigned long numItems = 0;
    
        WindowProperty() { }
    
        WindowProperty(const WindowProperty& rhs);
    
        ~WindowProperty();
    
        WindowProperty& operator=(const WindowProperty& rhs);
    };

    /**
     * Gets an arbitrary window property.
     *
     * @param window    The window to check for the given property.
     * 
     * @param property  The value used by XLib to describe the requested 
     *                  property.
     * 
     * @return  A structure containing the requested property, or one 
     *          containing no data if the property was not found.
     */
    WindowProperty getWindowProperty(Window window, Atom property);

    /**
     * Checks if a particular property is supported by the window manager.
     * 
     * @param property  The name of a property set by the window manager or 
     *                  pager.
     * 
     * @return  True iff this property is supported by the current window 
     *          manager or pager.
     */
    bool xPropertySupported(const char* property);

   //XLib display object used to connect to the X Window system.
    Display* display = nullptr; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XWindowInterface)
};
