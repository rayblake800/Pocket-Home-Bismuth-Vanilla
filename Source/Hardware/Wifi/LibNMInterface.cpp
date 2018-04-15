#include "LibNMInterface.h"
#include <map>
#include <nm-utils.h>
#include <nm-remote-connection.h>
#include "JuceHeader.h"

#if JUCE_DEBUG

/**
 * Converts a NMDeviceState to a String for debug output.
 */
String deviceStateString(NMDeviceState state)
{
    switch (state)
    {
        case NM_DEVICE_STATE_ACTIVATED:
            return "NM_DEVICE_STATE_ACTIVATED";
        case NM_DEVICE_STATE_PREPARE:
            return "NM_DEVICE_STATE_PREPARE";
        case NM_DEVICE_STATE_CONFIG:
            return "NM_DEVICE_STATE_CONFIG";
        case NM_DEVICE_STATE_IP_CONFIG:
            return "NM_DEVICE_STATE_IP_CONFIG";
        case NM_DEVICE_STATE_IP_CHECK:
            return "NM_DEVICE_STATE_IP_CHECK";
        case NM_DEVICE_STATE_SECONDARIES:
            return "NM_DEVICE_STATE_SECONDARIES";
        case NM_DEVICE_STATE_NEED_AUTH:
            return "NM_DEVICE_STATE_NEED_AUTH";
        case NM_DEVICE_STATE_DISCONNECTED:
            return "NM_DEVICE_STATE_DISCONNECTED";
        case NM_DEVICE_STATE_DEACTIVATING:
            return "NM_DEVICE_STATE_DEACTIVATING";
        case NM_DEVICE_STATE_FAILED:
            return "NM_DEVICE_STATE_FAILED";
        case NM_DEVICE_STATE_UNKNOWN:
            return "NM_DEVICE_STATE_UNKNOWN";
        case NM_DEVICE_STATE_UNMANAGED:
            return "NM_DEVICE_STATE_UNMANAGED";
        case NM_DEVICE_STATE_UNAVAILABLE:
            return "NM_DEVICE_STATE_UNAVAILABLE";
        default:
            return "Invalid state!";
    }
}
#endif

LibNMInterface::LibNMInterface(CriticalSection& wifiLock) :
NetworkInterface(wifiLock),
wifiLock(wifiLock)
{
    //connect signal handlers and update wifi data asynchronously outside of the
    //constructor
    GLibSignalHandler* handler;
    handler->gLibCallAsync([this]()
    {
        connectSignalHandlers();
        updateAllWifiData();
    });
}

/**
 * Check if the network manager found a valid wifi device.
 */
bool LibNMInterface::wifiDeviceFound()
{
    return isWifiAvailable();
}

/**
 * Check the NMDevice state to see if wifi is enabled.
 */
bool LibNMInterface::isWifiEnabled()
{
    const ScopedLock lock(wifiLock);
    switch (lastNMState)
    {
        case NM_DEVICE_STATE_DISCONNECTED:
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_NEED_AUTH:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
        case NM_DEVICE_STATE_ACTIVATED:
        case NM_DEVICE_STATE_DEACTIVATING:
        case NM_DEVICE_STATE_FAILED:
            return true;
    }
    return false;
}

/**
 * Check the NMDevice state to see if wifi is connecting.
 */
bool LibNMInterface::isWifiConnecting()
{
    const ScopedLock lock(wifiLock);
    switch (lastNMState)
    {
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_NEED_AUTH:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
            return true;
    }
    return false;
}

/**
 * Check the NMDevice state to see if wifi is connected.
 */
bool LibNMInterface::isWifiConnected()
{
    const ScopedLock lock(wifiLock);
    DBG("LibNMInterface::" << __func__ << ": state is "
            << deviceStateString(lastNMState));
    if (lastNMState == NM_DEVICE_STATE_ACTIVATED && connectedAP != nullptr)
    {
        return true;
    }
    return false;
}

             
/**
 * Check if the wifi device is connecting but is missing the required 
 * security key.
 */
bool LibNMInterface::isPSKNeeded()
{
    const ScopedLock lock(wifiLock);
    DBG("LibNMInterface::" << __func__ << ": state is "
            << deviceStateString(lastNMState));
    if (lastNMState == NM_DEVICE_STATE_NEED_AUTH && connectingAP != nullptr)
    {
        return true;
    }
    return false;
}

/**
 * Request information on the connected access point from the NMDevice.
 */
WifiAccessPoint::Ptr LibNMInterface::getConnectedAP()
{
    const ScopedLock lock(wifiLock);
    return connectedAP;
}

/**
 * Request information on the connecting access point from the NMDevice.
 */
WifiAccessPoint::Ptr LibNMInterface::getConnectingAP()
{
    const ScopedLock lock(wifiLock);
    return connectingAP;
}

/**
 * Request information on all wifi access points detected by the NMDevice.
 */
Array<WifiAccessPoint::Ptr> LibNMInterface::getVisibleAPs()
{
    const ScopedLock lock(wifiLock);
    return visibleAPs;
}

/**
 * Begin opening a connection to a wifi access point.
 */
void LibNMInterface::connectToAccessPoint(WifiAccessPoint::Ptr toConnect,
        String psk)
{
    initConnection(toConnect, psk);
}

/**
 * Checks the wifi device list, connection state, and active and pending 
 * connections.
 */
void LibNMInterface::updateAllWifiData()
{
    DBG("LibNMInterface::updateAllWifiData reloading data");
    ScopedLock updateLock(wifiLock);
    connectedAP = findConnectedAP();
    connectingAP = findConnectingAP();
    lastNMState = findWifiState();
    visibleAPs = findVisibleAPs();
    ScopedUnlock confirmUnlock(wifiLock);
    confirmWifiState();
}

/**
 * Close the currently active connection, if one exists.
 */
void LibNMInterface::disconnect()
{
    closeActiveConnection();
}

/**
 * Cancel any activating wifi connection.
 */
void LibNMInterface::stopConnecting()
{
    closeActivatingConnection();
}

/**
 * Asynchronously turns on the wifi radio.
 */
void LibNMInterface::enableWifi()
{
    if (!isWifiAvailable())
    {
        signalWifiDisabled();
        return;
    }
    setWifiEnabled(true);
}

/**
 * Asynchronously turns off the wifi radio.
 */
void LibNMInterface::disableWifi()
{
    if (!isWifiAvailable())
    {
        signalWifiDisabled();
        return;
    }
    setWifiEnabled(false);
}

/**
 * Notify listeners and save the connecting access point if starting to
 * connect.
 * 
 * @param connectingAP
 */
void LibNMInterface::connectingCallback(WifiAccessPoint::Ptr connectingAP)
{  
    ScopedLock updateLock(wifiLock);
    this->connectingAP = connectingAP;
    ScopedUnlock confirmUnlock(wifiLock);
    signalWifiConnecting();
}

/**
 * Notify listeners that a connection attempt failed.
 */
void LibNMInterface::connectionFailureCallback()
{   
    ScopedLock updateLock(wifiLock);
    connectingAP = nullptr;
    ScopedUnlock confirmUnlock(wifiLock);
    signalConnectionFailed();
}

/**
 * Notifies listeners when wifi turns on or off.
 */
void LibNMInterface::wifiEnablementChangeCallback(bool isEnabled)
{
    if (isEnabled)
    {
        requestScan();
        signalWifiEnabled();
    }
    else
    {
        signalWifiDisabled();
    }
}

/**
 * A callback function to run whenever the list of wifi access points is 
 * updated.
 */
void LibNMInterface::apUpdateCallback(Array<WifiAccessPoint::Ptr> visibleAPs) 
{
    ScopedLock updateLock(wifiLock);
    this->visibleAPs = visibleAPs;
    DBG("LibNMInterface::" << __func__ << ": found "
            << visibleAPs.size() << " AccessPoints");
 }

/**
 * Registers updates to the wifi device when the NetworkManager device state
 * changes.
 */
void LibNMInterface::stateUpdateCallback(NMDeviceState newState)
{
    ScopedLock updateLock(wifiLock);
    lastNMState = newState;
    DBG("LibNMInterface::" << __func__ << ":  changed to "
            << deviceStateString(newState));
    switch (newState)
    {
        case NM_DEVICE_STATE_ACTIVATED:
        {
            ScopedUnlock unlockForUpdate(wifiLock);
            connectedAP = findConnectedAP();
            if(connectedAP == nullptr)
            {
                DBG("LibNMInterface::" << __func__ 
                        << ":  AP connected but not found!");
            }
            else
            {
                signalWifiConnected(connectedAP);
            }
            break;
        }
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
        case NM_DEVICE_STATE_NEED_AUTH:
            /* No state change for now, wait for connection to complete/fail */
            //ensure the pending connection is registered
            if (connectingAP == nullptr)
            {
                connectingAP = findConnectingAP();
                if(connectingAP == nullptr)
                {
                    DBG("LibNMInterface::" << __func__ 
                            << ":  AP connecting but not found!");
                }
                else
                {
                    ScopedUnlock unlockForUpdate(wifiLock);
                    signalWifiConnecting();
                }
            }
            stopTimer();
            startTimer(wifiConnectionTimeout);
            break;
        case NM_DEVICE_STATE_DISCONNECTED:
        {
            ScopedUnlock unlockForUpdate(wifiLock);
            if(connectingAP == nullptr)
            {
                signalWifiDisconnected();
            }
            else
            {
                connectingAP = nullptr;
                signalConnectionFailed();
            }
            break;
        }
        case NM_DEVICE_STATE_DEACTIVATING:
        case NM_DEVICE_STATE_FAILED:
        {
            ScopedUnlock unlockForUpdate(wifiLock);
            signalWifiDisconnected();
            break;
        }
        case NM_DEVICE_STATE_UNKNOWN:
        case NM_DEVICE_STATE_UNMANAGED:
        case NM_DEVICE_STATE_UNAVAILABLE:
        default:
        {
            DBG("LibNMInterface::" << __func__
                    << ": wlan0 device entered unmanaged state: "
                    << deviceStateString(newState));
            ScopedUnlock unlockForUpdate(wifiLock);
            signalWifiDisconnected();
        }
    }
}

/**
 * Notifies listeners when the active access point changes.
 */
void LibNMInterface::connectionUpdateCallback(WifiAccessPoint::Ptr connected)
{
    ScopedLock updateLock(wifiLock);
    connectedAP = connected;
    ScopedUnlock notifyUnlock(wifiLock);
    if (connected == nullptr)
    {
        signalWifiDisconnected();
    }
    else
    {
        signalWifiConnected(connected);
    }
}


/**
 * called whenever the application window gains focus.
 */
void LibNMInterface::windowFocusGained()
{
    connectSignalHandlers();
    updateAllWifiData();
}

/**
 * called whenever the application window loses focus.
 */
void LibNMInterface::windowFocusLost()
{
    disconnectSignalHandlers();
    stopTimer();
}
