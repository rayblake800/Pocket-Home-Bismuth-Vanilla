#include "XWindowInterface.h"
#include "ProcessUtils.h"
#include <cstring>
#include <cstdlib>
#include <X11/Xutil.h>

/*
 * All relevant window properties:
 */
//Holds the list of all supported window properties.
static const constexpr char* supportedFeatureProperty = "_NET_SUPPORTED";
// Holds the current active window.
static const constexpr char* activeWindowProperty = "_NET_ACTIVE_WINDOW";
//Holds the index of the current active desktop.
static const constexpr char* currentDesktopProperty = "_NET_CURRENT_DESKTOP";
// Holds the desktop index where a window is located.
static const constexpr char* windowDesktopProperty = "_NET_WM_DESKTOP";

/*
 * Create a new interface to the X11 display manager.
 */
XWindowInterface::XWindowInterface(const char* displayName) :
display(XOpenDisplay(displayName)) { }
        
XWindowInterface::~XWindowInterface()
{
    if(display != nullptr)
    {
        XCloseDisplay(display);
        display = nullptr;
    }
}

/*
 * Gets the XLib window object that represents the pocket-home application
 * window.
 */
Window XWindowInterface::getPocketHomeWindow()
{
    using namespace juce;
    const int homeProcess = ProcessUtils::getProcessId();
    const String windowName = "pocket-home";
    Array<Window> possibleWindows = getMatchingWindows(
            [this, &homeProcess, &windowName](Window testWindow)
            {
                return getWindowPID(testWindow) == homeProcess
                        && windowNameMatches(windowName);
            });
    jassert(possibleWindows.size() == 1);
    return possibleWindows[0];
}

/*
 * Checks if a window's name matches a particular string.
 */   
bool XWindowInterface::windowNameMatches(
    Window window, 
    juce::String& windowName,
    bool ignoreCase,
    bool allowPartialMatch)
{
}

/*
 * Checks if a window's class or classname matches a particular string.
 */
bool XWindowInterface::windowClassMatches(
    Window window, 
    juce::String& windowClass,
    bool ignoreCase,
    bool allowPartialMatch)
{
}

/*
 * Gets the id of the process that created a window.
 */
int XWindowInterface::getWindowPID(Window window)
{
}

/*
 * Performs a breadth-first search of the entire window tree, returning
 * windows that fit some match function.
 */
juce::Array<Window> XWindowInterface::getMatchingWindows(
            std::function<bool(Window)> verifyMatch,
            bool stopAtFirstMatchDepth)
{
}

/*
 * Activates a window.  This will switch the active desktop to the one 
 * containing this window, bring the window to the front, and set it
 * as the focused window.
 */
void XWindowInterface::activateWindow(Window window)
{
}
    
/*
 * Finds the current selected desktop index.
 */
int XWindowInterface::getDesktopIndex()
{
}

/*
 * Sets the current active desktop index.  This will do nothing if the new
 * index is the same as the current index, the index is not a valid desktop 
 * index, or the system does not support multiple desktops.
 */
void XWindowInterface::setDesktopIndex(int desktopIndex)
{
}

/*
 * Gets the index of the desktop that contains a specific window.
 */
int XWindowInterface::getWindowDesktop(Window window)
{
}

XWindowInterface::WindowProperty::WindowProperty
(const XWindowInterface::WindowProperty& rhs)
{
    *this = rhs;
}
    
XWindowInterface::WindowProperty::~WindowProperty()
{
    if(data != nullptr)
    {
        free(data);
        data = nullptr;
    }
}
    
XWindowInterface::WindowProperty& 
XWindowInterface::WindowProperty::operator=
(const XWindowInterface::WindowProperty& rhs)
{
    numItems = rhs.numItems;
    type = rhs.type;
    size = rhs.size;
    if(data != nullptr)
    {
        free(data);
        data = nullptr;
    }
    if(rhs.data != nullptr)
    {
        data = static_cast<unsigned char*>(malloc(numItems * size));
        memcpy(data, rhs.data, numItems * size);
    }
    data = rhs.data;
    return *this;
}

/*
 * Gets an arbitrary window property.
 */
XWindowInterface::WindowProperty
XWindowInterface::getWindowProperty
(Window window, Atom property)
{
    unsigned long bytesAfter; //needed for XGetWindowProperty, result unused.
    XWindowInterface::WindowProperty propertyData;
    int status = XGetWindowProperty(display, window, property, 0, (~0L), false,
            AnyPropertyType, &propertyData.type, &propertyData.size,
            &propertyData.numItems, &bytesAfter, &propertyData.data);
    
    if (status != Success) 
    {
        if(status == BadWindow)
        {
            DBG("XWindowInterface::" << __func__ << ": Window " 
                    << juce::String((unsigned long) window) 
                    << "does not exist!");
        }
        DBG("XWindowInterface::" << __func__ << ": XGetWindowProperty failed!");
        propertyData = WindowProperty();
    }
    return propertyData;
}

/*
 * Checks if a particular property is supported by the window manager.
 */
bool XWindowInterface::xPropertySupported(const char* property)
{
    Window rootWindow = XDefaultRootWindow(display);
    Atom featureList = XInternAtom(*display, supportedFeatureProperty, false);
    WindowProperty supportedPropertyList 
            = getWindowProperty(rootWindow, featureList);
    
    if(supportedPropertyList.data == nullptr 
       || supportedPropertyList.numItems == 0)
    {
        return false;
    }
    
    Atom* propertyList = reinterpret_cast<Atom*>(supportedPropertyList.data);
    Atom neededProp = XInternAtom(display, property, false);
    for(long i = 0; i < supportedPropertyList.numItems; i++)
    {
        if(propertyList[i] == neededProp)
        {
            return true;
        }
    }
    return false;
}
