#include "WifiStatus.h"
#include "../JuceLibraryCode/JuceHeader.h"

ScopedPointer<WifiStatus> WifiStatus::instance = nullptr;

WifiStatus::~WifiStatus() { }

/**
 * @return the WifiStatus thread, or nullptr if the instance hasn't been
 * set yet.
 */
WifiStatus* WifiStatus::getInstance()
{
    return instance;
}

/**
 * Sets the WifiStatus thread instance.  This should only ever happen once
 * per program instance, after it is determined which WifiStatus subclass 
 * should be used.
 */
void WifiStatus::setInstance(WifiStatus* wifiStatus)
{
    if (instance != nullptr)
    {
        throw ExtraWifiInitException();
    }
    else
    {
        instance = wifiStatus;
    }
}

WifiStatus::Listener::Listener() { }

WifiStatus::Listener::~Listener() { }

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
        case wifiBusy:
            return "wifiBusy";
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
 * @throw ExtraWifiInitException if the WifiStatus instance already exists
 */
WifiStatus::WifiStatus() : Thread("WifiStatusThread")
{
    if (instance != nullptr)
    {
        throw ExtraWifiInitException();
    }
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
    if (isEnabled())
    {
        DBG("WifiStatus::" << __func__ << ": restarting wifi thread");
        startThread();
    }
    else
    {
        DBG("WifiStatus::" << __func__
                << ": wifi disabled, skipping wifi thread start");
    }
}
