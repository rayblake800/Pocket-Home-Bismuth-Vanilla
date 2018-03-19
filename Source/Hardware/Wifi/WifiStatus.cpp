#include "JuceHeader.h"
#include "WifiStatus.h"

WifiStatus::~WifiStatus()
{
    if (isThreadRunning())
    {
        DBG("WifiStatus::" << __func__ << ": shutting down thread");
        signalThreadShouldExit();
        Thread::waitForThreadToExit(-1);
    }
}

WifiStatus::Listener::Listener() { }

WifiStatus::Listener::~Listener()
{
    if (wifiStatus != nullptr)
    {
        wifiStatus->removeListener(this);
    }
}

#if JUCE_DEBUG

/**
 * @param event
 * @return a string representation of event for debugging purposes
 */
String WifiStatus::wifiEventString(WifiEvent event)
{
    switch (event)
    {
        case wifiEnabled:
            return "wifiEnabled";
        case wifiDisabled:
            return "wifiDisabled";
        case wifiConnected:
            return "wifiConnected";
        case wifiDisconnected:
            return "wifiDisconnected";
        case wifiConnectionFailed:
            return "wifiConnectionFailed";
    }
}
#endif

/**
 * Add a listener to the list of objects receiving updates on wifi state
 * changes.
 */
void WifiStatus::addListener(WifiStatus::Listener* listener)
{
    const ScopedLock lock(listenerLock);
    if (!listeners.contains(listener))
    {
        listeners.add(listener);
        listener->wifiStatus = this;
    }
}

/**
 * Remove a listener from the list of objects receiving updates on wifi state
 * changes.
 */
void WifiStatus::removeListener(WifiStatus::Listener* listener)
{
    const ScopedLock lock(listenerLock);
    listeners.removeAllInstancesOf(listener);
}

/**
 * Remove all listeners from the list of objects receiving updates on wifi 
 * state changes.
 */
void WifiStatus::clearListeners()
{
    const ScopedLock lock(listenerLock);
    listeners.clear();
}

/**
 * call listener->handleWifiEvent(event) for each registered listener object.
 */
void WifiStatus::notifyListeners(WifiEvent event)
{
    MessageManager::callAsync([this, event]()
    {
        const ScopedLock lock(listenerLock);
        DBG("WifiStatus::" << __func__ << ":" << wifiEventString(event)
                << ": notifying " << listeners.size() << " listeners.");
        for (WifiStatus::Listener* listener : listeners)
        {
            listener->handleWifiEvent(event);
        }
    });
}

/**
 * Shut down the WifiStatus thread when the application window loses
 * focus.
 */
void WifiStatus::windowFocusLost()
{
    DBG("WifiStatus::" << __func__ << ": wifi thread is "
            << (isThreadRunning() ? "running, stopping thread" : "not running"));
    signalThreadShouldExit();
}

/**
 * If wifi is enabled, restart the WifiStatus thread.
 */
void WifiStatus::windowFocusGained()
{
    DBG("WifiStatus::" << __func__ << ": restarting wifi thread");
        startThread();
}
