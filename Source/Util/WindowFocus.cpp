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
