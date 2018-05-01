#include "LibNMInterface.h"
#include <map>
#include <nm-utils.h>
#include <nm-remote-connection.h>
#include "JuceHeader.h"
#include "SavedConnections.h"
#if JUCE_DEBUG
#include "WifiDebugOutput.h"
#endif

LibNMInterface::LibNMInterface(CriticalSection& wifiLock) :
NetworkInterface(wifiLock),
wifiLock(wifiLock)
{
    updateAllWifiData();
}

/*
 * Check if the network manager found a valid wifi device.
 */
bool LibNMInterface::wifiDeviceFound()
{
    const ScopedLock lock(wifiLock);
    return !wifiDevice.isNull();
}

/*
 * Check the NMDevice state to see if wifi is enabled.
 */
bool LibNMInterface::isWifiEnabled()
{
    const ScopedLock lock(wifiLock);
    return client.wirelessEnabled();
}

/**
 * Check the NMDevice state to see if wifi is connecting.
 */
bool LibNMInterface::isWifiConnecting()
{
    const ScopedLock lock(wifiLock);
    switch (activatingConnection.getConnectionState())
    {
        case NM_ACTIVE_CONNECTION_STATE_ACTIVATING:
        case NM_ACTIVE_CONNECTION_STATE_ACTIVATED:
            return true;
    }
    return false;
}

/*
 * Check the NMDevice state to see if wifi is connected.
 */
bool LibNMInterface::isWifiConnected()
{
    const ScopedLock lock(wifiLock);
    return activeConnection.getConnectionState() 
            == NM_ACTIVE_CONNECTION_STATE_ACTIVATED;
}

/*
 * Request information on the connected access point from the NMDevice.
 */
WifiAccessPoint LibNMInterface::getConnectedAP()
{
    const ScopedLock lock(wifiLock);
    WifiAccessPoint connected(connectedAP);
    connected.setConnectionPath(activeConnection.getAccessPointPath());
    return connected;
}

/*
 * Request information on the connecting access point from the NMDevice.
 */
WifiAccessPoint LibNMInterface::getConnectingAP()
{
    const ScopedLock lock(wifiLock);
    WifiAccessPoint connecting(connectingAP);
    connecting.setConnectionPath(activatingConnection.getAccessPointPath());
    return connecting;
}

/*
 * Request information on all wifi access points detected by the NMDevice.
 */
Array<WifiAccessPoint> LibNMInterface::getVisibleAPs()
{
    const ScopedLock lock(wifiLock);
    Array<WifiAccessPoint> filteredAPs;
    filteredAPs.addIfNotAlreadyThere(getConnectedAP());
    filteredAPs.addIfNotAlreadyThere(getConnectingAP());
    Array<SavedConnection> savedCons = savedConnections.getWifiConnections();
    for(const NMPPAccessPoint& ap : visibleAPs)
    {
        WifiAccessPoint packagedAP(ap);
        if(!filteredAPs.contains(packagedAP))
        {
            for(const SavedConnection& con : savedCons)
            {
                if(ap.isValidConnection(con.getNMConnection()))
                {
                    packagedAP.setConnectionPath(con.getPath());
                    break;
                }
            }
            filteredAPs.add(packagedAP);
        }
    }
    return filteredAPs;
}

/*
 * Begin opening a connection to a wifi access point.
 */
void LibNMInterface::connectToAccessPoint(WifiAccessPoint toConnect,
        String psk)
{
    initConnection(toConnect, psk);
}

/*
 * Checks the wifi device list, connection state, and active and pending 
 * connections.
 */
void LibNMInterface::updateAllWifiData()
{
    DBG("LibNMInterface::updateAllWifiData reloading data");
    ScopedLock updateLock(wifiLock);
    lastNMState = wifiDevice.getState();
    savedConnections.updateSavedConnections();
    activeConnection = wifiDevice.getActiveConnection();
    connectedAP = wifiDevice.getActiveAccessPoint();
    activatingConnection = client.getActivatingConnection();
    connectingAP = wifiDevice.getAccessPoint
            (activatingConnection.getAccessPointPath());
    visibleAPs = wifiDevice.getAccessPoints();
    ScopedUnlock confirmUnlock(wifiLock);
    confirmWifiState();
}

/*
 * Close the currently active connection, if one exists.
 */
void LibNMInterface::disconnect()
{
    wifiDevice.disconnect();
}

/*
 * Cancel any activating wifi connection.
 */
void LibNMInterface::stopConnecting()
{
    client.deactivateConnection(activatingConnection);
}

/*
 * Asynchronously turns on the wifi radio.
 */
void LibNMInterface::enableWifi()
{
    if (wifiDevice.isNull())
    {
        signalWifiDisabled();
        return;
    }
    client.setWirelessEnabled(true);
}

/*
 * Asynchronously turns off the wifi radio.
 */
void LibNMInterface::disableWifi()
{
    if (wifiDevice.isNull())
    {
        signalWifiDisabled();
        return;
    }
    client.setWirelessEnabled(false);
}

/*
 * Notify listeners and save the connecting access point if starting to
 * connect.
 */
void LibNMInterface::connectingCallback(WifiAccessPoint::Ptr connectingAP)
{  
    ScopedLock updateLock(wifiLock);
    if(this->connectingAP != connectingAP)
    {
        this->connectingAP = connectingAP;
        ScopedUnlock confirmUnlock(wifiLock);
        signalWifiConnecting();
    }
}

/*
 * Notify listeners that a connection attempt failed.
 */
void LibNMInterface::connectionFailureCallback()
{   
    ScopedLock updateLock(wifiLock);
    if(connectingAP != nullptr)
    {
        connectingAP = nullptr;
        ScopedUnlock confirmUnlock(wifiLock);
        signalConnectionFailed();
    }
}

    
/*
 * Notify listeners that connection security settings were invalid.
 */
void LibNMInterface::invalidSecurityCallback()
{   
    ScopedLock updateLock(wifiLock);
    if(connectingAP != nullptr)
    {
        if(connectingAP->getRequiresAuth())
        {
            connectingAP->removePsk();
            connectingAP = nullptr;
            ScopedUnlock confirmUnlock(wifiLock);
            signalPskNeeded();
        }
        else
        {
            DBG("LibNMInterface::" << __func__ 
                    << ": access point does not actually require security");
            connectingAP = nullptr;
            ScopedUnlock confirmUnlock(wifiLock);
            signalConnectionFailed();
        }
    }
}

/*
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

/*
 * A callback function to run whenever the list of wifi access points is 
 * updated.
 */
void LibNMInterface::apUpdateCallback(Array<WifiAccessPoint::Ptr> visibleAPs) 
{
    ScopedLock updateLock(wifiLock);
    if(this->visibleAPs != visibleAPs)
    {
        this->visibleAPs = visibleAPs;
        DBG("LibNMInterface::" << __func__ << ": found "
                << visibleAPs.size() << " AccessPoints");
    }
 }

/*
 * Registers updates to the wifi device when the NetworkManager device state
 * changes.
 */
void LibNMInterface::stateUpdateCallback(NMDeviceState newState,
        NMDeviceStateReason stateReason)
{
    ScopedLock updateLock(wifiLock);
    if(lastNMState == newState)//duplicate callback, ignore
    {
        return;
    }
    
    lastNMState = newState;
    DBG("LibNMInterface::" << __func__ << ":  changed to "
            << deviceStateString(newState));
    DBG("LibNMInterface::" << __func__ << ":  reason="
            << deviceStateReasonString(stateReason));
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
            //TODO: check for authorization here
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

/*
 * Notifies listeners when the active access point changes.
 */
void LibNMInterface::connectionUpdateCallback(WifiAccessPoint::Ptr connected)
{
    ScopedLock updateLock(wifiLock);
    if(connectedAP != connected)
    {
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
}


/*
 * called whenever the application window gains focus.
 */
void LibNMInterface::windowFocusGained()
{
    connectSignalHandlers();
    updateAllWifiData();
}

/*
 * called whenever the application window loses focus.
 */
void LibNMInterface::windowFocusLost()
{
    disconnectSignalHandlers();
    stopTimer();
}
