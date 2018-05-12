#include <map>
#include "Utils.h"
#include "LibNMInterface.h"
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
    return !wifiDevice.isNull();
}

/*
 * Check the NMDevice state to see if wifi is enabled.
 */
bool LibNMInterface::isWifiEnabled()
{
    return client.wirelessEnabled();
}

/**
 * Check the NMDevice state to see if wifi is connecting.
 */
bool LibNMInterface::isWifiConnecting()
{
    return activeConnection.getConnectionState()
            == NM_ACTIVE_CONNECTION_STATE_ACTIVATING;
}

/*
 * Check the NMDevice state to see if wifi is connected.
 */
bool LibNMInterface::isWifiConnected()
{
    return activeConnection.getConnectionState() 
            == NM_ACTIVE_CONNECTION_STATE_ACTIVATED;
}


/*
 * Requests information on the active access point from the NMDevice.
 */
WifiAccessPoint LibNMInterface::getActiveAP()
{
    switch(activeConnection.getConnectionState())
    {
        case NM_ACTIVE_CONNECTION_STATE_ACTIVATING:
        case NM_ACTIVE_CONNECTION_STATE_ACTIVATED:
            return activeAP;
        default:
            return WifiAccessPoint();
    }
}

/*
 * Request information on all wifi access points detected by the NMDevice.
 */
Array<WifiAccessPoint> LibNMInterface::getVisibleAPs()
{
    Array<WifiAccessPoint> filteredAPs;
    if(!activeAP.isNull())
    {
        filteredAPs.add(activeAP);
    }
    Array<SavedConnection> savedCons = savedConnections.getWifiConnections();
    for(const NMPPAccessPoint& ap : visibleAPs)
    {
        WifiAccessPoint packagedAP(ap);
        if(!packagedAP.isNull() && packagedAP.getSSID().isNotEmpty() 
                && !filteredAPs.contains(packagedAP))
        {
            setAccessPointPaths(packagedAP);
            filteredAPs.add(packagedAP);
        }
    }
    return filteredAPs;
}

/*
 * Begin opening a connection to a wifi access point.
 */
void LibNMInterface::connectToAccessPoint(const WifiAccessPoint& toConnect,
        String psk)
{
    const NMPPAccessPoint& nmAP = toConnect.getNMAccessPoint();
    if(nmAP.isNull())
    {
        DBG("LibNMInterface::" << __func__ 
                << ": attempted to connect with null AP!");
    }
    else if(activeConnection.isConnectedAccessPoint(nmAP))
    {
        DBG("LibNMInterface::" << __func__ << ": access point " 
                << toConnect.getSSID() << " is already active!");
    }
    else
    {
        NMPPConnection toActivate;
        //Copy the saved connection, delete the original, and re-create it with
        //new security info.
        if(toConnect.getSavedConnectionPath().isNotEmpty())
        {
            SavedConnection saved = savedConnections.getConnection
                    (toConnect.getSavedConnectionPath());
            if(!saved.isNull())
            {
                toActivate = saved.getNMConnection();
            }          
        }
        if(toActivate.isNull())
        {
            toActivate = wifiDevice.getAvailableConnection(nmAP);
            if(toActivate.isNull())
            {
                toActivate = toConnect.createConnection(psk);
                newConnectionAP = toConnect;
                DBG("LibNMInterface::" << __func__ 
                        << ": Creating new connection for AP "
                        << toConnect.getSSID());
            }
            else
            {            
                if(failedConnectionAPs.contains(toConnect))
                {
                    DBG("LibNMInterface::" << __func__ 
                            << ": re-creating failed connection with new "
                            << "security settings.");
                    toConnect.setConnectionSecurity(toActivate, psk);
                    //Remove the basic connection settings so NetworkManager
                    //can re-generate them for the new connection.
                    toActivate.removeSetting(NM_TYPE_SETTING_CONNECTION);
                }
                else
                {
                    DBG("LibNMInterface::" << __func__ << ": Connecting to AP "
                            << toConnect.getSSID() 
                            << " with existing connection " 
                            << toActivate.getID());
                }
            }
        }
        if(!toActivate.isNull())
        {
            client.activateConnection(toActivate, wifiDevice, nmAP, this,
                    !failedConnectionAPs.contains(toConnect));
        }
        else
        {
            DBG("LibNMInterface::" << __func__ 
                    << ": failed to find or create a connection for AP "
                    << toConnect.getSSID());
        }
    }
    
}

/*
 * Finds the current network state of an access point object.
 */
WifiStateManager::AccessPointState LibNMInterface::getAPState 
(const WifiAccessPoint& accessPoint)
{
    if(accessPoint.isNull())
    {
        return WifiStateManager::nullAP;
    }
    if(activeConnection.isConnectedAccessPoint(accessPoint.getNMAccessPoint())
            || activeAP.sharesConnectionWith(accessPoint))
    {
        //This check will report a disconnecting AP slightly earlier than
        //waiting for the connection state to switch to 
        //NM_ACTIVE_CONNECTION_STATE_DEACTIVATING. This approach can't be used
        //when connecting, as the connecting AP might not have been saved yet.
        if(getWifiState() == WifiStateManager::disconnecting)
        {
            return WifiStateManager::disconnectingAP;
        }
        switch(activeConnection.getConnectionState())
        {
            case NM_ACTIVE_CONNECTION_STATE_ACTIVATING:
                return WifiStateManager::connectingAP;
            case NM_ACTIVE_CONNECTION_STATE_ACTIVATED:
                return WifiStateManager::connectedAP;
            case NM_ACTIVE_CONNECTION_STATE_DEACTIVATING:
                return WifiStateManager::disconnectingAP;
            case NM_ACTIVE_CONNECTION_STATE_DEACTIVATED:
                return WifiStateManager::disconnectedAP;
        }
    }
    if(failedConnectionAPs.contains(accessPoint))
    {
        return WifiStateManager::invalidSecurityAP;
    }
    if(visibleAPs.contains(accessPoint.getNMAccessPoint()))
    {
        return WifiStateManager::disconnectedAP;
    }
    return WifiStateManager::missingAP;
}     

/*
 * Finds and sets active and saved connection paths for a wifi access point.
 */
void LibNMInterface::setAccessPointPaths(WifiAccessPoint& accessPoint)
{
    if(!accessPoint.isNull())
    {
        const char* path = "";
        Array<NMPPActiveConnection> active = client.getActiveConnections();
        for(const NMPPActiveConnection& con : active)
        {
            if(con.isConnectedAccessPoint(accessPoint.getNMAccessPoint()))
            {
                path = con.getAccessPointPath();
                break;
            }
        }
        accessPoint.setActiveConnectionPath(path);
        savedConnections.updateSavedConnections();
        Array<SavedConnection> saved = savedConnections.findConnectionsForAP
                (accessPoint.getNMAccessPoint());
        if(!saved.isEmpty())
        {
            accessPoint.setSavedConnectionPath(saved[0].getPath());
        }
        else
        {
            accessPoint.setSavedConnectionPath("");
        }
    }
}

/*
 * Checks the wifi device list, connection state, and active and pending 
 * connections.
 */
void LibNMInterface::updateAllWifiData()
{
    DBG("LibNMInterface::updateAllWifiData: reloading data");
    ScopedLock updateLock(wifiLock);
    lastNMState = wifiDevice.getState();
    savedConnections.updateSavedConnections();
    activeConnection = wifiDevice.getActiveConnection();
    activeAP = WifiAccessPoint(wifiDevice.getAccessPoint
            (activeConnection.getAccessPointPath()));
    setAccessPointPaths(activeAP);
    visibleAPs = wifiDevice.getAccessPoints();
    ScopedUnlock confirmUnlock(wifiLock);
    confirmWifiState();
}

/*
 * Close the currently active/activating connection, if one exists.
 */
void LibNMInterface::disconnect()
{
    wifiDevice.disconnect();
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
    //wifiLock must not be acquired in the GLib thread!
    MessageManager::callAsync([this, connection, isNew]()
    {
        ScopedLock updateLock(wifiLock);
        switch(connection.getConnectionState())
        {
            case NM_ACTIVE_CONNECTION_STATE_UNKNOWN:
                DBG("LibNMInterface::openingConnection"
                        << ": unknown connection state");
                break;
            case NM_ACTIVE_CONNECTION_STATE_ACTIVATING:
            case NM_ACTIVE_CONNECTION_STATE_ACTIVATED:
            {
                DBG("LibNMInterface::openingConnection"
                        << ": setting new activating/activated connection");
                if(activeConnection != connection)
		{
		    activeConnection = connection;
		}
                const char* path = connection.getAccessPointPath();
                activeAP = WifiAccessPoint(wifiDevice.getAccessPoint(path));
                setAccessPointPaths(activeAP);
                break;
            }
            case NM_ACTIVE_CONNECTION_STATE_DEACTIVATING:
            case NM_ACTIVE_CONNECTION_STATE_DEACTIVATED:
                DBG("LibNMInterface::openingConnection"
                        << ": No GError, but the new connection is already "
                        << "being closed.");
        }
    });
}


/*
 * Notify listeners that a connection attempt failed.
 */
void LibNMInterface::openingConnectionFailed(NMPPActiveConnection connection, 
        GError* error, bool isNew)
{   
    //wifiLock must not be acquired in the GLib thread!
    MessageManager::callAsync([this, connection, error, isNew]()
    {
        ScopedLock updateLock(wifiLock);
        DBG("LibNMInterface::openingConnectionFailed" << ": Error "
                << error->code << ":" << error->message);
        //TODO: is it necessary to make sure activeConnection or activeAP
        //aren't currently set to this failed connection?  Probably not, but run
        //tests to be sure.

        //TODO: delete invalid new connections?
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
         */
        g_error_free(error);
        ScopedUnlock confirmUnlock(wifiLock);
        signalConnectionFailed();
    });
}

/*
 * Called whenever wireless is enabled or disabled, this method will update
 * the wifi state accordingly.
 */
void LibNMInterface::wirelessEnabledChange(bool enabled)
{  
    //wifiLock must not be acquired in the GLib thread!
    MessageManager::callAsync([this, enabled]()
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
    });
}


/*
 * This method will be called whenever the wifi device state changes.
 */
void LibNMInterface::stateChanged(NMDeviceState newState,
        NMDeviceState oldState, NMDeviceStateReason reason)
{
    //wifiLock must not be acquired in the GLib thread!
    MessageManager::callAsync([this, newState, oldState, reason]()
    {
        ScopedLock updateLock(wifiLock);
        if(lastNMState == newState)//duplicate callback, ignore
        {
            return;
        }
        lastNMState = newState;
        DBG("LibNMInterface::stateChanged" << ":  changed to "
                << deviceStateString(newState));
        DBG("LibNMInterface::stateChanged" << ":  reason="
                << deviceStateReasonString(reason));
        switch (newState)
        {
            case NM_DEVICE_STATE_ACTIVATED:
            {
                DBG("LibNMInterface::stateChanged: new connection activated, "
                        << "send connection signal");
		stopTimer();
                failedConnectionAPs.removeAllInstancesOf(activeAP);
		ScopedUnlock unlockForUpdate(wifiLock);
		signalWifiConnected();
                break;
            }
            case NM_DEVICE_STATE_PREPARE:
            case NM_DEVICE_STATE_CONFIG:
            case NM_DEVICE_STATE_IP_CONFIG:
            case NM_DEVICE_STATE_IP_CHECK:
            case NM_DEVICE_STATE_SECONDARIES:
            case NM_DEVICE_STATE_NEED_AUTH:
                //activeConnectionChanged() or openingConnection() will register
		//the new connection and send the connecting signal. 
		
		//Extend the connection timeout timer when activation state
		//changes.
                stopTimer();
                startTimer(wifiConnectionTimeout);
                break;
            case NM_DEVICE_STATE_DISCONNECTED:
            {
		stopTimer();
                ScopedUnlock unlockForUpdate(wifiLock);
                if(oldState == NM_DEVICE_STATE_DEACTIVATING)
                {
                    signalWifiDisconnected();
                }
                else if(!activeAP.isNull())
                {
                    activeAP = WifiAccessPoint();
                    signalConnectionFailed();
                }
                break;
            }
            case NM_DEVICE_STATE_DEACTIVATING:
            case NM_DEVICE_STATE_FAILED:
            {
                stopTimer();
                if(reason == NM_DEVICE_STATE_REASON_NO_SECRETS)
                {
                    failedConnectionAPs.add(activeAP);
                    if(newConnectionAP == activeAP)
                    {
                        DBG("LibNMInterface::stateChanged"
                                << ": Deleting failed new connection.");
                        setAccessPointPaths(activeAP);
                        SavedConnection toDelete
                                (activeAP.getSavedConnectionPath().toRawUTF8());
                        toDelete.deleteConnection();
                        activeConnection = NMPPActiveConnection();
                        activeAP = WifiAccessPoint();
                        newConnectionAP = WifiAccessPoint();
                    }
                    ScopedUnlock unlockForUpdate(wifiLock);
                    signalConnectionFailed();
                    return;
                }
                ScopedUnlock unlockForUpdate(wifiLock);
                signalWifiDisconnected();
                break;
            }
            case NM_DEVICE_STATE_UNKNOWN:
            case NM_DEVICE_STATE_UNMANAGED:
            case NM_DEVICE_STATE_UNAVAILABLE:
            default:
            {
                DBG("LibNMInterface::stateChanged"
                        << ": wlan0 device entered unmanaged state: "
                        << deviceStateString(newState));
		if(!activeAP.isNull())
	    	{
                    ScopedUnlock unlockForUpdate(wifiLock);
                    signalWifiDisconnected();
		}
            }
        }
    });
}

/*
 * This method will be called whenever the wifi device detects a new
 * access point.
 */
void LibNMInterface::accessPointAdded(NMPPAccessPoint addedAP)
{    
    //wifiLock must not be acquired in the GLib thread!
    MessageManager::callAsync([this, addedAP]()
    {
        ScopedLock updateLock(wifiLock);
        visibleAPs.add(addedAP);
        
        ScopedUnlock notifyUnlock(wifiLock);
        WifiAccessPoint newAP(addedAP);
        setAccessPointPaths(newAP);
        signalAPAdded(newAP);
    });
}

/*
 * This method will be called whenever the wifi device no longer detects
 * a wifi access point.
 */
void LibNMInterface::accessPointRemoved(NMPPAccessPoint removedAP)
{    
    //wifiLock must not be acquired in the GLib thread!
    MessageManager::callAsync([this, removedAP]()
    {
        ScopedLock updateLock(wifiLock);
        visibleAPs.removeAllInstancesOf(removedAP);
             
        ScopedUnlock notifyUnlock(wifiLock);
        WifiAccessPoint missingAP(removedAP);
        signalAPRemoved(missingAP);
    });
}

/*
 * Notifies listeners whenever the device's active connection
 * changes.
 */
void LibNMInterface::activeConnectionChanged(NMPPActiveConnection active)
{
    //wifiLock must not be acquired in the GLib thread!
    MessageManager::callAsync([this, active]()
    {
        ScopedLock updateLock(wifiLock);
        DBG("LibNMInterface::activeConnectionChanged" 
                << ": new active connection with state "
                << activeConnectionStateString(active.getConnectionState()));
        if(active != activeConnection)
        {
            activeConnection = active;
            if (active.isNull())
            {
                activeAP = WifiAccessPoint();
                ScopedUnlock notifyUnlock(wifiLock);
                signalWifiDisconnected();
            }
            else
            {
                NMPPAccessPoint nmAP = wifiDevice.getAccessPoint
                        (activeConnection.getAccessPointPath());
                activeAP = WifiAccessPoint(nmAP);
                jassert(!activeAP.isNull());
                activeAP.setActiveConnectionPath
                        (activeConnection.getAccessPointPath());
                Array<SavedConnection> saved 
                        = savedConnections.findConnectionsForAP
                        (activeAP.getNMAccessPoint());
                if(!saved.isEmpty())
                {
                    activeAP.setSavedConnectionPath(saved[0].getPath());
                    if(saved.size() > 1)
                    {
                        DBG("LibNMInterface::activeConnectionChanged: "
                                << "Multiple saved connections found!");
                        //TODO: figure out how to make sure networkManager picks
                        //the right one
                    }
                }
                ScopedUnlock notifyUnlock(wifiLock);
                if(active.getConnectionState() 
                        == NM_ACTIVE_CONNECTION_STATE_ACTIVATED)
		{
                    signalWifiConnected();
		}
		else 
		{
                    signalWifiConnecting();
		}
            }
        }
    });
}
