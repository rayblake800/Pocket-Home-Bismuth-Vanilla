#include "WifiStateManager.h"

ScopedPointer<ResourceManager::SharedResource>
        WifiStateManager::sharedResource = nullptr;

CriticalSection WifiStateManager::stateLock;

WifiStateManager::WifiStateManager() :
ResourceManager(sharedResource, stateLock,
[this]()->ResourceManager::SharedResource*
{

    return new WifiResource(stateLock);
}) { }

/**
 * Assigns a WifiNetworkInterface to this WifiStateManager. The state 
 * manager will take ownership of the network  interface, deleting it on 
 * destruction or when a new  interface is provided through 
 * setNetworkInterface.
 */
void WifiStateManager::setNetworkInterface
(WifiStateManager::NetworkInterface* interface)
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    wifiResource->setNetworkInterface(interface);
    interface->wifiResource = wifiResource;
    interface->wifiLock = &stateLock;
    interface->confirmWifiState();
}

/**
 * Gets the current state of the wifi device.
 */
WifiStateManager::WifiState WifiStateManager::getWifiState()
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    return wifiResource->getWifiState();
}

/**
 * Add a listener to the list of objects receiving updates whenever Wifi 
 * state changes.
 */
void WifiStateManager::addListener(WifiStateManager::Listener* listener)
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    wifiResource->addListener(listener);
}

/**
 * Searches the list of registered listeners for a particular one, and
 * removes it if it's found. 
 */
void WifiStateManager::removeListener(WifiStateManager::Listener* listener)
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    wifiResource->removeListener(listener);
}

/**
 * Gets the connected wifi access point, if one exists.
 */
WifiAccessPoint WifiStateManager::getConnectedAP()
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    return wifiResource->getConnectedAP();
}

/**
 * Gets the connecting wifi access point, if one exists.
 */
WifiAccessPoint WifiStateManager::getConnectingAP()
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    return wifiResource->getConnectingAP();
}

/**
 * Gets all access points visible to the wifi device.
 */
Array<WifiAccessPoint> WifiStateManager::getVisibleAPs()
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    return wifiResource->getVisibleAPs();
}

/**
 * Checks if wifi is enabled.
 */
bool WifiStateManager::isEnabled()
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    return wifiResource->isEnabled();
}

/**
 * Checks if the wifi device is connected to an access point.
 */
bool WifiStateManager::isConnected()
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    return wifiResource->isConnected();
}

/**
 * Attempts to open a connection to a wifi access point. This will fail if 
 * wifi is disabled, the access point is invalid, or the psk is wrong.
 */
void WifiStateManager::connectToAccessPoint(const WifiAccessPoint& toConnect,
        String psk)
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
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
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    wifiResource->disconnect(toDisconnect);
}

/**
 * If wifi is currently disabled, this will enable it.  Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::enableWifi()
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    wifiResource->enableWifi();
}

/**
 * If wifi is currently enabled, this will disable it. Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::disableWifi()
{
    const ScopedLock lock(stateLock);
    WifiResource* wifiResource
            = static_cast<WifiResource*> (sharedResource.get());
    wifiResource->disableWifi();
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
 * 
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::confirmWifiState()
{
    if (wifiResource == nullptr || wifiLock == nullptr)
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << "missing");
        return;
    }
    bool wifiConnecting = isWifiConnecting();
    bool wifiConnected = isWifiConnected();
    const ScopedLock wifiStateLock(*wifiLock);
    WifiState state = wifiResource->getWifiState();
    if (!isWifiEnabled())
    {
        if (state != turningOn && state != disabled)
        {
            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is actually disabled.");
            wifiResource->setWifiState(disabled);
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
            wifiResource->setWifiState(switchingConnection);
            if (!wifiResource->isTimerRunning())
            {
                wifiResource->startTimer
                        (wifiResource->wifiConnectionTimeout);
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
            wifiResource->setWifiState(enabled);
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
            wifiResource->setWifiState(connecting);
            if (!wifiResource->isTimerRunning())
            {
                wifiResource->startTimer
                        (wifiResource->wifiConnectionTimeout);
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
            wifiResource->setWifiState(connected);
            if (!wifiResource->isTimerRunning())
            {
                wifiResource->startTimer
                        (wifiResource->wifiConnectionTimeout);
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
    if (invalidWifiState())
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");
        return;
    }
    const ScopedLock wifiStateLock(*wifiLock);
    WifiState wifiState = wifiResource->getWifiState();
    if (wifiState != turningOff && wifiState != connecting
        && wifiState != switchingConnection)
    {

        DBG("NetworkInterface::" << __func__
                << ": started to connect during state "
                << wifiStateString(wifiState));
        wifiResource->startTimer(wifiResource->wifiConnectionTimeout);
        wifiResource->setWifiState(connecting);

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
    if (invalidWifiState())
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");
        return;
    }
    const ScopedLock wifiStateLock(*wifiLock);
    WifiState wifiState = wifiResource->getWifiState();
    if (wifiResource->getCancelledConnection() == connectedAP
        || (!wifiResource->getPendingAp().isNull() &&
            wifiResource->getPendingAp() != connectedAP))
    {
        jassert(wifiState == disconnecting || wifiState == switchingConnection);
        DBG("NetworkInterface::" << __func__ << ": connected to "
                << connectedAP.getSSID() << "but that connection was canceled.");
        DBG("NetworkInterface::" << __func__ << ": disconnecting...");
        if (wifiState != switchingConnection)
        {
            wifiResource->setWifiState(disconnecting);
        }
        disconnect();
        return;
    }
    else
    {
        wifiResource->setPendingConnection(WifiAccessPoint(), String());
        wifiResource->setCancelledConnection(WifiAccessPoint());
    }
    DBG("NetworkInterface::" << __func__ << ": connected to "
            << connectedAP.getSSID() << " during state "
            << wifiStateString(wifiState));
    wifiResource->setWifiState(connected);
}

/**
 * Whenever the wifi device fails to connect to an access point, the 
 * NetworkInterface should call this to  notify its WifiStateManager. This 
 * method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalConnectionFailed()
{
    if (invalidWifiState())
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");
        return;
    }
    const ScopedLock wifiStateLock(*wifiLock);
    switch (wifiResource->getWifiState())
    {

        case switchingConnection:
        case connecting:
            DBG("NetworkInterface::" << __func__ << ": connection failed.");
            wifiResource->setPendingConnection(WifiAccessPoint(), String());
            wifiResource->setCancelledConnection(WifiAccessPoint());
            wifiResource->setWifiState(enabled);
            wifiResource->stopTimer();
            return;
        case disabled:
        case turningOn:
        case enabled:
        case turningOff:
        case connected:
        case disconnecting:
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
    if (invalidWifiState())
    {
        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");
        return;
    }
    const ScopedLock wifiStateLock(*wifiLock);
    DBG("NetworkInterface::" << __func__ << ": wifi disconnected");
    wifiResource->stopTimer();
    switch (wifiResource->getWifiState())
    {

        case switchingConnection:
            jassert(!wifiResource->getPendingAp().isNull());
            DBG("NetworkInterface::" << __func__
                    << ": old connection closed, opening connection to "
                    << wifiResource->getPendingAp().getSSID());
            connectToAccessPoint(wifiResource->getPendingAp(),
                    wifiResource->getPendingPsk());
            wifiResource->setWifiState(connecting);
            wifiResource->startTimer(wifiResource->wifiConnectionTimeout);
        case connecting:
        case disconnecting:
        case connected:
            wifiResource->setWifiState(enabled);

            return;
        case turningOff:
            return;
        case disabled:
        case turningOn:
        case enabled:
            DBG("NetworkInterface::" << __func__
                    << ": unexpected disconnect from state "
                    << wifiStateString(wifiResource->getWifiState()));
    }
}

/**
 * Whenever the wifi device is turned on, the NetworkInterface should call this
 * to notify its WifiStateManager. This method acquires the WifiStateManager's 
 * stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiEnabled()
{
    if (invalidWifiState())
    {

        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");

        return;
    }
    const ScopedLock wifiStateLock(*wifiLock);
    DBG("NetworkInterface::" << __func__ << ": wifi enabled");
    wifiResource->stopTimer();
    wifiResource->setWifiState(enabled);
}

/**
 * Whenever the wifi device is turned off, the NetworkInterface should call this
 * to notify its WifiStateManager. This method acquires the WifiStateManager's 
 * stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiDisabled()
{
    if (invalidWifiState())
    {

        DBG("NetworkInterface::" << __func__ << ": wifi state manager is "
                << " missing or in an invalid state.");

        return;
    }
    const ScopedLock wifiStateLock(*wifiLock);
    DBG("NetworkInterface::" << __func__ << ": wifi disabled");
    wifiResource->stopTimer();
    wifiResource->setWifiState(disabled);
}

/**
 * @return true if the state manager is missing or in an invalid state.
 */
bool WifiStateManager::NetworkInterface::invalidWifiState()
{
    if (wifiResource != nullptr && wifiLock != nullptr)
    {
        const ScopedLock wifiStateLock(*wifiLock);
        WifiState state = wifiResource->getWifiState();
        return state == noStateManager
                || state == missingNetworkInterface;
    }
    return true;
}

WifiStateManager::WifiResource::WifiResource(CriticalSection& timerLock) :
timerLock(timerLock) { }

WifiStateManager::WifiResource::~WifiResource()
{
    if (networkInterface != nullptr)
    {
        networkInterface->wifiResource = nullptr;
    }
}

/**
 * Whenever a wifi operation is attempted, the timer is set to
 * the appropriate timeout period. If the timer goes off before 
 * the NetworkInterface responds, the WifiStateManager will
 * assume that the wifi operation failed.
 */
void WifiStateManager::WifiResource::timerCallback()
{
    const ScopedLock lock(timerLock);
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
void WifiStateManager::WifiResource::windowFocusLost()
{
    stopTimer();
}

/**
 * When window focus is gained, if the network interface has been assigned,
 * the wifi state will be updated.
 */
void WifiStateManager::WifiResource::windowFocusGained()
{
    if (networkInterface != nullptr)
    {
        networkInterface->confirmWifiState();
    }
}

/**
 * Update the current wifi state and notify all listeners.
 */
void WifiStateManager::WifiResource::setWifiState(WifiState state)
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
        for (WifiStateManager::Listener* listener : listeners)
        {
            MessageManager::callAsync([listener, state]()
            {
                listener->wifiStateChanged(state);
            });
        }
    }
}

/**
 * Checks the validity of the current wifi state, possibly changing the
 * state to missingNetworkInterface if necessary.
 */
bool WifiStateManager::WifiResource::invalidWifiState()
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

/**
 * Assigns a NetworkInterface to this WifiStateManager. The state manager will 
 * take ownership of the network  interface, deleting it on destruction or when 
 * a new interface is provided through setNetworkInterface.
 */
void WifiStateManager::WifiResource::setNetworkInterface
(NetworkInterface* interface)
{

    networkInterface = interface;
}

/**
 * Gets the current state of the wifi device.
 */
WifiStateManager::WifiState WifiStateManager::WifiResource::getWifiState()
{

    return wifiState;
}

/**
 * Add a listener to the list of objects receiving updates whenever Wifi state 
 * changes.
 */
void WifiStateManager::WifiResource::addListener
(WifiStateManager::Listener* listener)
{

    listeners.add(listener);
}

/**
 * Searches the list of registered listeners for a particular one, and removes
 * it if it's found. 
 */
void WifiStateManager::WifiResource::removeListener
(WifiStateManager::Listener* listener)
{

    listeners.removeAllInstancesOf(listener);
}

/**
 * Gets the connected wifi access point, if one exists.
 */
WifiAccessPoint WifiStateManager::WifiResource::getConnectedAP()
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
 * Gets the connecting wifi access point, if one exists.
 */
WifiAccessPoint WifiStateManager::WifiResource::getConnectingAP()
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
 * Gets all access points visible to the wifi device.
 */
Array<WifiAccessPoint> WifiStateManager::WifiResource::getVisibleAPs()
{
    if (networkInterface == nullptr)
    {
        DBG("WifiStateManager::" << __func__ << ": Can't get visible "
                << "access points, no network interface assigned.");

        return{};
    }
    return networkInterface->getVisibleAPs();
}

/**
 * Checks if wifi is enabled.
 */
bool WifiStateManager::WifiResource::isEnabled()
{
    if (networkInterface == nullptr)
    {

        return false;
    }
    return networkInterface->isWifiEnabled();
}

/**
 * Checks if the wifi device is connected to an access point.
 */
bool WifiStateManager::WifiResource::isConnected()
{
    if (networkInterface == nullptr)
    {

        return false;
    }
    return networkInterface->isWifiConnected();
}

/**
 * Attempts to open a connection to a wifi access point. This will fail if 
 * wifi is disabled, the access point is invalid, or the psk is wrong.
 */
void WifiStateManager::WifiResource::connectToAccessPoint
(const WifiAccessPoint& toConnect, String psk)
{
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
 * access point, that connection will be closed or canceled. 
 */
void WifiStateManager::WifiResource::disconnect
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
 * If wifi is currently disabled, this will enable it.  Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::WifiResource::enableWifi()
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
 * If wifi is currently enabled, this will disable it. Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::WifiResource::disableWifi()
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


