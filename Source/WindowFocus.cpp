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
    bool focused = isActiveWindow();
    accessListeners([this, focused](Array<Listener*>& listeners)
    {
        DBG("WindowFocus::activeWindowStatusChanged: notifying "
                << listeners.size() << " listeners that window focus was "
                << (focused ? "gained" : "lost"));
        for (Listener* listener : listeners)
        {
            if (focused)
            {
                listener->windowFocusGained();
            }
            else
            {
                listener->windowFocusLost();

            }
        }
    });
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
