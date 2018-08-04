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
//Holds the id of the process that created the window.
static const constexpr char* windowProcessProperty = "_NET_WM_PID";

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
Window XWindowInterface::getPocketHomeWindow() const
{
    using namespace juce;
    Component * rootComponent = Desktop::getInstance().getComponent(0);
    Window windowHandle = (Window) rootComponent->getWindowHandle();
    DBG("APP WINDOW:");
    
    #if JUCE_DEBUG
        printWindowInfo(windowHandle);
    #endif
    return windowHandle;
//    const int homeProcess = ProcessUtils::getProcessId();
//    const String windowName = "pocket-home";
//    Array<Window> possibleWindows = getMatchingWindows(
//            [this, &homeProcess, &windowName](Window testWindow)->bool
//            {
//                return getWindowPID(testWindow) == homeProcess
//                        && getWindowName(testWindow) == windowName
//                        && getWindowClass(testWindow).isNotEmpty();
//            });
//    jassert(possibleWindows.size() == 1);
//    return possibleWindows[0];
}
  
/*
 * Gets the title of a window.
 */
juce::String XWindowInterface::getWindowName(const Window window) const
{
    using namespace juce;
    XTextProperty textProp;
    char** nameList = nullptr;
    XGetWMName(display, window, &textProp);
    String name;
    if(textProp.nitems > 0)
    {
        int count = 0;
        Xutf8TextPropertyToTextList(display, &textProp, &nameList, &count);
        if(count > 1)
        {
            DBG("XWindowInterface::" << __func__ << ": " << "Window has "
                    << count << " names, returning comma-separated list.");
        }
        for(int i = 0; i < count; i++)
        {
            String partialName(CharPointer_UTF8(nameList[i]));
            if(!name.isEmpty() && !partialName.isEmpty())
            {
                name += ",";
            }
            name += partialName;
        }
    }
    XFreeStringList(nameList);
    XFree(textProp.value);
    return name;
}
    
/*
 * Gets the class of a window.
 */
juce::String XWindowInterface::getWindowClass(const Window window) const
{
    using namespace juce;
    XClassHint classHint;
    if(!XGetClassHint(display, window, &classHint))
    {
        return String();
    }
    String classStr(classHint.res_name);
    XFree(classHint.res_name);
    XFree(classHint.res_class);
    return classStr;
}

/*
 * Gets the className of a window. 
 */
juce::String XWindowInterface::getWindowClassName(const Window window) const
{
    using namespace juce;
    XClassHint classHint;
    if(!XGetClassHint(display, window, &classHint))
    {
        return String();
    }
    String classStr(classHint.res_class);
    XFree(classHint.res_name);
    XFree(classHint.res_class);
    return classStr;
}

/*
 * Gets the id of the process that created a window.
 */
int XWindowInterface::getWindowPID(const Window window) const
{
    Atom pidAtom = XInternAtom(display, windowProcessProperty, false);
    WindowProperty pidProp = getWindowProperty(window, pidAtom);
    if(pidProp.numItems == 0 || pidProp.size == 0
            || pidProp.data == nullptr)
    {
        return -1;
    }
    return (int) *((unsigned long *) pidProp.data); 
}


/*
 * Gets all child windows of a given parent window.
 */
juce::Array<Window> XWindowInterface::getWindowChildren
(const Window parent) const
{
    using namespace juce;
    Array<Window> children;
    unsigned int numChildren = 0;
    Window* childWindows = nullptr;
    Window unneededReturnVal;
    Status success = XQueryTree(display, parent,
            &unneededReturnVal, &unneededReturnVal,
            &childWindows, &numChildren);
    if(success && numChildren > 0)
    {
        for(unsigned int i = 0; i < numChildren; i++)
        {
            children.add(childWindows[i]);
        }
    }
    if(childWindows != nullptr)
    {
        XFree(childWindows);
        childWindows = nullptr;
    }
    return children;
}

/*
 * Performs a breadth-first search of the entire window tree, returning
 * windows that fit some match function.
 */
juce::Array<Window> XWindowInterface::getMatchingWindows(
            const std::function<bool(const Window)> verifyMatch,
            const bool stopAtFirstMatchDepth) const
{
    using namespace juce;
    const int screenCount = ScreenCount(display);
    Array<Window> matches;
    Array<Window> currentDepth;
    Array<Window> nextDepth;
    for(int i = 0; i < screenCount; i++)
    {
        currentDepth.add(RootWindow(display, i));
    }
    
    while(!currentDepth.isEmpty())
    {
        for(const Window& window : currentDepth)
        {
            if(verifyMatch(window))
            {
                matches.add(window);
            }
            if(stopAtFirstMatchDepth && !matches.isEmpty())
            {
                continue;
            }        
            nextDepth.addArray(getWindowChildren(window));
        }
        if(stopAtFirstMatchDepth && !matches.isEmpty())
        {
            currentDepth.clear();
        }
        else
        {
            currentDepth = nextDepth;
            nextDepth.clear();
        }
    }
    return matches;
}
 
/*
 * Gets all siblings of a window and returns the list sorted from front to
 * back.
 */
juce::Array<Window> XWindowInterface::getWindowSiblings
(const Window window) const
{
    Window parent = getWindowParent(window);
    return getWindowChildren(parent);
}

/*
 * Finds the parent of a window.
 */
Window XWindowInterface::getWindowParent(const Window window) const
{
    juce::Array<Window> parents = getWindowAncestry(window);
    if(parents.size() < 2)
    {
        return 0;
    }
    return parents[parents.size() - 2];    
}

/*
 * Gets a window's index among its siblings in the window tree.  Lower
 * values are closer to the front.
 */
int XWindowInterface::getHeightIndex(const Window window) const
{
    return getWindowSiblings(window).indexOf(window);
}
    
 
/*
 * Finds all direct ancestors of a window and returns them in parent->child
 * order.
 */
juce::Array<Window> XWindowInterface::getWindowAncestry
(const Window window) const
{
    using namespace juce;
    Array<Window> ancestry;
    const int screenCount = ScreenCount(display);
    for(int i = 0; i < screenCount; i++)
    {
        Window root = RootWindow(display, i);
        ancestry.add(root);
        if(root == window)
        {
            return ancestry;
        }
        
        ancestry = recursiveWindowSearch(ancestry, window);
        if(!ancestry.isEmpty())
        {
            break;
        }
    }
    return ancestry; 
}


/*
 * Checks if a specific window is active.
 * 
 * @param window  An XLib window identifier;
 * 
 * @return   True iff the window exists, has nonzero size, is on the current
 *           desktop, has keyboard focus, and is not covered by other
 *           windows.
 */
bool XWindowInterface::isActiveWindow(const Window window) const
{
    using namespace juce;
    XWindowAttributes attr;
    int foundAttributes = XGetWindowAttributes(display, window, &attr);
    if(foundAttributes == 0)
    {
        DBG("XWindowInterface::" << __func__ 
                << ": No, failed to get window attributes.");
        return false;
    }
    if(attr.map_state == IsUnmapped || attr.map_state == IsUnviewable)
    {
        DBG("XWindowInterface::" << __func__ 
                << ": No, window is unmapped or unviewable.");
        return false;
    }
    if(attr.width == 0 || attr.height == 0)
    {
        DBG("XWindowInterface::" << __func__ 
                << ": No, the window has zero area.");
        return false;
    }
    if(getDesktopIndex() != getWindowDesktop(window))
    {
        DBG("XWindowInterface::" << __func__ 
                << ": No, the window is on the wrong desktop.");
        return false;
    }
    
    
    jassert(xPropertySupported(activeWindowProperty));
    const Atom request = XInternAtom(display, activeWindowProperty, false);
    const Window root = XDefaultRootWindow(display);
    WindowProperty windowProp = getWindowProperty(root, request);
    if(windowProp.numItems == 0)
    {
        DBG("XWindowInterface::" << __func__ 
                << ": No, there is no focused window.");
        return false;
    }
    
    if(window != *((Window*) windowProp.data))
    {
        DBG("XWindowInterface::" << __func__ << ": No, window " 
                << getWindowName(*((Window*) windowProp.data))
                << " is focused.");
        return false;
    }
    
    Array<Window> ancestors = getWindowAncestry(window);
    Array<Window> siblings = getWindowChildren(ancestors[ancestors.size() - 2]);
    int higherWindows = (siblings.size() - 1) - siblings.indexOf(window);
    if(higherWindows != 0)
    {
        DBG("XWindowInterface::" << __func__ << ": No, "  << higherWindows 
                << " window(s) are above this window");
        //printWindowInfo(siblings.getLast());
        return false;
    }
    
    DBG("XWindowInterface::" << __func__ << ": Yes, this window is active");
    return true;

}

/*
 * Activates a window.  This will switch the active desktop to the one 
 * containing this window, bring the window to the front, and set it
 * as the focused window.
 */
void XWindowInterface::activateWindow(const Window window) const
{
    using namespace juce;
    DBG("XWindowInterface::" << __func__ << ": activating window:");
    #if JUCE_DEBUG
        printWindowInfo(window);
    #endif
    jassert(xPropertySupported(activeWindowProperty));
    if(xPropertySupported(currentDesktopProperty)
        && xPropertySupported(windowDesktopProperty))
    {
        setDesktopIndex(getWindowDesktop(window));
    }
    
    XEvent xEvent;
    memset(&xEvent, 0, sizeof(xEvent));
    xEvent.type = ClientMessage;
    xEvent.xclient.display = display;
    xEvent.xclient.window = window;
    xEvent.xclient.message_type 
            = XInternAtom(display, activeWindowProperty, false);
    xEvent.xclient.format = 32;
    xEvent.xclient.data.l[0] = 2L; /* 2 == Message from a window pager */
    xEvent.xclient.data.l[1] = CurrentTime;

    XWindowAttributes winAttr;
    XGetWindowAttributes(display, window, &winAttr);
    int result = XSendEvent(display, winAttr.screen->root, false,
            SubstructureNotifyMask | SubstructureRedirectMask,
            &xEvent);

    if(result == BadWindow)
    {
        DBG("XWindowInterface::" << __func__ << ": Bad window error!");
    }
    else if(result == BadValue)
    {
        DBG("XWindowInterface::" << __func__ << ": Bad value error!");
    }
    //Raise and all parent windows before raising the target window
    Array<Window> ancestors = getWindowAncestry(window);
    jassert(!ancestors.isEmpty() && ancestors.getLast() == window);
    
    for(const Window& window : ancestors)
    {
        XWindowAttributes winAttr;
        XGetWindowAttributes(display, window, &winAttr);
        DBG("override_redirect =" << ((int) winAttr.override_redirect));

        //Raise window first(does nothing in some window managers):
        XRaiseWindow(display, window);
    }
    XFlush(display);

}
    
/*
 * Finds the current selected desktop index.
 */
int XWindowInterface::getDesktopIndex() const
{
    if(!xPropertySupported(currentDesktopProperty))
    {
        return -1;
    }
    Atom request = XInternAtom(display, currentDesktopProperty, false);
    Window rootWindow = XDefaultRootWindow(display);
    WindowProperty desktopProp = getWindowProperty(rootWindow, request);
    if(desktopProp.numItems == 0 || desktopProp.size == 0
            || desktopProp.data == nullptr)
    {
        return -1;
    }
    return (int) *((long*) desktopProp.data);
}

/*
 * Sets the current active desktop index.  This will do nothing if the new
 * index is the same as the current index, the index is not a valid desktop 
 * index, or the system does not support multiple desktops.
 */
void XWindowInterface::setDesktopIndex(const int desktopIndex) const
{
    if(!xPropertySupported(currentDesktopProperty))
    {
        return;
    }
    
    Window rootWindow = RootWindow(display, 0);
    XEvent xEvent;
    memset(&xEvent, 0, sizeof(xEvent));
    xEvent.type = ClientMessage;
    xEvent.xclient.display = display;
    xEvent.xclient.window = rootWindow;
    xEvent.xclient.message_type 
            = XInternAtom(display, currentDesktopProperty, false);
    XSendEvent(display, rootWindow, false,
            SubstructureNotifyMask | SubstructureRedirectMask,
            &xEvent);
}

/*
 * Gets the index of the desktop that contains a specific window.
 */
int XWindowInterface::getWindowDesktop(const Window window) const
{
    if(!xPropertySupported(windowDesktopProperty))
    {
        return -1;
    }
    Atom request = XInternAtom(display, windowDesktopProperty, false);
    WindowProperty desktopProp = getWindowProperty(window, request);
    if(desktopProp.numItems == 0 || desktopProp.size == 0
            || desktopProp.data == nullptr)
    {
        return -1;
    }
    return (int) *((long*) desktopProp.data);
}

#if JUCE_DEBUG
/*
 * Prints comprehensive debug information about a window.
 */
void XWindowInterface::printWindowInfo(const Window window) const
{
    using namespace juce;
    String id((unsigned long) window);
    String name = getWindowName(window);
    String claz = getWindowClass(window);
    String className = getWindowClassName(window);
    int desktop = getWindowDesktop(window);
    int pid = getWindowPID(window);
    Array<Window> parents = getWindowAncestry(window);
    
    std::cout << "Window " << id << ": " << name << "[class " << claz
            << ", className " << className << "]\n";
    std::cout << "\tDepth: " << parents.size() << ", Process: " << pid
            << ", Desktop: " << desktop << "\n";
    
    XWindowAttributes attr;
    int foundAttributes = XGetWindowAttributes(display, window, &attr);
    if(foundAttributes == 0)
    {
        std::cout << "\tUnable to read window attributes\n";
        return;
    }
    
    std::cout << "\tPosition: (" << attr.x << "," << attr.y << "), size: ("
            << attr.width << "," << attr.height << ")\n";
}

/*
 * Recursively prints the entire window tree under some root window, from front
 * to back.
 */
void XWindowInterface::printWindowTree(Window root, const int depth)
{
    using namespace juce;
    if(root == 0 && depth == 0)
    {
        root = XDefaultRootWindow(display);
    }
    String output;
    for(int i = 0; i < depth; i++)
    {
        output += "\t";
    }
    output += String((unsigned long) root);
    
    String title = getWindowName(root);
    if(title.isNotEmpty())
    {
        output += ":";
        output += title;
    }
    String winClass = getWindowClass(root);
    String className = getWindowClassName(root);
    if(winClass.isNotEmpty() || className.isNotEmpty())
    {
        output += "[";
        output += winClass;
        if(winClass.isNotEmpty() && className.isNotEmpty())
        {
            output += ", ";
        }
        output += className;
        output += "]";
    }
    std::cout << output << std::endl;
    Array<Window> children = getWindowChildren(root);
    for(int i = children.size() - 1; i >= 0; i--)
    {
        printWindowTree(children[i], depth+1);
    }
}

#endif


/*
 * Recursively search for a specific window's ancestry.
 */
juce::Array<Window> XWindowInterface::recursiveWindowSearch
(juce::Array<Window> parents, const Window searchWin) const
{
    using namespace juce;
    const Window lastParent = parents[parents.size() - 1];
    Array<Window> expandedParents;
    Array<Window> children = getWindowChildren(lastParent);
    for(const Window& child : children)
    {
        expandedParents = parents;
        expandedParents.add(child);
        if(child == searchWin)
        {
            break;
        }
        expandedParents = recursiveWindowSearch(expandedParents, searchWin);
        if(!expandedParents.isEmpty())
        {
            break;
        }
    }
    return expandedParents;
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
(const Window window, const Atom property) const
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
        DBG("XWindowInterface::" << __func__ << ": XGetWindowProperty failed "
                << "to find property " << XGetAtomName(display, property));
        propertyData = WindowProperty();
    }
    return propertyData;
}

/*
 * Checks if a particular property is supported by the window manager.
 */
bool XWindowInterface::xPropertySupported(const char* property) const
{
    Window rootWindow = XDefaultRootWindow(display);
    Atom featureList = XInternAtom(display, supportedFeatureProperty, false);
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
