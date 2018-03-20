#include "WifiStateManager.h"

WifiStateManager::WifiStateManager() { }

/**
 * On destruction, the WifiStateManager removes all references
 * to itself from its Listener objects, and destroys its
 * NetworkInterface.
 * This method acquires the WifiStateManager's stateLock.
 */
WifiStateManager::~WifiStateManager()
{
    ScopedLock wifiStateLock(stateLock);
    for (Listener* listener : listeners)
    {
        listener->stateManager = nullptr;
    }
    if (networkInterface != nullptr)
    {
        networkInterface->stateManager = nullptr;
    }
}

/**
 * Assigns a NetworkInterface to this WifiStateManager.
 * The state manager will take ownership of the network
 * interface, deleting it on destruction or when a new
 * interface is provided through setNetworkInterface.
 * This method acquires the WifiStateManager's stateLock.
 *
 * @param interface takes responsibility for communicating
 * with the Wifi device.
 */
void WifiStateManager::setNetworkInterface(NetworkInterface* interface)
{
    ScopedLock wifiStateLock(stateLock);
    if (networkInterface != nullptr && networkInterface->stateManager == this)
    {
        networkInterface->stateManager = nullptr;
    }
    networkInterface = interface;
    networkInterface->stateManager = this;
    networkInterface->confirmWifiState();
}

/**
 * @return the current state of the wifi device, or
 * missingNetworkInterface if the NetworkInterface
 * hasn`t been set.
 * This method acquires the WifiStateManager's stateLock.
 */
WifiStateManager::WifiState WifiStateManager::getWifiState()
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
    return networkInterface->getVisibleAPs();
}

/**
 * @return true iff the wifi device is currently enabled.
 */
bool WifiStateManager::isEnabled()
{
    if (networkInterface == nullptr)
    {
        return false;
    }
    return networkInterface->isWifiEnabled();
}

/**
 * @return true iff the wifi device is currently connected.
 */
bool WifiStateManager::isConnected()
{
    if (networkInterface == nullptr)
    {
        return false;
    }
    return networkInterface->isWifiConnected();
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
void WifiStateManager::connectToAccessPoint(const WifiAccessPoint& toConnect,
        String psk)
{
    ScopedLock wifiStateLock(stateLock);
    if (invalidWifiState())
    {
        DBG("WifiStateManager::" << __func__ << ": invalid wifi state:"
                << wifiStateString(wifiState));
        return;
    }
    if (toConnect.isNull())
    {
        DBG("WifiStateManager::" << __func__
                << ": Tried to connect to null access point!");
        return;
    }
    if (cancelledConnection == toConnect)
    {

        DBG("WifiStateManager::" << __func__ << ": Connecting to "
                << toConnect.getSSID() << " was recently canceled, attempting to"
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
 * access point, that connection will be closed or canceled.  This method
 * acquires the WifiStateManager's stateLock.
 * 
 * @param toDisconnect should be the connected or connecting access point.
 */
void WifiStateManager::disconnect(const WifiAccessPoint& toDisconnect)
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
            //disconnect
        case connected:
            if (networkInterface->getConnectedAP() != toDisconnect)
            {
                jassert(pendingConnection.ap.isNull());
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
            jassert(pendingConnection.ap.isNull());
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
 * If wifi is currently disabled, this will enable it.  Otherwise,
 * nothing will happen.  This method acquires the WifiStateManager's 
 * stateLock.
 */
void WifiStateManager::enableWifi()
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
            DBG("WifiStateManager::" << __func__ << ": enabling wifi");
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
 * If wifi is currently enabled, this will disable it.  Otherwise,
 * nothing will happen.  This method acquires the WifiStateManager's 
 * stateLock.
 */
void WifiStateManager::disableWifi()
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
WifiStateManager::WifiState WifiStateManager::Listener::getWifiState()
{
    if (stateManager == nullptr)
    {

        return noStateManager;
    }
    return stateManager->getWifiState();
}

WifiStateManager::NetworkInterface::NetworkInterface() { }

/**
 * If the NetworkInterface belongs to a WifiStateManager
 * and it's destroyed early, it will safely remove all
 * references to itself from the WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
WifiStateManager::NetworkInterface::~NetworkInterface()
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
 * This method queries the wifi device to ensure that the current 
 * tracked wifi state actually matches the device state.  If necessary,
 * this will update the WifiStateManager's current state.  The
 * NetworkInterface should call this method whenever it has reason
 * to believe the wifi state may be out of date.  
 * 
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::confirmWifiState()
{
    if (stateManager == nullptr)
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << "missing");
        return;
    }
    bool wifiConnecting = isWifiConnecting();
    bool wifiConnected = isWifiConnected();
    ScopedLock wifiStateLock(stateManager->stateLock);
    WifiState state = stateManager->wifiState;
    if (!isWifiEnabled())
    {
        if (state != turningOn && state != disabled)
        {
            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is actually disabled.");
            stateManager->setWifiState(disabled);
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
            stateManager->setWifiState(switchingConnection);
            if (!stateManager->isTimerRunning())
            {
                stateManager->startTimer
                        (stateManager->wifiConnectionTimeout);
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
            stateManager->setWifiState(enabled);
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
            stateManager->setWifiState(connecting);
            if (!stateManager->isTimerRunning())
            {
                stateManager->startTimer
                        (stateManager->wifiConnectionTimeout);
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
            stateManager->setWifiState(connected);
            if (!stateManager->isTimerRunning())
            {
                stateManager->startTimer
                        (stateManager->wifiConnectionTimeout);
            }
        }
        return;
    }
}

/**
 * Whenever the wifi device detects a connection being established, the 
 * WifiNetworkInterface should call this to notify its WifiStateManager. 
 * This is intended to notify the state manager of unexpected
 * connections, so it's not necessary to call this for connections
 * opened by the state manager, but extra calls shouldn't cause any
 * real problems.
 * This method acquires the WifiStateManager's stateLock.
 *
 * @param connectedAP the newly connected access point
 */
void WifiStateManager::NetworkInterface::signalWifiConnecting()
{
    if (invalidWifiState())
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");
        return;
    }
    ScopedLock wifiStateLock(stateManager->stateLock);
    WifiState wifiState = stateManager->wifiState;
    if (wifiState != turningOff && wifiState != connecting
        && wifiState != switchingConnection)
    {

        DBG("NetworkInterface::" << __func__
                << ": started to connect during state "
                << wifiStateString(wifiState));
        stateManager->startTimer(stateManager->wifiConnectionTimeout);
        stateManager->setWifiState(connecting);

    }
}

/**
 * Whenever the wifi device establishes a new connection,
 * the NetworkInterface should call this to notify
 * its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 *
 * @param connectedAP the newly connected access point
 */
void WifiStateManager::NetworkInterface::signalWifiConnected
(const WifiAccessPoint& connectedAP)
{
    if (invalidWifiState())
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");
        return;
    }
    ScopedLock wifiStateLock(stateManager->stateLock);
    WifiState wifiState = stateManager->wifiState;
    if (stateManager->cancelledConnection == connectedAP
        || (!stateManager->pendingConnection.ap.isNull() &&
            stateManager->pendingConnection.ap != connectedAP))
    {
        jassert(wifiState == disconnecting || wifiState == switchingConnection);
        DBG("NetworkInterface::" << __func__ << ": connected to "
                << connectedAP.getSSID() << "but that connection was canceled.");
        DBG("NetworkInterface::" << __func__ << ": disconnecting...");
        if (wifiState != switchingConnection)
        {
            stateManager->setWifiState(disconnecting);
        }
        disconnect();
        return;
    }
    else
    {

        stateManager->pendingConnection = {};
        stateManager->cancelledConnection = WifiAccessPoint();
    }
    DBG("NetworkInterface::" << __func__ << ": connected to "
            << connectedAP.getSSID() << " during state "
            << wifiStateString(wifiState));
    stateManager->setWifiState(connected);
}

/**
 * Whenever the wifi device fails to connect to an access
 * point, the NetworkInterface should call this to
 * notify its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalConnectionFailed()
{
    if (invalidWifiState())
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");
        return;
    }
    ScopedLock wifiStateLock(stateManager->stateLock);
    switch (stateManager->wifiState)
    {

        case switchingConnection:
        case connecting:
            DBG("NetworkInterface::" << __func__ << ": connection failed.");
            stateManager->pendingConnection = {};
            stateManager->cancelledConnection = WifiAccessPoint();
            stateManager->setWifiState(enabled);
            stateManager->stopTimer();

            return;
        case disabled:
        case turningOn:
        case enabled:
        case turningOff:
        case connected:
        case disconnecting:
            DBG("NetworkInterface::" << __func__
                    << ": unexpected connection failure from state "
                    << wifiStateString(stateManager->wifiState));
    }
}

/**
 * Whenever the wifi device disconnects from a wifi access
 * point, the NetworkInterface should call this to
 * notify its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiDisconnected()
{
    if (invalidWifiState())
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");
        return;
    }
    ScopedLock wifiStateLock(stateManager->stateLock);
    DBG("NetworkInterface::" << __func__ << ": wifi disconnected");
    stateManager->stopTimer();
    switch (stateManager->wifiState)
    {

        case switchingConnection:
            jassert(!stateManager->pendingConnection.ap.isNull());
            DBG("NetworkInterface::" << __func__
                    << ": old connection closed, opening connection to "
                    << stateManager->pendingConnection.ap.getSSID());
            connectToAccessPoint(stateManager->pendingConnection.ap,
                    stateManager->pendingConnection.psk);
            stateManager->setWifiState(connecting);
            stateManager->startTimer(stateManager->wifiConnectionTimeout);
        case connecting:
        case disconnecting:
        case connected:
            stateManager->setWifiState(enabled);

            return;
        case turningOff:
            return;
        case disabled:
        case turningOn:
        case enabled:
            DBG("NetworkInterface::" << __func__
                    << ": unexpected disconnect from state "
                    << wifiStateString(stateManager->wifiState));
    }
}

/**
 * Whenever the wifi device is turned on, the 
 * NetworkInterface should call this to
 * notify its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiEnabled()
{
    if (invalidWifiState())
    {

        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");

        return;
    }
    ScopedLock wifiStateLock(stateManager->stateLock);
    DBG("NetworkInterface::" << __func__ << ": wifi enabled");
    stateManager->stopTimer();
    stateManager->setWifiState(enabled);
}

/**
 * Whenever the wifi device is turned off, the 
 * NetworkInterface should call this to
 * notify its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiDisabled()
{
    if (invalidWifiState())
    {

        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");

        return;
    }
    ScopedLock wifiStateLock(stateManager->stateLock);
    DBG("NetworkInterface::" << __func__ << ": wifi disabled");
    stateManager->stopTimer();
    stateManager->setWifiState(disabled);
}

/**
 * @return true if the state manager is missing or in
 * an invalid state.
 */
bool WifiStateManager::NetworkInterface::invalidWifiState()
{
    if (stateManager != nullptr)
    {

        WifiState state = stateManager->wifiState;

        return state == noStateManager
                || state == missingNetworkInterface;
    }
    return true;
}

/**
 * Whenever a wifi operation is attempted, the timer is set to
 * the appropriate timeout period. If the timer goes off before 
 * the NetworkInterface responds, the WifiStateManager will
 * assume that the wifi operation failed.
 * This method acquires the WifiStateManager's stateLock.

 */
void WifiStateManager::timerCallback()
{
    ScopedLock wifiStateLock(stateLock);
    stopTimer();
    if (invalidWifiState())
    {
        DBG("WifiStateManager::" << __func__ << ": invalid wifi state:"
                << wifiStateString(wifiState));
        return;
    }
    pendingConnection = {};
    cancelledConnection = WifiAccessPoint();
    switch (wifiState)
    {
        case connecting:
        case switchingConnection:
            DBG("WifiStateManager::" << __func__
                    << ": wifi connection timed out.");
            setWifiState(enabled);
            return;
        case disconnecting:
            if (networkInterface->isWifiConnected())
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
            if (networkInterface->isWifiEnabled())
            {
                DBG("WifiStateManager::" << __func__
                        << ": failed to enable wifi!");
            }
            else
            {
                DBG("WifiStateManager::" << __func__
                        << ": finished turning on wifi, but no signal was received");
                setWifiState(enabled);
            }
            return;
        case turningOff:
            if (!networkInterface->isWifiEnabled())
            {
                DBG("WifiStateManager::" << __func__
                        << ": failed to disable wifi!");
            }
            else
            {

                DBG("WifiStateManager::" << __func__
                        << ": finished turning off wifi, but no signal was received");
                setWifiState(disabled);
            }
            return;
        case disabled:
        case enabled:
        case connected:

            DBG("NetworkInterface::" << __func__
                    << ": unexpected timeout from state "
                    << wifiStateString(wifiState));
    };
}

/**
 * When window focus is lost, the timer will be canceled.  
 */
void WifiStateManager::windowFocusLost()
{
    stopTimer();
}

/**
 * When window focus is gained, if the network interface has been assigned,
 * the wifi state will be updated.
 */
void WifiStateManager::windowFocusGained()
{
    if (networkInterface != nullptr)
    {
        networkInterface->confirmWifiState();
    }
}

/**
 * Update the current wifi state and notify all listeners.
 * This method should only be called after acquiring the WifiStateManager's
 * stateLock
 */
void WifiStateManager::setWifiState(WifiState state)
{
    if (networkInterface == nullptr)
    {
        state = missingNetworkInterface;
    }
    if (state != wifiState)
    {
        wifiState = state;
        DBG("WifiStateManager::" << __func__ << ": Setting wifi state to "
                << wifiStateString(wifiState));
        for (Listener* listener : listeners)
        {

            listener->wifiStateChanged(wifiState);
        }
    }
}

/**
 * Checks the validity of the current wifi state, possibly changing the
 * state to missingNetworkInterface if necessary.  This method should only 
 * be called after acquiring the WifiStateManager's stateLock.
 */
bool WifiStateManager::invalidWifiState()
{
    if (networkInterface == nullptr)
    {
        if (wifiState != missingNetworkInterface)
        {
            setWifiState(missingNetworkInterface);
        }
        return true;
    }
    return wifiState == noStateManager
            || wifiState == missingNetworkInterface;
}

