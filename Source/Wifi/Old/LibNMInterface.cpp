#include <map>
#include "Utils.h"
#include "LibNMInterface.h"
#include "JuceHeader.h"
#include "Config/MainFile.h"
#include "Config/MainKeys.h"
#include "LibNM/DBus/SavedConnectionLoader.h"
#if JUCE_DEBUG
#include "WifiDebugOutput.h"
#endif

LibNMInterface::LibNMInterface()
{
    using juce::String;
    using juce::Array;
    Config::MainFile config;
    String wifiIface = config.getConfigValue<String>
            (Config::MainKeys::wifiInterfaceKey);
    if(wifiIface.isNotEmpty())
    {
        wifiDevice = client.getWifiDeviceByIface(wifiIface.toRawUTF8());
    }
    else
    {
        Array<LibNM::DeviceWifi> devices = client.getWifiDevices();
        for(const LibNM::DeviceWifi& dev : devices)
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
juce::Array<WifiAccessPoint> LibNMInterface::getVisibleAPs()
{
    using namespace juce;
    Array<WifiAccessPoint> filteredAPs;
    if(!activeAP.isNull())
    {
        filteredAPs.add(activeAP);
    }
    Array<LibNM::SavedConnection> savedCons 
        = savedConnections.getWifiConnections();
    for(const LibNM::AccessPoint& ap : visibleAPs)
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
        juce::String psk)
{
    using namespace juce;
    const LibNM::AccessPoint& nmAP = toConnect.getNMAccessPoint();
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
        LibNM::Connection toActivate;
        //Copy the saved connection, delete the original, and re-create it with
        //new security info.
        if(toConnect.getSavedConnectionPath().isNotEmpty())
        {
            LibNM::SavedConnection saved = savedConnections.getConnection
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
 * Asynchronously scans for nearby wifi access points, updating the access point
 * list.
 */
void LibNMInterface::scanAccessPoints()
{
    wifiDevice.requestScan();
}


/*
 * Finds the current network state of an access point object.
 */
AccessPointState LibNMInterface::getAPState 
(const WifiAccessPoint& accessPoint)
{
    if(accessPoint.isNull())
    {
        return AccessPointState::nullAP;
    }
    if(activeConnection.isConnectedAccessPoint(accessPoint.getNMAccessPoint())
            || activeAP.sharesConnectionWith(accessPoint))
    {
        //This check will report a disconnecting AP slightly earlier than
        //waiting for the connection state to switch to 
        //NM_ACTIVE_CONNECTION_STATE_DEACTIVATING. This approach can't be used
        //when connecting, as the connecting AP might not have been saved yet.
        if(getWifiState() == WifiState::disconnecting)
        {
            return AccessPointState::disconnectingAP;
        }
        switch(activeConnection.getConnectionState())
        {
            case NM_ACTIVE_CONNECTION_STATE_ACTIVATING:
                return AccessPointState::connectingAP;
            case NM_ACTIVE_CONNECTION_STATE_ACTIVATED:
                return AccessPointState::connectedAP;
            case NM_ACTIVE_CONNECTION_STATE_DEACTIVATING:
                return AccessPointState::disconnectingAP;
            case NM_ACTIVE_CONNECTION_STATE_DEACTIVATED:
                return AccessPointState::disconnectedAP;
            case NM_ACTIVE_CONNECTION_STATE_UNKNOWN:
                return AccessPointState::missingAP;
        }
    }
    if(failedConnectionAPs.contains(accessPoint))
    {
        return AccessPointState::invalidSecurityAP;
    }
    if(visibleAPs.contains(accessPoint.getNMAccessPoint()))
    {
        return AccessPointState::disconnectedAP;
    }
    return AccessPointState::missingAP;
}     
    
/*
 * Finds the last time a connection was active using a specific access
 * point.
 */
juce::Time LibNMInterface::lastConnectionTime
(const WifiAccessPoint& accessPoint)
{
    using juce::Time;
    using LibNM::SavedConnection;
    Time connectionTime;
    juce::Array<SavedConnection> apConnections = savedConnections
            .findConnectionsForAP(accessPoint.getNMAccessPoint());
    for(SavedConnection& con : apConnections)
    {
        Time lastConnected = con.lastConnectionTime();
        if(lastConnected.toMilliseconds() > connectionTime.toMilliseconds())
        {
            connectionTime = lastConnected;
        }
    }
    return connectionTime;
}

/*
 * Finds and sets active and saved connection paths for a wifi access point.
 */
void LibNMInterface::setAccessPointPaths(WifiAccessPoint& accessPoint)
{
    using juce::Array;

    if(!accessPoint.isNull())
    {
        const char* path = "";
        Array<LibNM::ActiveConnection> active = client.getActiveConnections();
        for(const LibNM::ActiveConnection& con : active)
        {
            if(con.isConnectedAccessPoint(accessPoint.getNMAccessPoint()))
            {
                path = con.getAccessPointPath();
                break;
            }
        }
        accessPoint.setActiveConnectionPath(path);
        savedConnections.updateSavedConnections();
        Array<LibNM::SavedConnection> saved 
            = savedConnections.findConnectionsForAP
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
    lastNMState = wifiDevice.getState();
    savedConnections.updateSavedConnections();
    activeConnection = wifiDevice.getActiveConnection();
    const char* apPath = activeConnection.getAccessPointPath();
    if(apPath[0] != 0)
    {
        LibNM::AccessPoint activeNMAP = wifiDevice.getAccessPoint(apPath);
        if(!activeNMAP.isNull())
        {
            WifiAccessPoint newActive(activeNMAP);
            activeAP = newActive;
            setAccessPointPaths(activeAP);
        }
    }
    visibleAPs = wifiDevice.getAccessPoints();
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
void LibNMInterface::openingConnection(LibNM::ActiveConnection connection,
            bool isNew)
{
    using namespace juce;
    //wifiLock must not be acquired in the GLib thread!
    std::function<void()> asyncAction 
        = buildAsyncFunction(SharedResource::LockType::write,
    [this, connection, isNew]()
    {
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
    MessageManager::callAsync(asyncAction);
}


/*
 * Notify listeners that a connection attempt failed.
 */
void LibNMInterface::openingConnectionFailed(LibNM::ActiveConnection connection, 
        GError* error, bool isNew)
{   
    using namespace juce;
    //wifiLock must not be acquired in the GLib thread!
    std::function<void()> asyncAction 
        = buildAsyncFunction(SharedResource::LockType::write,
    [this, connection, error, isNew]()
    {
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
        signalConnectionFailed();
    });
    MessageManager::callAsync(asyncAction);
}

/*
 * Called whenever wireless is enabled or disabled, this method will update
 * the wifi state accordingly.
 */
void LibNMInterface::wirelessEnabledChange(bool enabled)
{  
    using namespace juce;
    //wifiLock must not be acquired in the GLib thread!
    std::function<void()> asyncAction 
        = buildAsyncFunction(SharedResource::LockType::write,
    [this, enabled]()
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
    MessageManager::callAsync(asyncAction);
}


/*
 * This method will be called whenever the wifi device state changes.
 */
void LibNMInterface::stateChanged(NMDeviceState newState,
        NMDeviceState oldState, NMDeviceStateReason reason)
{
    using namespace juce;
    //wifiLock must not be acquired in the GLib thread!
    std::function<void()> asyncAction 
        = buildAsyncFunction(SharedResource::LockType::write,
    [this, newState, oldState, reason]()
    {
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
		
		    //Extend the connection timeout timer when activation state changes.
                stopTimer();
                startTimer(wifiConnectionTimeout);
                break;
            case NM_DEVICE_STATE_DISCONNECTED:
            {
		        stopTimer();
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
                        LibNM::SavedConnection toDelete
                                (activeAP.getSavedConnectionPath().toRawUTF8());
                        toDelete.deleteConnection();
                        activeConnection = LibNM::ActiveConnection();
                        activeAP = WifiAccessPoint();
                        newConnectionAP = WifiAccessPoint();
                    }
                    signalConnectionFailed();
                    return;
                }
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
                    signalWifiDisconnected();
		        }
            }
        }
    });
    MessageManager::callAsync(asyncAction);
}

/*
 * This method will be called whenever the wifi device detects a new
 * access point.
 */
void LibNMInterface::accessPointAdded(LibNM::AccessPoint addedAP)
{    
    using namespace juce;
    //wifiLock must not be acquired in the GLib thread!
    std::function<void()> asyncAction 
        = buildAsyncFunction(SharedResource::LockType::write,
    [this, addedAP]()
    {
        if(!addedAP.isNull())
        {
            WifiAccessPoint newAP(addedAP);
            visibleAPs.add(addedAP);
            setAccessPointPaths(newAP);
            signalAPAdded(newAP);
        }
    });
    MessageManager::callAsync(asyncAction);
}

/*
 * This method will be called whenever the wifi device no longer detects
 * a wifi access point.
 */
void LibNMInterface::accessPointRemoved(LibNM::AccessPoint removedAP)
{    
    using namespace juce;
    jassert(!removedAP.isNull());
    //wifiLock must not be acquired in the GLib thread!
    std::function<void()> asyncAction
        = buildAsyncFunction(SharedResource::LockType::write,
    [this, removedAP]()
    {
        // If the access point is now null, all references to the NMAP
        // are now clear, and it shouldn't be necessary to call
        // signalAPRemoved.
        if(!removedAP.isNull())
        {
            visibleAPs.removeAllInstancesOf(removedAP);
            WifiAccessPoint missingAP(removedAP);
            if(!missingAP.isNull())
            {
                signalAPRemoved(missingAP);
            }
        }
    });
    MessageManager::callAsync(asyncAction);
}

/*
 * Notifies listeners whenever the device's active connection
 * changes.
 */
void LibNMInterface::activeConnectionChanged(LibNM::ActiveConnection active)
{
    using namespace juce;
    //wifiLock must not be acquired in the GLib thread!
    std::function<void()> asyncAction 
        = buildAsyncFunction(SharedResource::LockType::write,
    [this, active]()
    {
        DBG("LibNMInterface::activeConnectionChanged" 
                << ": new active connection with state "
                << activeConnectionStateString(active.getConnectionState()));
        if(active != activeConnection)
        {
            activeConnection = active;
            if (active.isNull())
            {
                activeAP = WifiAccessPoint();
                signalWifiDisconnected();
                return;
            }
            else
            {
                LibNM::AccessPoint nmAP = wifiDevice.getAccessPoint
                        (activeConnection.getAccessPointPath());
                activeAP = WifiAccessPoint(nmAP);
                jassert(!activeAP.isNull());
                activeAP.setActiveConnectionPath
                        (activeConnection.getAccessPointPath());
                Array<LibNM::SavedConnection> saved 
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
                if(active.getConnectionState() 
                        == NM_ACTIVE_CONNECTION_STATE_ACTIVATED)
		{
                    signalWifiConnected();
		}
		else 
		{
                    signalWifiConnecting();
		}
                return;
            }
        }
    });
    MessageManager::callAsync(asyncAction);
}
