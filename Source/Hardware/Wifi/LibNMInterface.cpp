#include "LibNMInterface.h"
#include <map>
#include "JuceHeader.h"
#include "MainConfigFile.h"
#include "SavedConnections.h"
#if JUCE_DEBUG
#include "WifiDebugOutput.h"
#endif

LibNMInterface::LibNMInterface(CriticalSection& wifiLock) :
NetworkInterface(wifiLock),
wifiLock(wifiLock)
{
    MainConfigFile config;
    String wifiIface = config.getConfigValue<String>
            (MainConfigFile::wifiInterfaceKey);
    if(wifiIface.isNotEmpty())
    {
        wifiDevice = client.getWifiDeviceByIface(wifiIface.toRawUTF8());
    }
    else
    {
        Array<NMPPDeviceWifi> devices = client.getWifiDevices();
        for(const NMPPDeviceWifi& dev : devices)
        {
            if(dev.isManaged())
            {
                wifiDevice = dev;
                break;
            }
        }
    }
    clientListener = new ClientListener(*this, client);
    deviceListener = new DeviceListener(*this, wifiDevice);
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
    //search for active connections
    const NMPPAccessPoint& nmAP = toConnect.getNMAccessPoint();
    if(nmAP.isNull())
    {
        DBG("LibNMInterface::" << __func__ 
                << ": attempted to connect with null AP!");
    }
    else if(activatingConnection.isConnectedAccessPoint(nmAP))
    {
        DBG("LibNMInterface::" << __func__ << ": access point " 
                << toConnect.getSSID() << " is already connecting!");
    }
    else if(activeConnection.isConnectedAccessPoint(nmAP))
    {
        DBG("LibNMInterface::" << __func__ << ": access point " 
                << toConnect.getSSID() << " is already connected!");
    }
    else
    {
        NMPPConnection toActivate = wifiDevice.getAvailableConnection(nmAP);
        if(!toActivate.isNull())
        {
            DBG("LibNMInterface::" << __func__ << ": Connecting to AP "
                    << toConnect.getSSID() << " with existing connection "
                    << toActivate.getID());
        }
        else
        {
            Array<SavedConnection> matchingSaved 
                    = savedConnections.findConnectionsForAP(nmAP);
            if(!matchingSaved.isEmpty())
            {
                toActivate = matchingSaved[0].getNMConnection();
                DBG("LibNMInterface::" << __func__ << ": Found "
                        << matchingSaved.size() 
                        << " matching saved connection(s)");
                DBG("LibNMInterface::" << __func__ << ":  connecting to"
                        << toConnect.getSSID() << " with saved connection "
                        << toActivate.getID());
            }
            else
            {
                toActivate = toConnect.createConnection(psk);
            }
        }
        if(!toActivate.isNull())
        {
            client.activateConnection(toActivate, wifiDevice, nmAP, this);
        }
        else
        {
            DBG("LibNMInterface::" << __func__ 
                    << ": failed to find or create a connection for AP "
                    <<toConnect.getSSID());
        }
    }
    
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
void LibNMInterface::openingConnection(NMPPActiveConnection connection,
            bool isNew)
{
    ScopedLock updateLock(wifiLock);
    switch(connection.getConnectionState())
    {
        case NM_ACTIVE_CONNECTION_STATE_UNKNOWN:
            DBG("LibNMInterface::" << __func__ << ": unknown connection state");
            break;
        case NM_ACTIVE_CONNECTION_STATE_ACTIVATING:
            DBG("LibNMInterface::" << __func__ 
                    << ": setting new activating connection");
            activatingConnection = connection;
            connectingAP = wifiDevice.getAccessPoint
                    (connection.getAccessPointPath());
            break;
        case NM_ACTIVE_CONNECTION_STATE_ACTIVATED:
            DBG("LibNMInterface::" << __func__ 
                    << ": new connection already activated, let "
                    << "activeConnectionChanged() handle it.");
            break;
        case NM_ACTIVE_CONNECTION_STATE_DEACTIVATING:
        case NM_ACTIVE_CONNECTION_STATE_DEACTIVATED:
            DBG("LibNMInterface::" << __func__ 
                    << ": No error, but new connection already being closed.");
    }
    
}


/*
 * Notify listeners that a connection attempt failed.
 */
void LibNMInterface::openingConnectionFailed(NMPPActiveConnection connection, 
        GError* error, bool isNew)
{   
    ScopedLock updateLock(wifiLock);
    DBG("LibNMInterface::" << __func__ << ": Error "
            << error->code << ":" << error->message);
    //TODO: is it necessary to make sure this isn't the active/activating
    //connection or access point?  Probably not, but run tests to be sure.
    
    //TODO: properly detect invalid security errors, then:
    /*
     if(isNew)
     {
         SavedConnection failed(connection.getPath());
         failed.deleteConnection();
     }
     else
     {
        //ask before deleting non-new connection.
     }
     ScopedUnlock confirmUnlock(wifiLock);
     signalConnectionFailed();
     */
}

/*
 * Called whenever wireless is enabled or disabled, this method will update
 * the wifi state accordingly.
 */
void LibNMInterface::wirelessEnabledChange(bool enabled)
{
    if (enabled)
    {
        wifiDevice.requestScan();
        signalWifiEnabled();
    }
    else
    {
        signalWifiDisabled();
    }
}


/*
 * This method will be called whenever the wifi device state changes.
 */
void LibNMInterface::stateChanged(NMDeviceState newState,
        NMDeviceState oldState, NMDeviceStateReason reason)
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
            << deviceStateReasonString(reason));
    switch (newState)
    {
        case NM_DEVICE_STATE_ACTIVATED:        
            DBG("LibNMInterface::" << __func__ 
                    << ": new connection activated, let "
                    << "activeConnectionChanged() handle it.");
            break;
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
        case NM_DEVICE_STATE_NEED_AUTH:
            /* No state change for now, wait for connection to complete/fail */
            //ensure the pending connection is registered
            if (activatingConnection.isNull() || connectingAP.isNull())
            {
                activatingConnection = client.getActivatingConnection();
                connectingAP = wifiDevice.getAccessPoint
                        (activatingConnection.getAccessPointPath());
                if(connectingAP.isNull())
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
            stopTimer();
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
 * This method will be called whenever the wifi device detects a new
 * access point.
 */
void LibNMInterface::accessPointAdded(NMPPAccessPoint addedAP)
{
    ScopedLock updateLock(wifiLock);
    visibleAPs.add(addedAP);
    //TODO: add AP change notification for listeners.
}

/*
 * This method will be called whenever the wifi device no longer detects
 * a wifi access point.
 */
void LibNMInterface::accessPointRemoved(NMPPAccessPoint removedAP)
{
    ScopedLock updateLock(wifiLock);
    visibleAPs.removeAllInstancesOf(removedAP);
    //TODO: add AP change notification for listeners.
}

/*
 * Notifies listeners whenever the device's active connection
 * changes.
 */
void LibNMInterface::activeConnectionChanged(NMPPActiveConnection active)
{
    ScopedLock updateLock(wifiLock);
    DBG("LibNMInterface::" << __func__ << ": new active connection with state "
            << activeConnectionStateString(active.getConnectionState()));
    if(active != activeConnection)
    {
        jassert(active.isNull() 
                || (active == activatingConnection 
                && connectingAP == wifiDevice.getAccessPoint
                (active.getAccessPointPath())));
        activeConnection = active;
        activatingConnection = NMPPActiveConnection();
        if (active.isNull())
        {
            connectedAP = NMPPAccessPoint();
            connectingAP = NMPPAccessPoint();
            ScopedUnlock notifyUnlock(wifiLock);
            signalWifiDisconnected();
        }
        else
        {
            connectedAP = connectingAP;
            connectingAP = NMPPAccessPoint();
            jassert(active.getConnectionState() 
                    == NM_ACTIVE_CONNECTION_STATE_ACTIVATED);
            ScopedUnlock notifyUnlock(wifiLock);
            signalWifiConnected(connectedAP);
        }
    }
}
