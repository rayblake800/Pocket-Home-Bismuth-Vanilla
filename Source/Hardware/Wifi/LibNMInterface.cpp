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

LibNMInterface::LibNMInterface()
{
    setNMCallbacks();
    connectSignalHandlers();
    updateAllWifiData();
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
 * @return true iff the wifi device is currently connecting to an access point.
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
    if (lastNMState == NM_DEVICE_STATE_ACTIVATED && !connectedAP.isVoid())
    {
        return true;
    }
    return false;
}

/**
 * Request information on the connected access point from the NMDevice.
 */
WifiAccessPoint LibNMInterface::getConnectedAP()
{
    const ScopedLock lock(wifiLock);
    return connectedAP;
}

/**
 * Request information on the connecting access point from the NMDevice.
 */
WifiAccessPoint LibNMInterface::getConnectingAP()
{
    const ScopedLock lock(wifiLock);
    return connectingAP;
}

/**
 * Request information on all wifi access points detected by the NMDevice.
 */
Array<WifiAccessPoint> LibNMInterface::getVisibleAPs()
{
    const ScopedLock lock(wifiLock);
    return visibleAPs;
}

/**
 * Asynchronously attempt to connect to a wifi access point.
 */
void LibNMInterface::connectToAccessPoint(const WifiAccessPoint& toConnect,
        String psk)
{
    initConnection(toConnect,psk,
            [this](WifiAccessPoint ap)
            {
                connectingAP = ap;
                signalWifiConnecting();
            },
            [this]()
            {
                connectingAP = WifiAccessPoint();
                signalConnectionFailed();
            });
}

/**
 * Asynchronously checks the wifi device list, connection state, and 
 * active and pending connections.
 */
void LibNMInterface::updateAllWifiData()
{
    ScopedLock updateLock(wifiLock);
    connectedAP = findConnectedAP();
    connectingAP = findConnectingAP();
    lastNMState = findWifiState();
    visibleAPs = updatedVisibleAPs();
    ScopedUnlock confirmUnlock(wifiLock);
    confirmWifiState();
}

/**
 * Removes the active wifi connection
 */
void LibNMInterface::disconnect()
{
    closeActiveConnection();
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
 * Attaches LibNMInterface callback functions to the network manager client
 * and wifi device in order to receive updates when wifi actions occur.
 */
void LibNMInterface::setNMCallbacks()
{
    ScopedLock initLock(wifiLock);
    //Signal: notifies that wifi has turned on or off
    setWifiEnablementChangeCallback([this](bool enabled)
    {
        if(enabled)
        {
            requestScan();
            signalWifiEnabled();
        }
        else
        {
            signalWifiDisabled();
        }
    });

    //Signal: notifies that wifi state has changed
    setStateUpdateCallback([this](NMDeviceState newState)
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
                signalWifiConnected(findConnectedAP());
            break;
        }
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
            /* No state change for now, wait for connection to complete/fail */
            //ensure the pending connection is registered
            if (connectingAP.isVoid())
            {
                connectingAP = findConnectingAP();
                ScopedUnlock unlockForUpdate(wifiLock);
                signalWifiConnecting();
            }
            break;
        case NM_DEVICE_STATE_NEED_AUTH:
        {
            /* FIXME: let this drop into the general failed case for now
             *        eventually this should prompt the user
             */
            DBG("LibNMInterface::" << __func__
                    << " missing auth, canceling connection.");
            ScopedUnlock unlockForUpdate(wifiLock);
            closeActivatingConnection();
        }
        case NM_DEVICE_STATE_DISCONNECTED:
        {
            ScopedUnlock unlockForUpdate(wifiLock);
            signalConnectionFailed();
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
    });

    //Signal: notifies that the active access point has changed
    setConnectionUpdateCallback([this](WifiAccessPoint connection)
    {
        ScopedLock updateLock(wifiLock);
        connectedAP = connection;
        ScopedUnlock notifyUnlock(wifiLock);
        if(connection.isVoid())
        {
            signalWifiDisconnected();
        }
        else
        {
            signalWifiConnected(connection);
        }
    });

    //Signal: notifies that the visible access point list changed.
    setConnectionUpdateCallback([this](Array<WifiAccessPoint> visible)
    {
        ScopedLock updateLock(wifiLock);
        visibleAPs = visible;
        DBG("LibNMInterface::" << __func__ << ": found "
                << visibleAPs.size() << " AccessPoints");
    });
}

/**
 * called whenever the application window gains focus.
 */
void LibNMInterface::windowFocusGained()
{
    connectSignalHandlers();
}

/**
 * called whenever the application window loses focus.
 */
void LibNMInterface::windowFocusLost()
{
    disconnectSignalHandlers();
}
