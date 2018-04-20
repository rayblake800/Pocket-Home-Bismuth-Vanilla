#include "WifiStateManager.h"
#include "GLibSignalHandler.h"

ScopedPointer<ResourceManager::SharedResource>
        WifiStateManager::sharedResource = nullptr;

CriticalSection WifiStateManager::stateLock;

WifiStateManager::WifiStateManager
(std::function<ResourceManager::SharedResource*(CriticalSection&)> 
createWifiResource):
ResourceManager(sharedResource, stateLock,
        [&stateLock,&createWifiResource]
        {
            return createWifiResource(stateLock);
        }) { }

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
WifiAccessPoint::Ptr WifiStateManager::getConnectedAP()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->getConnectedAP();
}

/**
 * Gets the connecting wifi access point, if one exists.
 */
WifiAccessPoint::Ptr WifiStateManager::getConnectingAP()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->getConnectingAP();
}

/**
 * Gets all access points visible to the wifi device.
 */
Array<WifiAccessPoint::Ptr> WifiStateManager::getVisibleAPs()
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
void WifiStateManager::connectToAccessPoint(WifiAccessPoint::Ptr toConnect,
        String psk)
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    if (toConnect == nullptr)
    {
        DBG("WifiStateManager::" << __func__
                << ": Tried to connect to null access point!");
        return;
    }
    else if(toConnect == getConnectedAP())
    {
        DBG("WifiStat"
                "eManager::" << __func__
                << ": already connected to " << toConnect->getSSID());
        return;
    }
    else if(toConnect == getConnectingAP())
    {
        DBG("WifiStateManager::" << __func__
                << ": already connecting to " << toConnect->getSSID());
        return;
    }
    
    WifiState wifiState = wifiResource->getWifiState();
    switch (wifiState)
    {
        //start connecting:
        case connecting:
            stopConnecting();
            break;
        case connected:
            DBG("WifiStateManager::" << __func__
                    << ": closing previous connection");
            disconnect();
            break;
        case disconnecting:
        case missingPassword:
        case enabled:
            break;
        default:
            DBG("WifiStateManager::" << __func__ << ": Can't connect to "
                    << toConnect->getSSID() << " from wifi state "
                    << wifiStateString(wifiState));
            return;
    }
    DBG("WifiStateManager::" << __func__ << ": Connecting to access "
            << "point " << toConnect->getSSID());
    wifiResource->setWifiState(connecting);
    wifiResource->connectToAccessPoint(toConnect, psk);
    wifiResource->startTimer(wifiResource->wifiConnectionTimeout);
}

/**
 * If currently connected to an access point, that connection will be closed or 
 * canceled.
 */
void WifiStateManager::disconnect()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    WifiState wifiState = wifiResource->getWifiState();
    switch (wifiState)
    {
        case connected:
            DBG("WifiStateManager::" << __func__ << ": Disconnecting... ");
            wifiResource->setWifiState(disconnecting);
            wifiResource->startTimer(wifiResource->wifiConnectionTimeout);
            wifiResource->disconnect();
            return;
        default:
            DBG("WifiStateManager::" << __func__
                    << ": Attempted to disconnect from state " 
                    << wifiStateString(wifiState));
    }
}


/**
 * If attempting to connect to a wifi access point, that attempted
 * connection will be canceled.
 */
void WifiStateManager::stopConnecting()
{
    const ScopedLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    WifiState wifiState = wifiResource->getWifiState();
    switch (wifiState)
    {
        case connecting:
        case missingPassword:
            DBG("WifiStateManager::" << __func__ << ": Disconnecting... ");
            wifiResource->setWifiState(disconnecting);
            wifiResource->startTimer(wifiResource->wifiConnectionTimeout);
            wifiResource->stopConnecting();
            return;
        default:
            DBG("WifiStateManager::" << __func__
                    << ": Attempted to stop connecting from state " 
                    << wifiStateString(wifiState));
    }
}

/**
 * If wifi is currently disabled, this will enable it.  Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::enableWifi()
{
    if(getWifiState() != missingNetworkDevice)
    {
        const ScopedLock lock(stateLock);
        NetworkInterface* wifiResource
                = static_cast<NetworkInterface*> (sharedResource.get());
        WifiState wifiState = wifiResource->getWifiState();
        switch (wifiState)
        {
            case disabled:
                DBG("WifiStateManager::WifiResource::" << __func__ 
                        << ": enabling wifi");
                wifiResource->setWifiState(turningOn);
                wifiResource->startTimer
                        (wifiResource->wifiEnabledChangeTimeout);
                wifiResource->enableWifi();
                return;
            case turningOn:
                DBG("WifiStateManager::" << __func__
                        << ": wifi is already being enabled");
                return;
            case missingNetworkDevice:
                DBG("WifiStateManager::" << __func__ 
                        << ": wifi device is missing");
            default:
                DBG("WifiStateManager::" << __func__ << ": wifi is already on");
                return;
        }
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
    WifiState wifiState = wifiResource->getWifiState();
    switch (wifiState)
    {
        case turningOff:
            DBG("WifiStateManager::" << __func__
                    << ": wifi is already turning off.");
            return;          
        case disabled:
        case missingNetworkDevice:
            DBG("WifiStateManager::" << __func__
                    << ": wifi is already disabled");
            return;
        default:
            DBG("WifiStateManager::" << __func__ << ": disabling wifi");
            wifiResource->setWifiState(turningOff);
            wifiResource->startTimer(wifiResource->wifiEnabledChangeTimeout);
            wifiResource->disableWifi();
    }
}

WifiStateManager::NetworkInterface::NetworkInterface(CriticalSection& wifiLock)
: wifiLock(wifiLock) { }

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
            WifiStateManager::Listener* toNotify
                    = notifyQueue.removeAndReturn(notifyQueue.size() - 1);
            const ScopedUnlock signalUnlock(wifiLock);
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
    //DBG("NetworkInterface::" << __func__ << ": state was "
    //        << wifiStateString(state)
    //        << ", checking if that is still accurate");
    
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
    if (wifiConnecting)
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
        }
    }
    else if (wifiConnected)
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
    }
    else
    {
        if (state != enabled && state != turningOff)
        {
            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is enabled/not connected.");
            setWifiState(enabled);
        }
    }
}

/**
 * Whenever the wifi device detects a connection being established, the 
 * NetworkInterface should call this to notify its WifiStateManager.
 */
void WifiStateManager::NetworkInterface::signalWifiConnecting()
{
    const ScopedLock updateLock(wifiLock);
    WifiState wifiState = getWifiState();
    if (wifiState != connecting)
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
(WifiAccessPoint::Ptr connectedAP)
{
    DBG("NetworkInterface::" << __func__ << ": connected to "
            << connectedAP->getSSID() << " during state "
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
                    << wifiStateString(getWifiState()));
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
    //DBG("NetworkInterface::" << __func__ << ": wifi disconnected");
    stopTimer();
    WifiState currentState = getWifiState();
    const ScopedUnlock stateUnlock(wifiLock);
    
    switch (currentState)
    {
        case connecting:
        case disconnecting:
        case connected:
        case missingPassword:
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

    //DBG("NetworkInterface::" << __func__ << ": wifi enabled");
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
    //DBG("NetworkInterface::" << __func__ << ": wifi disabled");
    setWifiState(disabled);
    const ScopedLock timerLock(wifiLock);
    stopTimer();
}

        
/**
 * When wifi is connecting and a missing psk is required, the 
 * NetworkInterface should call this to notify its WifiStateManager.
 */
void WifiStateManager::NetworkInterface::signalPskNeeded()
{
    //DBG("NetworkInterface::" << __func__ << ": missing password");
    setWifiState(missingPassword);
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
