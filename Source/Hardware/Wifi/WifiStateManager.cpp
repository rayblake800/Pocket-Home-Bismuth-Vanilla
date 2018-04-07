#include "WifiStateManager.h"

ScopedPointer<ResourceManager::SharedResource>
        WifiStateManager::sharedResource = nullptr;

CriticalSection WifiStateManager::stateLock;

WifiStateManager::WifiStateManager
(std::function<ResourceManager::SharedResource()> createWifiResource):
ResourceManager(sharedResource, stateLock,createWifiResource) { }

/**
 * Gets the current state of the wifi device.
 */
WifiStateManager::WifiState WifiStateManager::getWifiState()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->getWifiState();
}


/**
 * Add a listener to the list of objects receiving updates whenever Wifi 
 * state changes.
 */
void WifiStateManager::addListener(WifiStateManager::Listener* listener)
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    wifiResource->addListener(listener);
}

/**
 * Searches the list of registered listeners for a particular one, and
 * removes it if it's found. 
 */
void WifiStateManager::removeListener(WifiStateManager::Listener* listener)
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    wifiResource->removeListener(listener);
}

/**
 * Gets the connected wifi access point, if one exists.
 */
WifiAccessPoint WifiStateManager::getConnectedAP()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->getConnectedAP();
}

/**
 * Gets the connecting wifi access point, if one exists.
 */
WifiAccessPoint WifiStateManager::getConnectingAP()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->getConnectingAP();
}

/**
 * Gets all access points visible to the wifi device.
 */
Array<WifiAccessPoint> WifiStateManager::getVisibleAPs()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->getVisibleAPs();
}

/**
 * Checks if wifi is enabled.
 */
bool WifiStateManager::isEnabled()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->isWifiEnabled();
}

/**
 * Checks if the wifi device is connected to an access point.
 */
bool WifiStateManager::isConnected()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->isWifiConnected();
}

/**
 * Attempts to open a connection to a wifi access point. This will fail if 
 * wifi is disabled, the access point is invalid, or the psk is wrong.
 */
void WifiStateManager::connectToAccessPoint(const WifiAccessPoint& toConnect,
        String psk)
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    wifiResource->connectToAccessPoint(toConnect, psk);
}

/**
 * If currently connected to or trying to connect to a particular access 
 * point, that connection will be closed or canceled.
 * 
 * @param toDisconnect  Specifies the access point that should not be 
 *                       connected.
 */
void WifiStateManager::disconnect(const WifiAccessPoint& toDisconnect)
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    wifiResource->disconnect(toDisconnect);
}

/**
 * If wifi is currently disabled, this will enable it.  Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::enableWifi()
{
    if(!invalidWifiState())
    {
        const ScopedLock lock(stateLock);
        NetworkInterface* wifiResource
                = static_cast<NetworkInterface*> (sharedResource.get());
        wifiResource->enableWifi();
    }
}

/**
 * If wifi is currently enabled, this will disable it. Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::disableWifi()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    wifiResource->disableWifi();
}

/**
 * Gets the current state of the wifi device.
 */
WifiStateManager::WifiState WifiStateManager::NetworkInterface::getWifiState()
{
    return wifiState;
}


/**
 * Update the current wifi state and notify all listeners.
 */
void WifiStateManager::NetworkInterface::setWifiState(WifiState state)
{
    const ScopedLock stateLock(wifiLock);
    if (state != wifiState)
    {
        wifiState = state;
        DBG("WifiStateManager::" << __func__ << ": Setting wifi state to "
                << wifiStateString(wifiState));
        while (!notifyQueue.isEmpty())
        {
            //All notification occurs on the message thread, so we must be
            //calling from another thread while the message thread handles a
            //different set of notifications.  Unlock and yield until
            //awaitingSignal is empty again.
            const ScopedUnlock signalUnlock(wifiLock);
            Thread::yield();
        }
        notifyQueue = listeners;
        while (!notifyQueue.isEmpty())
        {
            Listener* toNotify
                    = notifyQueue.removeAndReturn(notifyQueue.size() - 1);
            const ScopedUnlock signalUnlock(wifiLock);
            const MessageManagerLock mmLock;
            toNotify->wifiStateChanged(state);
        }
    }
}


/**
 * Add a listener to the list of objects receiving updates whenever Wifi state 
 * changes.
 */
void WifiStateManager::NetworkInterface::addListener
(WifiStateManager::Listener* listener)
{

    listeners.add(listener);
}

/**
 * Searches the list of registered listeners for a particular one, and removes
 * it if it's found. 
 */
void WifiStateManager::NetworkInterface::removeListener
(WifiStateManager::Listener* listener)
{

    listeners.removeAllInstancesOf(listener);
}


/**
 * On destruction, listeners will remove themselves from
 * their WifiStateManager if necessary.
 */
WifiStateManager::Listener::~Listener()
{
    WifiStateManager stateManager;
    stateManager.removeListener(this);
}

/**
 * This method queries the wifi device to ensure that the current tracked wifi 
 * state actually matches the device state.  If necessary, this will update the 
 * WifiStateManager's current state.  The NetworkInterface should call this 
 * method whenever it has reason to believe the wifi state may be out of date.  
 */
void WifiStateManager::NetworkInterface::confirmWifiState()
{
    const ScopedLock wifiStateLock(wifiLock);
    bool wifiConnecting = isWifiConnecting();
    bool wifiConnected = isWifiConnected();
    WifiState state = getWifiState();
    const ScopedUnlock wifiStateUnlock(wifiLock);
    if(!wifiDeviceFound())
    {
        if (state != missingNetworkDevice)
        {
            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but should be missingNetworkDevice.");
            setWifiState(missingNetworkDevice);
        }
        return;    
    }
    if (!isWifiEnabled())
    {
        if (state != turningOn && state != disabled)
        {
            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is actually disabled."); 
            setWifiState(disabled);
        }
        return;
    }
    if (wifiConnecting && wifiConnected)
    {
        if (state != switchingConnection && state != turningOff)
        {
            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is actually switching connections.");
            setWifiState(switchingConnection);
            const ScopedLock timerLock(wifiLock);
            if (!isTimerRunning())
            {
                startTimer(wifiConnectionTimeout);
            }
        }
        return;
    }
    else if (!wifiConnecting && !wifiConnected)
    {
        if (state != enabled && state != turningOff)
        {
            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is enabled/not connected.");
            setWifiState(enabled);
        }
        return;
    }
    else if (wifiConnecting && !wifiConnected)
    {
        if (state != connecting && state != turningOff)
        {

            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is connecting.");
            setWifiState(connecting);
            const ScopedLock timerLock(wifiLock);
            if (!isTimerRunning())
            {
                startTimer(wifiConnectionTimeout);
            }
            return;
        }
    }
    else if (!wifiConnecting && wifiConnected)
    {
        if (state != connected && state != disconnecting)
        {

            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is connected.");
            setWifiState(connected);
            const ScopedLock timerLock(wifiLock);
            if (!isTimerRunning())
            {
                startTimer(wifiConnectionTimeout);
            }
        }
        return;
    }
}

/**
 * Whenever the wifi device detects a connection being established, the 
 * NetworkInterface should call this to notify its WifiStateManager.
 */
void WifiStateManager::NetworkInterface::signalWifiConnecting()
{
    const ScopedLock updateLock(wifiLock);
    if (invalidWifiState())
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");
        return;
    }
    WifiState wifiState = getWifiState();
    if (wifiState != turningOff && wifiState != connecting
        && wifiState != switchingConnection)
    {

        DBG("NetworkInterface::" << __func__
                << ": started to connect during state "
                << wifiStateString(wifiState));
        startTimer(wifiConnectionTimeout);
        const ScopedUnlock stateUnlock(wifiLock);
        setWifiState(connecting);
    }
}

/**
 * Whenever the wifi device establishes a new connection, the NetworkInterface 
 * should call this to notify its WifiStateManager. This method acquires the 
 * WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiConnected
(const WifiAccessPoint& connectedAP)
{
    DBG("NetworkInterface::" << __func__ << ": connected to "
            << connectedAP.getSSID() << " during state "
            << wifiStateString(getWifiState()));
    setWifiState(connected);
}

/**
 * Whenever the wifi device fails to connect to an access point, the 
 * NetworkInterface should call this to  notify its WifiStateManager. This 
 * method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalConnectionFailed()
{
    switch (getWifiState())
    {
        case switchingConnection:
        case connecting:
            DBG("NetworkInterface::" << __func__ << ": connection failed.");
            {
                const ScopedLock timerLock(wifiLock);
                stopTimer();
            }
            setWifiState(enabled);
            return;
        default:
            DBG("NetworkInterface::" << __func__
                    << ": unexpected connection failure from state "
                    << wifiStateString(wifiResource->getWifiState()));
    }
}

/**
 * Whenever the wifi device disconnects from a wifi access point, the 
 * NetworkInterface should call this to notify its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiDisconnected()
{
    const ScopedLock wifiStateLock(wifiLock);
    DBG("NetworkInterface::" << __func__ << ": wifi disconnected");
    stopTimer();
    WifiState currentState = getWifiState();
    const ScopedUnlock stateUnlock(wifiLock);
    
    switch (currentState)
    {
        case switchingConnection:
        {
            setWifiState(connecting);
            const ScopedLock timerLock(wifiLock);
            startTimer(wifiConnectionTimeout);
        }
        case connecting:
        case disconnecting:
        case connected:
            setWifiState(enabled);
            return;
        case turningOff:
            return;
        default:
            DBG("NetworkInterface::" << __func__
                    << ": unexpected disconnect from state "
                    << wifiStateString(currentState));
    }
}

/**
 * Whenever the wifi device is turned on, the NetworkInterface should call this
 * to notify its WifiStateManager. This method acquires the WifiStateManager's 
 * stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiEnabled()
{

    DBG("NetworkInterface::" << __func__ << ": wifi enabled");
    setWifiState(enabled);
    const ScopedLock timerLock(wifiLock);
    stopTimer();
}

/**
 * Whenever the wifi device is turned off, the NetworkInterface should call this
 * to notify its WifiStateManager. This method acquires the WifiStateManager's 
 * stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiDisabled()
{
    DBG("NetworkInterface::" << __func__ << ": wifi disabled");
    setWifiState(disabled);
    const ScopedLock timerLock(wifiLock);
    stopTimer();
}



/**
 * Whenever a wifi operation is attempted, the timer is set to
 * the appropriate timeout period. If the timer goes off before 
 * the NetworkInterface responds, the WifiStateManager will
 * assume that the wifi operation failed.
 */
void WifiStateManager::NetworkInterface::timerCallback()
{
    const ScopedLock lock(wifiLock);
    stopTimer();
    bool wifiEnabled = isWifiEnabled();
    bool wifiConnected = isWifiConnected();
    WifiState currentState = wifiState;
    const ScopedUnlock stateUpdateUnlock(wifiLock);
    switch (currentState)
    {
        case connecting:
        case switchingConnection:
            DBG("WifiStateManager::" << __func__
                    << ": wifi connection timed out.");
            setWifiState(enabled);
            return;
        case disconnecting:
            if (wifiConnected)
            {
                DBG("WifiStateManager::" << __func__
                        << ": disconnecting failed, this shouldn't be possible!");
            }
            else
            {
                DBG("WifiStateManager::" << __func__
                        << ": finished disconnecting, but no signal was received");
                setWifiState(enabled);
            }
            return;
        case turningOn:
            if (wifiEnabled)
            {
                DBG("WifiStateManager::" << __func__
                        << ": failed to enable wifi!");
            }
            else
            {
                DBG("WifiStateManager::" << __func__
                        << ": finished turning on wifi, but no signal was received");
                setWifiState(wifiEnabled ? enabled : disabled);
            }
            return;
        case turningOff:
            if (wifiEnabled)
            {
                DBG("WifiStateManager::" << __func__
                        << ": failed to disable wifi!");
            }
            else
            {

                DBG("WifiStateManager::" << __func__
                        << ": finished turning off wifi, but no signal was received");
                setWifiState(wifiEnabled ? enabled : disabled);
            }
            return;
        case disabled:
        case enabled:
        case connected:

            DBG("NetworkInterface::" << __func__
                    << ": unexpected timeout from state "
                    << wifiStateString(currentState));
    };
}

/**
 * Attempts to open a connection to a wifi access point. This will fail if 
 * wifi is disabled, the access point is invalid, or the psk is wrong.
 */
void WifiStateManager::NetworkInterface::connectToAccessPoint
(const WifiAccessPoint& toConnect, String psk)
{
    if (invalidWifiState())
    {
        DBG("WifiStateManager::" << __func__ << ": invalid wifi state:"
                << wifiStateString(wifiState));
        return;
    }
    if (toConnect.isVoid())
    {
        DBG("WifiStateManager::" << __func__
                << ": Tried to connect to null access point!");
        return;
    }

    switch (wifiState)
    {
        //start connecting:
        case connected:
            
            disconnect();
        case enabled:
            DBG("WifiStateManager::" << __func__ << ": Connecting to access "
                    << "point " << toConnect.getSSID());
            connectToAccessPoint(toConnect, psk);
            break;
            //start switching access point:
            if (getConnectedAP() == toConnect)
            {
                DBG("WifiStateManager::" << __func__ << ": Trying to open "
                        << " connection to access point " << toConnect.getSSID()
                        << "but that access point is already connected");
                return;
            }
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
        case disabled:
            DBG("WifiStateManager::" << __func__ << ": Can't connect to "
                    << toConnect.getSSID() << " wifi is disabled.");
            break;
        case turningOn:
            DBG("WifiStateManager::" << __func__ << ": Can't connect to "
                    << toConnect.getSSID() << " wifi is still turning on.");
            break;
        case turningOff:
            DBG("WifiStateManager::" << __func__ << ": Can't connect to "
                    << toConnect.getSSID() << " wifi is still turning off.");

            break;
    }
}

/**
 * If currently connected to or trying to connect to a particular
 * access point, that connection will be closed or canceled. 
 */
void WifiStateManager::NetworkInterface::disconnect
(const WifiAccessPoint& toDisconnect)
{
    if (invalidWifiState())
    {
        DBG("WifiStateManager::" << __func__ << ": invalid wifi state:"
                << wifiStateString(wifiState));
        return;
    }
    switch (wifiState)
    {
            //disconnect
        case connected:
            if (networkInterface->getConnectedAP() != toDisconnect)
            {
                jassert(pendingConnection.ap.isVoid());
                DBG("WifiStateManager::" << __func__
                        << ": Can't disconnect from " << toDisconnect.getSSID()
                        << ", wifi is connected to a different access point.");
            }
            else
            {
                DBG("WifiStateManager::" << __func__ << ": Disconnecting from "
                        << toDisconnect.getSSID());
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
                        << ": Can't disconnect from " << toDisconnect.getSSID()
                        << ", already connecting to a different access point.");
            }
            else
            {
                DBG("WifiStateManager::" << __func__
                        << ": Canceling connection to " << toDisconnect.getSSID());
                pendingConnection = {};
                cancelledConnection = toDisconnect;
                //state change will occur on networkInterface signal/timeout
                //from the former pending connection.
            }

            //no action
        case turningOff:
            DBG("WifiStateManager::" << __func__
                    << ": Wifi is turning off, no need to disconnect from "
                    << toDisconnect.getSSID());
            break;
        case disconnecting:
            jassert(pendingConnection.ap.isVoid());
            DBG("WifiStateManager::" << __func__ << ": Wifi is already "
                    << "disconnecting with no pending connection.");
            break;
        case disabled:
            DBG("WifiStateManager::" << __func__ << ": Can't disconnect "
                    << " from " << toDisconnect.getSSID()
                    << ", wifi is disabled.");

            break;
        case turningOn:
        case enabled:
            DBG("WifiStateManager::" << __func__ << ": Can't disconnect "
                    << " from " << toDisconnect.getSSID()
                    << ", network interface is missing");
    }
}

/**
 * If wifi is currently disabled, this will enable it.  Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::NetworkInterface::enableWifi()
{
    ScopedLock wifiStateLock(stateLock);
    if (invalidWifiState())
    {
        DBG("WifiStateManager::" << __func__ << ": invalid wifi state:"
                << wifiStateString(wifiState));
        return;
    }
    switch (wifiState)
    {
        case disabled:
            DBG("WifiStateManager::WifiResource::" << __func__ 
                    << ": enabling wifi");
            networkInterface->enableWifi();
            startTimer(wifiEnabledChangeTimeout);
            setWifiState(turningOn);
            return;
        case turningOn:
            DBG("WifiStateManager::" << __func__
                    << ": wifi is already being enabled");

            return;
        case enabled:
        case turningOff:
        case connecting:
        case connected:
        case disconnecting:
        case switchingConnection:
            DBG("WifiStateManager::" << __func__ << ": wifi is already on");
    }
}

/**
 * If wifi is currently enabled, this will disable it. Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::NetworkInterface::disableWifi()
{
    if (invalidWifiState())
    {
        DBG("WifiStateManager::" << __func__ << ": invalid wifi state:"
                << wifiStateString(wifiState));
        return;
    }
    switch (wifiState)
    {
        case switchingConnection:
            pendingConnection = {};
        case turningOn:
        case enabled:
        case connecting:
        case connected:
        case disconnecting:
            DBG("WifiStateManager::" << __func__ << ": disabling wifi");
            networkInterface->disableWifi();
            startTimer(wifiEnabledChangeTimeout);
            setWifiState(turningOff);
            return;
        case turningOff:
            DBG("WifiStateManager::" << __func__
                    << ": wifi is already turning off.");
            return;
        case disabled:
            DBG("WifiStateManager::" << __func__
                    << ": wifi is already disabled");

            return;
    }
}

/**
 * Checks the validity of the current wifi state, possibly changing the
 * state to missingNetworkInterface if necessary.
 * 
 * @return true iff the current wifi state is noStateManager or 
 * missingNetworkInterface
 */
bool WifiStateManager::invalidWifiState()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->invalidWifiState();
}


