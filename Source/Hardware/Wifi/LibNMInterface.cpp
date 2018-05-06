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
        if(packagedAP.getSSID().isNotEmpty() 
                && !filteredAPs.contains(packagedAP))
        {
            for(const SavedConnection& con : savedCons)
            {
                if(ap.isValidConnection(con.getNMConnection()))
                {
                    packagedAP.setConnectionPath(con.getPath());
                    break;
                }
            }
            if(!packagedAP.isNull())
            {
                filteredAPs.add(packagedAP);
            }
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
    //search for active connections
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
    if(visibleAPs.contains(accessPoint.getNMAccessPoint()))
    {
        return WifiStateManager::disconnectedAP;
    }
    return WifiStateManager::missingAP;
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
    const char* path = activeConnection.getAccessPointPath();
    activeAP = wifiDevice.getAccessPoint(path);
    activeAP.setConnectionPath(path);
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
                DBG("LibNMInterface::" << __func__ 
                        << ": unknown connection state");
                break;
            case NM_ACTIVE_CONNECTION_STATE_ACTIVATING:
            case NM_ACTIVE_CONNECTION_STATE_ACTIVATED:
            {
                DBG("LibNMInterface::" << __func__ 
                        << ": setting new activating/activated connection");
                if(activeConnection != connection)
		{
		    activeConnection = connection;
		}
                const char* path = connection.getAccessPointPath();
                activeAP = WifiAccessPoint(wifiDevice.getAccessPoint(path));
                activeAP.setConnectionPath(path);
                break;
            }
            case NM_ACTIVE_CONNECTION_STATE_DEACTIVATING:
            case NM_ACTIVE_CONNECTION_STATE_DEACTIVATED:
                DBG("LibNMInterface::" << __func__ << ": No error, but new "
                        << "connection already being closed.");
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
        g_error_free(error);
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
        DBG("LibNMInterface::" << __func__ << ":  changed to "
                << deviceStateString(newState));
        DBG("LibNMInterface::" << __func__ << ":  reason="
                << deviceStateReasonString(reason));
        switch (newState)
        {
            case NM_DEVICE_STATE_ACTIVATED:
            {
                DBG("LibNMInterface::" << __func__ 
                        << ": new connection activated, send connection signal");
		stopTimer();
		ScopedUnlock unlockForUpdate(wifiLock);
		signalWifiConnected(activeAP);
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
        //TODO: add AP change notification for listeners.
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
        //TODO: add AP change notification for listeners.
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
        DBG("LibNMInterface::" << __func__ 
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
                ScopedUnlock notifyUnlock(wifiLock);
                if(active.getConnectionState() 
                        == NM_ACTIVE_CONNECTION_STATE_ACTIVATED)
		{
                    signalWifiConnected(activeAP);
		}
		else 
		{
                    signalWifiConnecting();
		}
            }
        }
    });
}
