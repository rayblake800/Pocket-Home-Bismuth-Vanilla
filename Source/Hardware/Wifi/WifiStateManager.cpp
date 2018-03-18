#include "WifiStateManager.h"

WifiStateManager::WifiStateManager() { }

/**
 * On destruction, the WifiStateManager removes all references
 * to itself from its Listener objects, and destroys its
 * WifiNetworkInterface.
 * This method acquires the WifiStateManager's stateLock.
 */
WifiStateManager::~WifiStateManager()
{
    ScopedLock wifiStateLock(stateLock);
    for (Listener* listener : wifiListeners)
    {
        listener->stateManager = nullptr;
    }
    if (networkInterface != nullptr)
    {
        networkInterface->stateManager = nullptr;
    }
}

/**
 * Assigns a WifiNetworkInterface to this WifiStateManager.
 * The state manager will take ownership of the network
 * interface, deleting it on destruction or when a new
 * interface is provided through setNetworkInterface.
 * This method acquires the WifiStateManager's stateLock.
 *
 * @param interface takes responsibility for communicating
 * with the Wifi device.
 */
void WifiStateManager::setNetworkInterface(WifiNetworkInterface* interface)
{
    ScopedLock wifiStateLock(stateLock);
    if (networkInterface != nullptr && networkInterface->stateManager == this)
    {
        networkInterface->stateManager = nullptr;
    }
    networkInterface = interface;
}

/**
 * @return the current state of the wifi device, or
 * missingNetworkInterface if the WifiNetworkInterface
 * hasn`t been set.
 * This method acquires the WifiStateManager's stateLock.
 */
WifiState WifiStateManager::getWifiState()
{
    ScopedLock wifiStateLock(stateLock);
    return wifiState;
}

/**
 * Add a listener to the list of objects receiving updates
 * whenever Wifi state changes.
 * This method acquires the WifiStateManager's stateLock.
 *
 * If the listener had already been added to another
 * WifiStateManager, it will be removed from that state manager
 * before it's added to this one.  In debug builds this will
 * also print a warning and fail an assertion, as there really
 * shouldn`t be more than one WifiStateManager.
 *
 * @param listener will be subscribed to state updates
 */
void WifiStateManager::addListener(Listener* listener)
{
    ScopedLock wifiStateLock(stateLock);
    if (listener->stateManager != nullptr)
    {
        if (listener->stateManager == this)
        {
            jassert(listeners.contains(listener));
            DBG("WifiStateManager::" << __func__
                    << ": this listener has already been added.");
            return;
        }
        else
        {
            jassert(false);
            DBG("WifiStateManager::" << __func__
                    << ": this listener has another state manager!");
            listener->stateManager->removeListener(listener);
        }
    }
    listener->stateManager = this;
    listeners.add(listener);
}

/**
 * Search the list of registered listeners for a particular one,
 * and remove it if it's found.
 *  This method acquires the WifiStateManager's stateLock.
 * @param listener will be unsubscribed from state updates.
 */
void WifiStateManager::removeListener(Listener* listener)
{
    ScopedLock wifiStateLock(stateLock);
    if (listener->stateManager == this)
    {
        listener->stateManager = nullptr;
    }
    listeners.removeAllInstancesOf(listener);
}

/**
 * @return the current connected Wifi access point, or
 * a null WifiAccessPoint if there is no connection.
 */
WifiAccessPoint WifiStateManager::getConnectedAP()
{
    if (networkInterface == nullptr)
    {
        DBG("WifiStateManager::" << __func__ << ": Can't get connected"
                << " access point, no network interface assigned.");
        return WifiAccessPoint();
    }
    return networkInterface->getConnectedAP();
}

    /**
     * @return the current connecting WifiAccessPoint if one
     * is found, or a null WifiAccessPoint if not connecting.
     */
WifiAccessPoint WifiStateManager::getConnectingAP()
{
    if (networkInterface == nullptr)
    {
        DBG("WifiStateManager::" << __func__ << ": Can't get connecting"
                << " access point, no network interface assigned.");
        return WifiAccessPoint();
    }
    return networkInterface->getConnectingAP();
}

/**
 * @return all wifi access points currently detected.
 * If wifi is disabled, this will return an empty list.
 */
Array<WifiAccessPoint> WifiStateManager::getVisibleAPs()
{
    if (networkInterface == nullptr)
    {
        DBG("WifiStateManager::" << __func__ << ": Can't get visible "
                << "access points, no network interface assigned.");
        return {};
    }
    return networkInterface->getConnectedAP();
}

/**
 * Attempt to open a connection to a wifi access point.
 * This will fail if wifi is disabled, the access point is
 * invalid, or the psk is wrong.
 * This method acquires the WifiStateManager's stateLock.
 *
 * @param toConnect should be a valid nearby access point
 *
 * @param psk wifi key for toConnect, or the empty string
 * if toConnect isn't secured.
 */
void WifiStateManager::connectToAccessPoint(WifiAccessPoint toConnect,
        String psk = String())
{
    ScopedLock wifiStateLock(stateLock);
    if (networkInterface == nullptr)
    {
        setWifiState(noStateManager);
    }
    if (cancelledConnection == toConnect)
    {

        DBG("WifiStateManager::" << __func__ << ": Connecting to "
                << toConnect.getSSID << " was recently canceled, attempting to"
                << " resume connection attempt");
        cancelledConnection = WifiAccessPoint();
    }
    switch (wifiState)
    {
            //start connecting:
        case enabled:
            DBG("WifiStateManager::" << __func__ << ": Connecting to access "
                    << "point " << toConnect.getSSID());
            pendingConnection.ap = toConnect;
            pendingConnection.psk = psk;
            setWifiState(connecting);
            networkInterface->connectToAccessPoint(toConnect, psk);
            break;

            //start switching access point:
        case connected:
            if (networkInterface->getConnectedAP() == toConnect)
            {

                DBG("WifiStateManager::" << __func__ << ": Trying to open "
                        << " connection to access point " << toConnect.getSSID()
                        << "but that access point is already connected");
                return;
            }
            networkInterface->disconnect();
            startTimer(wifiConnectionTimeout);
        case connecting:
        case switchingConnection:
            if (pendingConnection.ap == toConnect)
            {
                DBG("WifiStateManager::" << __func__ << ": Trying to open "
                        << " connection to access point " << toConnect.getSSID()
                        << "but that access point is already connecting");
                return;
            }
            DBG("WifiStateManager::" << __func__ << ": Switching to access "
                    << "point " << toConnect.getSSID());
        case disconnecting:
            pendingConnection.ap = toConnect;
            pendingConnection.psk = psk;
            setWifiState(switchingConnection);
            break;

            //can't connect
        case missingNetworkInterface:
            DBG("WifiStateManager::" << __func__ << ": Can't connect to "
                    << toConnect.getSSID << " no network interface assigned.");
            break;
        case disabled:
            DBG("WifiStateManager::" << __func__ << ": Can't connect to "
                    << toConnect.getSSID << " wifi is disabled.");
            break;
        case turningOn:
            DBG("WifiStateManager::" << __func__ << ": Can't connect to "
                    << toConnect.getSSID << " wifi is still turning on.");
            break;
        case turningOff:
            DBG("WifiStateManager::" << __func__ << ": Can't connect to "
                    << toConnect.getSSID << " wifi is still turning off.");
            break;
        case noStateManager:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "noStateManager!");
            break;
    };
}

/**
 * If currently connected to or trying to connect to a particular
 * access point, that connection will be closed or canceled.  This method
 * acquires the WifiStateManager's stateLock.
 * 
 * @param toDisconnect should be the connected or connecting access point.
 */
void WifiStateManager::disconnect(WifiAccessPoint toDisconnect)
{
    ScopedLock wifiStateLock(stateLock);
    if (networkInterface == nullptr)
    {
        DBG("WifiStateManager::" << __func__ << ": Can't disconnect from "
                << toConnect.getSSID << " no network interface assigned.");
        return;
    }
    switch (wifiState)
    {
            //disconnect
        case connected:
            if (networkInterface->getConnectedAP() != toDisconnect)
            {
                jassert(pendingConnection.ap.isNull());
                DBG("WifiStateManager::" << __func__
                        << ": Can't disconnect from " << toConnect.getSSID()
                        << ", wifi is connected to a different access point.");
            }
            else
            {
                DBG("WifiStateManager::" << __func__ << ": Disconnecting from "
                        << toConnect.getSSID());
                networkInterface->disconnect();
                startTimer(wifiConnectionTimeout);
                setWifiState(disconnecting);
            }
            return;

            //cancel connection
        case connecting:
        case switchingConnection:
            jassert(networkInterface->getConnectedAP() != toDisconnect);
            if (pendingConnection.ap != toDisconnect)
            {
                DBG("WifiStateManager::" << __func__
                        << ": Can't disconnect from " << toConnect.getSSID()
                        << ", already connecting to a different access point.");
            }
            else
            {
                DBG("WifiStateManager::" << __func__
                        << ": Canceling connection to " << toConnect.getSSID());
                pendingConnection = {};
                cancelledConnection = toDisconnect;
                //state change will occur on networkInterface signal/timeout
                //from the former pending connection.
            }

            //no action
        case turningOff:
            DBG("WifiStateManager::" << __func__
                    << ": Wifi is turning off, no need to disconnect from "
                    << toConnect.getSSID());
            break;
        case disconnecting:
            jassert(pendingConnection.ap.isNull());
            DBG("WifiStateManager::" << __func__ << ": Wifi is already "
                    << "disconnecting with no pending connection."
            break;
        case missingNetworkInterface:
            DBG("WifiStateManager::" << __func__ << ": Can't disconnect "
                    << " from " << toConnect.getSSID()
                    << ", network interface is missing");
            break;
        case disabled:
            DBG("WifiStateManager::" << __func__ << ": Can't disconnect "
                    << " from " << toConnect.getSSID()
                    << ", wifi is disabled.");
            break;
        case turningOn:
        case enabled:
            DBG("WifiStateManager::" << __func__ << ": Can't disconnect "
                    << " from " << toConnect.getSSID()
                    << ", network interface is missing");

            break;
        case noStateManager:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "noStateManager!");
    };
}

WifiStateManager::Listener::Listener() { }

/**
 * On destruction, listeners will remove themselves from
 * their WifiStateManager if necessary. If this is necessary,
 * this method indirectly acquires the WifiStateManager's stateLock.
 */
WifiStateManager::Listener::~Listener()
{
    if (stateManager != nullptr)
    {

        stateManager->removeListener(this);
    }
}

/**
 * @return the current wifi state if the listener was added
 * to a WifiStateManager.  Otherwise, returns noStateManager.
 * If the listener was added to a state manager, this method 
 * indirectly acquires the WifiStateManager's stateLock.
 */
WifiState WifiStateManager::Listener::getWifiState()
{
    if (stateManager == nullptr)
    {

        return noStateManager;
    }
    return stateManager->getWifiState();
}

WifiStateManager::WifiNetworkInterface::WifiNetworkInterface() { }

/**
 * If the WifiNetworkInterface belongs to a WifiStateManager
 * and it's destroyed early, it will safely remove all
 * references to itself from the WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
~WifiStateManager::WifiNetworkInterface::WifiNetworkInterface()
{
    if (stateManager != nullptr)
    {
        ScopedLock wifiStateLock(stateManager->stateLock);
        if (stateManager->networkInterface == this)
        {

            DBG(__func__ << ": removing network interface from state manager.");
                    stateManager->networkInterface.release();
        }
    }
}


/**
 * Whenever the wifi device establishes a new connection,
 * the WifiNetworkInterface should call this to notify
 * its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 *
 * @param connected the newly connected access point
 */
void WifiStateManager::WifiNetworkInterface::signalWifiConnected
        (WifiAccessPoint connected) { }

/**
 * Whenever the wifi device establishes a new connection,
 * the WifiNetworkInterface should call this to notify
 * its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 *
 * @param connected the newly connected access point
 */
void WifiStateManager::WifiNetworkInterface::signalWifiConnected
        (WifiAccessPoint connected)
{
    if (stateManager == nullptr)
    {
        DBG("WifiNetworkInterface::" << __func__ << ": no state manager.");
        return;
    }
    WifiState wifiState = stateManager->getWifiState();
            ScopedLock wifiStateLock(stateManager->stateLock);
    switch (wifiState)
    {
        case disabled:
        case turningOn:
        case enabled:
        case turningOff:
        case connecting:
        case connected:
        case disconnecting:
        case switchingConnection:
        case noStateManager:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "noStateManager!");

        case missingNetworkInterface:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "missingNetworkInterface!");
    };
}

/**
 * Whenever the wifi device fails to connect to an access
 * point, the WifiNetworkInterface should call this to
 * notify its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::WifiNetworkInterface::signalConnectionFailed()
{

    if (stateManager == nullptr)
    {
        DBG("WifiNetworkInterface::" << __func__ << ": no state manager.");
        return;
    }
    WifiState wifiState = stateManager->getWifiState();
            ScopedLock wifiStateLock(stateManager->stateLock);
    switch (wifiState)
    {
        case disabled:
        case turningOn:
        case enabled:
        case turningOff:
        case connecting:
        case connected:
        case disconnecting:
        case switchingConnection:
        case noStateManager:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "noStateManager!");

        case missingNetworkInterface:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "missingNetworkInterface!");
    };
}

/**
 * Whenever the wifi device disconnects from a wifi access
 * point, the WifiNetworkInterface should call this to
 * notify its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::WifiNetworkInterface::signalWifiDisconnected()
{
    if (stateManager == nullptr)
    {
        DBG("WifiNetworkInterface::" << __func__ << ": no state manager.");
        return;
    }
    WifiState wifiState = stateManager->getWifiState();
            ScopedLock wifiStateLock(stateManager->stateLock);
    switch (wifiState)
    {
        case disabled:
        case turningOn:
        case enabled:
        case turningOff:
        case connecting:
        case connected:
        case disconnecting:
        case switchingConnection:
        case noStateManager:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "noStateManager!");

        case missingNetworkInterface:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "missingNetworkInterface!");
    };
}

/**
 * Whenever the wifi device is turned on, the 
 * WifiNetworkInterface should call this to
 * notify its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::WifiNetworkInterface::signalWifiEnabled()
{
    if (stateManager == nullptr)
    {
        DBG("WifiNetworkInterface::" << __func__ << ": no state manager.");
        return;
    }
    WifiState wifiState = stateManager->getWifiState();
            ScopedLock wifiStateLock(stateManager->stateLock);
    switch (wifiState)
    {
        case disabled:
        case turningOn:
        case enabled:
        case turningOff:
        case connecting:
        case connected:
        case disconnecting:
        case switchingConnection:
        case noStateManager:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "noStateManager!");

        case missingNetworkInterface:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "missingNetworkInterface!");
    };
}

/**
 * Whenever the wifi device is turned off, the 
 * WifiNetworkInterface should call this to
 * notify its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::WifiNetworkInterface::signalWifiDisabled()
{
    if (stateManager == nullptr)
    {
        DBG("WifiNetworkInterface::" << __func__ << ": no state manager.");
        return;
    }
    WifiState wifiState = stateManager->getWifiState();
            ScopedLock wifiStateLock(stateManager->stateLock);
    switch (wifiState)
    {
        case disabled:
        case turningOn:
        case enabled:
        case turningOff:
        case connecting:
        case connected:
        case disconnecting:
        case switchingConnection:
        case noStateManager:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "noStateManager!");

        case missingNetworkInterface:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "missingNetworkInterface!");
    };
}

/**
 * Whenever a wifi operation is attempted, the timer is set to
 * the appropriate timeout period. If the timer goes off before 
 * the WifiNetworkInterface responds, the WifiStateManager will
 * assume that the wifi operation failed.
 * This method acquires the WifiStateManager's stateLock.

 */
void WifiStateManager::timerCallback() override
{
    if (stateManager == nullptr)
    {
        DBG("WifiNetworkInterface::" << __func__ << ": no state manager.");
        return;
    }
    WifiState wifiState = stateManager->getWifiState();
            ScopedLock wifiStateLock(stateManager->stateLock);
    switch (wifiState)
    {
        case disabled:
        case turningOn:
        case enabled:
        case turningOff:
        case connecting:
        case connected:
        case disconnecting:
        case switchingConnection:
        case noStateManager:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "noStateManager!");

            break;
        case missingNetworkInterface:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "missingNetworkInterface!");
    };
}

/**
 * Update the current wifi state and notify all listeners.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::setWifiState(WifiState state)
{
    if (stateManager == nullptr)
    {
        DBG("WifiNetworkInterface::" << __func__ << ": no state manager.");
        return;
    }
    WifiState wifiState = stateManager->getWifiState();
            ScopedLock wifiStateLock(stateManager->stateLock);
    switch (wifiState)
    {
        case disabled:
        case turningOn:
        case enabled:
        case turningOff:
        case connecting:
        case connected:
        case disconnecting:
        case switchingConnection:
        case noStateManager:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "noStateManager!");

        case missingNetworkInterface:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "missingNetworkInterface!");
    };
}


/**
 * Checks the validity of the current wifi state, possibly changing the
 * state to missingNetworkInterface if necessary.  This method should only 
 * be called after acquiring the WifiStateManager's stateLock.
 * 
 * @return true iff the current wifi state is noStateManager or 
 * missingNetworkInterface
 */
bool WifiStateManager::invalidWifiState() { }

/*
switch (wifiState)
{
 case missingNetworkInterface:
 case disabled:
 case turningOn:
        case enabled:
        case turningOff:
        case connecting:
        case connected:
        case disconnecting:
        case switchingConnection:
        case noStateManager:
            DBG("WifiStateManager::" << __func__ << " ERROR: invalid state"
                    << "noStateManager!");
    };
 */