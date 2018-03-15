#include "WindowFocus.h"

/**
 * Access the listener array through a callback function that will receive
 * the listener array reference as a parameter.  
 * 
 * @param listenerAction must not create or destroy any listeners or 
 * call accessListeners itself or the application will deadlock.
 */
static void accessListeners(std::function
        <void(Array<WindowFocus::Listener*>&) > listenerAction)
{
    static CriticalSection listenerLock;
    static Array<WindowFocus::Listener*> listeners;
    const ScopedLock lock(listenerLock);
    listenerAction(listeners);
}

WindowFocus::BroadcastWindow::BroadcastWindow(const String& title,
        Colour backgroundColour, int requiredButtons) :
DocumentWindow(title, backgroundColour, requiredButtons) { }

WindowFocus::BroadcastWindow::~BroadcastWindow() { }

/**
 * Ensure all listeners are notified when window state changes.
 */
void WindowFocus::BroadcastWindow::activeWindowStatusChanged()
{
    //TODO: find out why the window system triggers a bunch of repeated 
    //focus loss effects on program start.  For now, use a static value to
    //prevent repeats of the same focus event type.
    static bool focused = !isActiveWindow();
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
        else{
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

WindowFocus::Listener::Listener()
{
    accessListeners([this] (Array<Listener*>& listeners)
    {
        listeners.add(this);
    });
}

WindowFocus::Listener::~Listener()
{
    accessListeners([this] (Array<Listener*>& listeners)
    {
        listeners.removeAllInstancesOf(this);
    });
}
