#include "WindowFocus.h"

/**
 * Access the listener array through a callback function that will receive
 * the listener array reference as a parameter.  
 * 
 * @param listenerAction  This must not create or destroy any listeners, or 
 *                         call accessListeners itself.  If it does, the 
 *                         application will deadlock.
 */
static void accessListeners(std::function
        <void(Array<WindowFocus::Listener*>&) > listenerAction)
{
    static CriticalSection listenerLock;
    static Array<WindowFocus::Listener*> listeners;
    const ScopedLock lock(listenerLock);
    listenerAction(listeners);
}

WindowFocus::BroadcastWindow::BroadcastWindow
(const String& title, Colour backgroundColour, int requiredButtons) :
DocumentWindow(title, backgroundColour, requiredButtons) { }

/**
 * Ensure all listeners are notified when window state changes.
 */
void WindowFocus::BroadcastWindow::activeWindowStatusChanged()
{
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
    accessListeners([this] (Array<Listener*>& listeners)
    {
        listeners.removeAllInstancesOf(this);
    });
}
