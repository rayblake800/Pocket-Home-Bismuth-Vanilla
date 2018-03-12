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

/**
 * Add a listener to the list of objects receiving updates on wifi state
 * changes.
 */
void WifiStatus::addListener(WifiStatus::Listener* listener)
{
    if (listeners.contains(listener))
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
    listeners.removeAllInstancesOf(listener);
}

/**
 * Remove all listeners from the list of objects receiving updates on wifi 
 * state changes.
 */
void WifiStatus::clearListeners()
{
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
 * call listener->handleWifiEnabled() for each registered listener object.
 */
void WifiStatus::notifyListenersWifiEnabled()
{
    for (WifiStatus::Listener* listener : listeners)
    {
        listener->handleWifiEnabled();
    }
}

/**
 * call listener->handleWifiDisabled() for each registered listener object.
 */
void WifiStatus::notifyListenersWifiDisabled()
{
    for (WifiStatus::Listener* listener : listeners)
    {
        listener->handleWifiDisabled();
    }
}

/**
 * call listener->handleWifiConnected() for each registered listener object.
 */
void WifiStatus::notifyListenersWifiConnected()
{
    for (WifiStatus::Listener* listener : listeners)
    {
        listener->handleWifiConnected();
    }
}

/**
 * call listener->handleWifiDisconnected() for each registered listener 
 * object.
 */
void WifiStatus::notifyListenersWifiDisconnected()
{
    for (WifiStatus::Listener* listener : listeners)
    {
        listener->handleWifiDisconnected();
    }
}

/**
 * call listener->handleWifiFailedConnect() for each registered listener 
 * object.
 */
void WifiStatus::notifyListenersWifiFailedConnect()
{
    for (WifiStatus::Listener* listener : listeners)
    {
        listener->handleWifiFailedConnect();
    }
}

/**
 * call listener->handleWifiBusy() for each registered listener object.
 */
void WifiStatus::notifyListenersWifiBusy()
{
    for (WifiStatus::Listener* listener : listeners)
    {
        listener->handleWifiBusy();
    }
}
