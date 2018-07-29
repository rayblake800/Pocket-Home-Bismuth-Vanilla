#include "WindowFocus.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <optional>

//holds the current window focus state.
static bool focused = false;

/*
 * Checks if the main application window is currently focused.
 */
bool WindowFocus::isFocused()
{
    return focused;
}

/**
 * Checks if a window's name or application name match a given string.
 * 
 * @param display     The X11 display interface.
 * 
 * @param  window     A window object obtained from the display.
 * 
 * @param windowName  The name of an application or window.
 * 
 * @return  True iff the window's name or application name match windowName,
 *          ignoring case.
 */
static bool checkWindowMatch
(Display* display, Window window, juce::String& windowName)
{
    using namespace juce;
    //Search window name first
    bool nameMatchFound = false;
    XTextProperty textProp;
    char** nameList = nullptr;
    XGetWMName(display, window, &textProp);
    if(textProp.nitems > 0)
    {
        int count = 0;
        Xutf8TextPropertyToTextList(display, &textProp, &nameList, &count);
        for(int i = 0; i < count; i++)
        {
            String listName(CharPointer_UTF8(nameList[i]));
            nameMatchFound = listName.containsIgnoreCase(windowName);
            if(nameMatchFound)
            {
                DBG("WindowFocus::" << __func__ << ": " << windowName
                        << " matches name " << listName << " window=" 
                        << String(reinterpret_cast<unsigned long>(window)));
                break;
            }
        }
        XFreeStringList(nameList);
        XFree(textProp.value);   
    }
    if(nameMatchFound)
    {
        return true;
    }
    
    //If no nameMatch was found, try searching by class 
    XWindowAttributes attr;
    XClassHint classHint;
    XGetWindowAttributes(display, window, &attr);

    if(!XGetClassHint(display, window, &classHint))
    {
        return false;
    }
    bool windowMatches = windowName.equalsIgnoreCase(classHint.res_name) 
            || windowName.equalsIgnoreCase(classHint.res_class);
    DBG("WindowFocus::" << __func__ << ": " << windowName
                << (windowMatches ? " matches" : " doesn't match")
                << " XClassHint name=" << classHint.res_name
                << ", class=" << classHint.res_class << " window=" 
                << String(reinterpret_cast<unsigned long>(window)));
    XFree(classHint.res_name);
    XFree(classHint.res_class);
    return windowMatches;
}

/**
 * Searches all children of a window, returning the first one matching a given
 * window name.
 * 
 * @param display     The X11 display interface.
 * 
 * @param rootWindow  A window object obtained from the display.
 * 
 * @param windowName  A window name to search for among all child windows of
 *                    rootWindow
 * 
 * @return   The first matching child window, or no value if no match is found.
 */
static std::optional<Window> findMatchingChild
(Display* display, Window rootWindow, juce::String& windowName, int depth)
{
    unsigned int numChildren = 0;
    std::optional<Window> matchingWindow;
    Window* childWindows = nullptr;
    Window unneededReturnVal;
    Status success = XQueryTree(display, rootWindow,
            &unneededReturnVal, &unneededReturnVal,
            &childWindows, &numChildren);
    if(!success)
    {
        if(childWindows != nullptr)
        {
            XFree(childWindows);
        }
        return matchingWindow;
    }
    
    if(numChildren > 0)
    {
    DBG("WindowFocus::" << __func__ << ": Depth " << depth << ", searching "
            << (int)numChildren << " child windows");
    }
    //check children for matches
    for(unsigned int i = 0; i < numChildren; i++)
    {
        if(matchingWindow.has_value())
        {
            break;
        }
        if(checkWindowMatch(display, childWindows[i], windowName))
        {
            matchingWindow = childWindows[i];
        }
    }
    
    //recursively check the children of child windows
    for(unsigned int i = 0; i < numChildren; i++)
    {
        if(matchingWindow.has_value())
        {
            break;
        }
        matchingWindow = findMatchingChild
                (display, childWindows[i], windowName, depth + 1);
    }
    
    if(childWindows != nullptr)
    {
        XFree(childWindows);
    }
    return matchingWindow;
}

/**
 * Finds a window in the X Window System.
 * 
 * @param display     The X11 display interface.
 * 
 * @param windowName  The name of a window, or the name of a window's
 *                    application.
 * 
 * @return  The first matching value found, or no value if no match is found.
 */
static std::optional<Window> findWindow
(Display* display, juce::String& windowName)
{
    std::optional<Window> matchingWindow;
    const int screenCount = ScreenCount(display);
    for(int i = 0; i < screenCount; i++)
    {
        Window root = RootWindow(display, i);
        if(checkWindowMatch(display, root, windowName))
        {
            matchingWindow = root;
            break;
        }
        matchingWindow = findMatchingChild(display, root, windowName, 1);
        if(matchingWindow.has_value())
        {
            break;
        }
    }
    
    if(matchingWindow)
    {
        DBG("Found window " << reinterpret_cast<unsigned long>(*matchingWindow));
    }
    return matchingWindow;
}
    
/*
 * Attempts to find and focus a specific application window.  This assumes
 * that the window you're looking for is not on another desktop, as any
 * system running pocket-home should have no good reason to have multiple
 * desktops.  If this assumption is false, focusing the window will not
 * work.
 */
bool WindowFocus::focusWindow(juce::String windowAppName)
{
    Display * display = XOpenDisplay(nullptr);
    if(display == nullptr)
    {
        DBG(__func__ << ": Couldn't open display!");
        return false;
    }
    
    bool windowFocused = false;
    std::optional<Window> matchingWindow = findWindow(display, windowAppName);
    if(matchingWindow.has_value())
    {
        // Adapted from xdo.c:xdo_activate_window at 
        // https://github.com/jordansissel/xdotool
        int result = 0;
        //Raise window first(does nothing in some window managers):
        result = XRaiseWindow(display, *matchingWindow);
        XFlush(display);
        if(result == 0)
        {
            DBG(__func__ << ": window raised, focusing window.");
        }
        else
        {
            DBG(__func__ << ": XRaiseWindow failed, "
                    << "attempting to focus the window anyway.");  
        }
        
        //Focus the window:
        XEvent xev;
        XWindowAttributes winAttr;
        memset(&xev, 0, sizeof(xev));
        xev.type = ClientMessage;
        xev.xclient.display = display;
        xev.xclient.window = *matchingWindow;
        xev.xclient.message_type = XInternAtom(display, 
                "_NET_ACTIVE_WINDOW", False);
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = 2L; /* 2 == Message from a window pager */
        xev.xclient.data.l[1] = CurrentTime;      
        XGetWindowAttributes(display, *matchingWindow, &winAttr);
        result = XSendEvent(display, winAttr.screen->root, False,
                   SubstructureNotifyMask | SubstructureRedirectMask, &xev);
        DBG("__func__" << ": window focus result is " << result);
        windowFocused = (result == 0);
    }
    XCloseDisplay(display);
    return windowFocused;
}

/**
 * Access the listener array through a callback function that will receive
 * the listener array reference as a parameter.  
 * 
 * @param listenerAction  This must not create or destroy any listeners, or 
 *                         call accessListeners itself.  If it does, the 
 *                         application will deadlock.
 */
static void accessListeners(std::function
        <void(juce::Array<WindowFocus::Listener*>&) > listenerAction)
{
    using namespace juce;
    static CriticalSection listenerLock;
    static Array<WindowFocus::Listener*> listeners;
    const ScopedLock lock(listenerLock);
    listenerAction(listeners);
}

WindowFocus::BroadcastWindow::BroadcastWindow(const juce::String& title,
        juce::Colour backgroundColour, int requiredButtons) :
juce::DocumentWindow(title, backgroundColour, requiredButtons) { }

/**
 * Ensure all listeners are notified when window state changes.
 */
void WindowFocus::BroadcastWindow::activeWindowStatusChanged()
{
    using namespace juce;
    if (focused != isActiveWindow())
    {
        focused = !focused;
        if (focused)
        {
            accessListeners([this](Array<Listener*>& listeners)
            {
                DBG("WindowFocus::activeWindowStatusChanged: gained focus,"
                        << " notifying " << listeners.size() << " listeners.");
                for (Listener* listener : listeners)
                {
                    listener->windowFocusGained();
                }
            });
        }
        else
        {
            accessListeners([this](Array<Listener*>& listeners)
            {
                DBG("WindowFocus::activeWindowStatusChanged: lost focus,"
                        << " notifying " << listeners.size() << " listeners.");
                for (Listener* listener : listeners)
                {
                    listener->windowFocusLost();
                }
            });

        }

    }
}

/**
 * Adds itself to the Listener list on creation.
 */
WindowFocus::Listener::Listener()
{
    using namespace juce;
    accessListeners([this] (Array<Listener*>& listeners)
    {
        listeners.add(this);
    });
}

/**
 * Removes itself from the listener list on destruction.
 */
WindowFocus::Listener::~Listener()
{
    using namespace juce;
    accessListeners([this] (Array<Listener*>& listeners)
    {
        listeners.removeAllInstancesOf(this);
    });
}
