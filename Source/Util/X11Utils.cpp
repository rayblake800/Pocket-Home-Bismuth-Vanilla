#include "X11Utils.h"
#include <cstring>
#include <cstdlib>
#include <X11/Xutil.h>

//####### Checking window properties: ########################################## 
/*
 * Relevant window properties:
 */
//Holds the list of all supported window properties.
static const constexpr char* supportedFeatureProperty = "_NET_SUPPORTED";
// Holds the current active window.
static const constexpr char* activeWindowProperty = "_NET_ACTIVE_WINDOW";
//Holds the index of the current active desktop.
static const constexpr char* currentDesktopProperty = "_NET_CURRENT_DESKTOP";
// Holds the desktop index where a window is located.
static const constexpr char* windowDesktopProperty = "_NET_WM_DESKTOP";

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
 * @param display   The display object used to interact with the X Window 
 *                  server.
 * 
 * @param window    The window to check for the given property.
 * 
 * @param property  The value used by XLib to describe the requested property.
 * 
 * @return  A structure containing the requested property, or one containing no
 *          data if the property was not found.
 */
WindowProperty getWindowProperty
(X11Utils::XDisplay& display, Window window, Atom property);

/**
 * Checks if a particular property is supported by the window manager.
 * 
 * @param display   The display object used to interact with the X Window 
 *                  server.
 * 
 * @param property  The name of a property set by the window manager or pager.
 * 
 * @return  True iff this property is supported by the current window manager
 *           or pager.
 */
bool xPropertySupported(X11Utils::XDisplay& display, const char* property);



/*
 * Create a new interface to the X11 display manager.
 */
X11Utils::XDisplay::XDisplay(const char* displayName) : 
display(XOpenDisplay(displayName)) { }


X11Utils::XDisplay::~XDisplay()
{
    if(display != nullptr)
    {
        XCloseDisplay(display);
        display = nullptr;
    }
}

/*
 * Gets the internal display object.
 */
Display* X11Utils::XDisplay::operator*()
{
    return display;
}

WindowProperty::WindowProperty(const WindowProperty& rhs)
{
    *this = rhs;
}

WindowProperty::~WindowProperty()
{
    if(data != nullptr)
    {
        free(data);
        data = nullptr;
    }
}

WindowProperty& WindowProperty::operator=(const WindowProperty& rhs)
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
WindowProperty getWindowProperty
(X11Utils::XDisplay& display, Window window, Atom property)
{
    unsigned long bytesAfter; //needed for XGetWindowProperty, result unused.
    WindowProperty propertyData;
    int status = XGetWindowProperty(*display, window, property, 0, (~0L), false,
            AnyPropertyType, &propertyData.type, &propertyData.size,
            &propertyData.numItems, &bytesAfter, &propertyData.data);
    
    if (status != Success) 
    {
        if(status == BadWindow)
        {
            DBG("X11Utils::" << __func__ << ": Window " 
                    << juce::String((unsigned long) window) 
                    << "does not exist!");
        }
        DBG("X11Utils::" << __func__ << ": XGetWindowProperty failed!");
        propertyData = WindowProperty();
    }
    return propertyData;
}

/*
 * Checks if a particular property is supported by the window manager.
 */
bool xPropertySupported(X11Utils::XDisplay& display, const char* property)
{
    Window rootWindow = XDefaultRootWindow(*display);
    Atom featureList = XInternAtom(*display, supportedFeatureProperty, false);
    WindowProperty supportedPropertyList 
            = getWindowProperty(display, rootWindow, featureList);
    
    if(supportedPropertyList.data == nullptr 
       || supportedPropertyList.numItems == 0)
    {
        return false;
    }
    
    Atom* propertyList = reinterpret_cast<Atom*>(supportedPropertyList.data);
    Atom neededProp = XInternAtom(*display, property, false);
    for(long i = 0; i < supportedPropertyList.numItems; i++)
    {
        if(propertyList[i] == neededProp)
        {
            return true;
        }
    }
    return false;
}