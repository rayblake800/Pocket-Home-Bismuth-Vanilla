#include "WifiStateManager.h"
#include "GLibSignalThread.h"

juce::ScopedPointer<ResourceManager::SharedResource>
        WifiStateManager::sharedResource = nullptr;

juce::ReadWriteLock WifiStateManager::stateLock;

WifiStateManager::WifiStateManager
(std::function<ResourceManager::SharedResource*(juce::ReadWriteLock&)> 
createWifiResource):
ResourceManager(sharedResource, stateLock,
        [&createWifiResource]
        {
            return createWifiResource(WifiStateManager::stateLock);
        }) { }

/*
 * Gets the current state of the wifi device.
 */
WifiStateManager::WifiState WifiStateManager::getWifiState()
{
    using namespace juce;
    const ScopedReadLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->getWifiState();
}


/*
 * Add a listener to the list of objects receiving updates whenever Wifi 
 * state changes.
 */
void WifiStateManager::addListener(WifiStateManager::Listener* listener)
{
    using namespace juce;
    const ScopedWriteLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    wifiResource->addListener(listener);
}

/*
 * Searches the list of registered listeners for a particular one, and
 * removes it if it's found. 
 */
void WifiStateManager::removeListener(WifiStateManager::Listener* listener)
{
    using namespace juce;
    const ScopedWriteLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    wifiResource->removeListener(listener);
}

/*
 * Gets the connected or connecting wifi access point.
 */
WifiAccessPoint WifiStateManager::getActiveAP()
{
    using namespace juce;
    const ScopedReadLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->getActiveAP();
}

/*
 * Gets all access points visible to the wifi device.
 */
juce::Array<WifiAccessPoint> WifiStateManager::getVisibleAPs()
{
    using namespace juce;
    const ScopedReadLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->getVisibleAPs();
}

/*
 * Checks if wifi is enabled.
 */
bool WifiStateManager::isEnabled()
{
    using namespace juce;
    const ScopedReadLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->isWifiEnabled();
}

/*
 * Checks if the wifi device is connected to an access point.
 */
bool WifiStateManager::isConnected()
{
    using namespace juce;
    const ScopedReadLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->isWifiConnected();
}

/*
 * Attempts to open a connection to a wifi access point. This will fail if 
 * wifi is disabled, the access point is invalid, or the psk is wrong.
 */
void WifiStateManager::connectToAccessPoint(const WifiAccessPoint& toConnect,
        juce::String psk)
{
    using namespace juce;
    const ScopedWriteLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    switch(getAPState(toConnect))
    {
        case nullAP:
            DBG("WifiStateManager::" << __func__
                    << ": Tried to connect to null access point!");
            return;
        case missingAP:
            DBG("WifiStateManager::" << __func__
                    << ": Access point " << toConnect.getSSID()
                    << "is no longer visible");
            return;
        case connectedAP:
            DBG("WifiStateManager::" << __func__
                    << ": already connected to " << toConnect.getSSID());
            return;
        case connectingAP:
            DBG("WifiStateManager::" << __func__
                    << ": already connecting to " << toConnect.getSSID());
            return;
    }
    
    WifiState wifiState = wifiResource->getWifiState();
    switch (wifiState)
    {
        case connecting:
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
                    << toConnect.getSSID() << " from wifi state "
                    << wifiStateString(wifiState));
            return;
    }
    DBG("WifiStateManager::" << __func__ << ": Connecting to access "
            << "point " << toConnect.getSSID());
    wifiResource->connectToAccessPoint(toConnect, psk);
    wifiResource->startTimer(wifiResource->wifiConnectionTimeout);
}

/*
 * If currently connected to an access point, that connection will be closed or 
 * canceled.
 */
void WifiStateManager::disconnect()
{
    using namespace juce;
    const ScopedWriteLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    WifiState wifiState = wifiResource->getWifiState();
    switch (wifiState)
    {
	case connecting:
        case missingPassword:
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

/*
 * If wifi is currently disabled, this will enable it.  Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::enableWifi()
{
    using namespace juce;
    if(getWifiState() != missingNetworkDevice)
    {
        const ScopedWriteLock lock(stateLock);
        NetworkInterface* wifiResource
                = static_cast<NetworkInterface*> (sharedResource.get());
        WifiState wifiState = wifiResource->getWifiState();
        switch (wifiState)
        {
            case disabled:
                DBG("WifiStateManager::" << __func__ 
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

/*
 * If wifi is currently enabled, this will disable it. Otherwise, nothing 
 * will happen.
 */
void WifiStateManager::disableWifi()
{
    using namespace juce;
    const ScopedWriteLock lock(stateLock);
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
 
/*
 * Finds the current network state of an access point object.
 */
WifiStateManager::AccessPointState WifiStateManager::getAPState
(const WifiAccessPoint& accessPoint)
{
    using namespace juce;
    if(accessPoint.isNull())
    {
        return nullAP;
    }
    const ScopedReadLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->getAPState(accessPoint);
}
     
/*
 * Finds the last time a connection was active using a specific access
 * point.
 */
juce::Time WifiStateManager::lastConnectionTime
(const WifiAccessPoint& accessPoint)
{
    using namespace juce;
    if(accessPoint.isNull())
    {
        return Time();
    }
    if(getAPState(accessPoint) == connectedAP)
    {
        return Time::getCurrentTime();
    }
    const ScopedReadLock lock(stateLock);
    NetworkInterface* wifiResource
            = static_cast<NetworkInterface*> (sharedResource.get());
    return wifiResource->lastConnectionTime(accessPoint);
}

WifiStateManager::NetworkInterface::NetworkInterface
(juce::ReadWriteLock& wifiLock) : wifiLock(wifiLock) { }

/*
 * Gets the current state of the wifi device.
 */
WifiStateManager::WifiState WifiStateManager::NetworkInterface::getWifiState()
{
    return wifiState;
}


/*
 * Update the current wifi state and notify all listeners.
 */
void WifiStateManager::NetworkInterface::setWifiState(WifiState state)
{    
    using namespace juce;
    if (state != wifiState)
    {
        wifiLock.enterWrite();
        DBG("WifiStateManager::setWifiState: Setting wifi state to "
                << wifiStateString(wifiState));
        wifiState = state;
        wifiLock.exitWrite();
        MessageManager::callAsync([this,state]()
        {
            wifiLock.enterWrite();
            //make sure the sharedResource wasn't destroyed.
            if(this == WifiStateManager::sharedResource.get())
            {
                jassert(notifyQueue.isEmpty());
                notifyQueue = listeners;
                while(this == WifiStateManager::sharedResource.get()
                        && !notifyQueue.isEmpty())
                {
                    WifiStateManager::Listener* toNotify = notifyQueue
                            .removeAndReturn(notifyQueue.size() - 1);
                    wifiLock.exitWrite();
                    toNotify->wifiStateChanged(state);
                    wifiLock.enterWrite();
                }
            }
            wifiLock.exitWrite();
        });
    }
}

/*
 * Add a listener to the list of objects receiving updates whenever Wifi state 
 * changes.
 */
void WifiStateManager::NetworkInterface::addListener
(WifiStateManager::Listener* listener)
{
    listeners.add(listener);
}

/*
 * Searches the list of registered listeners for a particular one, and removes
 * it if it's found. 
 */
void WifiStateManager::NetworkInterface::removeListener
(WifiStateManager::Listener* listener)
{

    listeners.removeAllInstancesOf(listener);
}


/*
 * On destruction, listeners will remove themselves from
 * their WifiStateManager if necessary.
 */
WifiStateManager::Listener::~Listener()
{
    WifiStateManager stateManager;
    stateManager.removeListener(this);
}

/*
 * This method queries the wifi device to ensure that the current tracked wifi 
 * state actually matches the device state.  If necessary, this will update the 
 * WifiStateManager's current state.  The NetworkInterface should call this 
 * method whenever it has reason to believe the wifi state may be out of date.  
 */
void WifiStateManager::NetworkInterface::confirmWifiState()
{
    using namespace juce;
    wifiLock.enterRead();
    bool wifiConnecting = isWifiConnecting();
    bool wifiConnected = isWifiConnected();
    WifiState state = getWifiState();
    wifiLock.exitRead();
    
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
            const ScopedWriteLock timerLock(wifiLock);
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
            const ScopedWriteLock timerLock(wifiLock);
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

/*
 * Whenever the wifi device detects a connection being established, the 
 * NetworkInterface should call this to notify its WifiStateManager.
 */
void WifiStateManager::NetworkInterface::signalWifiConnecting()
{
    wifiLock.enterWrite();
    WifiState wifiState = getWifiState();
    if (wifiState != connecting)
    {
        DBG("NetworkInterface::" << __func__
                << ": started to connect during state "
                << wifiStateString(wifiState));
        startTimer(wifiConnectionTimeout);
        wifiLock.exitWrite();
        setWifiState(connecting);
    }
    else
    {
        wifiLock.exitWrite();
    }
}

/*
 * Whenever the wifi device establishes a new connection, the NetworkInterface 
 * should call this to notify its WifiStateManager. This method acquires the 
 * WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiConnected()
{
    DBG("NetworkInterface::" << __func__ 
            << ": connection established during state "
            << wifiStateString(getWifiState()));
    setWifiState(connected);
}

/*
 * Whenever the wifi device fails to connect to an access point, the 
 * NetworkInterface should call this to  notify its WifiStateManager. This 
 * method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalConnectionFailed()
{
    using namespace juce;
    switch (getWifiState())
    {
        case connecting:
            DBG("NetworkInterface::" << __func__ << ": connection failed.");
            {
                const ScopedWriteLock timerLock(wifiLock);
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

/*
 * Whenever the wifi device disconnects from a wifi access point, the 
 * NetworkInterface should call this to notify its WifiStateManager.
 * This method acquires the WifiStateManager's stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiDisconnected()
{
    wifiLock.enterWrite();
    //DBG("NetworkInterface::" << __func__ << ": wifi disconnected");
    stopTimer();
    WifiState currentState = getWifiState();
    wifiLock.exitWrite();
    
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

/*
 * Whenever the wifi device is turned on, the NetworkInterface should call this
 * to notify its WifiStateManager. This method acquires the WifiStateManager's 
 * stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiEnabled()
{
    using namespace juce;
    //DBG("NetworkInterface::" << __func__ << ": wifi enabled");
    setWifiState(enabled);
    const ScopedWriteLock timerLock(wifiLock);
    stopTimer();
}

/*
 * Whenever the wifi device is turned off, the NetworkInterface should call this
 * to notify its WifiStateManager. This method acquires the WifiStateManager's 
 * stateLock.
 */
void WifiStateManager::NetworkInterface::signalWifiDisabled()
{
    using namespace juce;
    //DBG("NetworkInterface::" << __func__ << ": wifi disabled");
    setWifiState(disabled);
    const ScopedWriteLock timerLock(wifiLock);
    stopTimer();
}

        
/*
 * When wifi is connecting and a missing psk is required, the 
 * NetworkInterface should call this to notify its WifiStateManager.
 */
void WifiStateManager::NetworkInterface::signalPskNeeded()
{
    using namespace juce;
    //DBG("NetworkInterface::" << __func__ << ": missing password");
    setWifiState(missingPassword);
    const ScopedWriteLock timerLock(wifiLock);
    stopTimer();
}
     
/*
 * Whenever a new wifi access point is detected, the NetworkInterface
 * should call this to notify its WifiStateManager.
 */
void WifiStateManager::NetworkInterface::signalAPAdded
(const WifiAccessPoint& addedAP)
{
    using namespace juce;
    MessageManager::callAsync([this, addedAP]()
    {
        wifiLock.enterWrite();
        if(this == WifiStateManager::sharedResource.get())
        {
            if (!addedAP.isNull())
            {
                jassert(notifyQueue.isEmpty());
                notifyQueue = listeners;
                while(this == WifiStateManager::sharedResource.get()
                        && !notifyQueue.isEmpty())
                {
                    WifiStateManager::Listener* toNotify = notifyQueue
                            .removeAndReturn(notifyQueue.size() - 1);
                    wifiLock.exitWrite();
                    toNotify->accessPointAdded(addedAP);
                    wifiLock.enterWrite();
                }
            }
        }
        wifiLock.exitWrite();
    });
}

/*
 * Whenever a wifi access point loses visibility, the NetworkInterface
 * should call this to notify its WifiStateManager.
 */
void WifiStateManager::NetworkInterface::signalAPRemoved
(const WifiAccessPoint& removedAP)
{
    using namespace juce;
    MessageManager::callAsync([this, removedAP]()
    {
        wifiLock.enterWrite();
        if(this == WifiStateManager::sharedResource.get())
        {
            if (!removedAP.isNull())
            {
                jassert(notifyQueue.isEmpty());
                notifyQueue = listeners;
                while(this == WifiStateManager::sharedResource.get()
                        && !notifyQueue.isEmpty())
                {
                    WifiStateManager::Listener* toNotify = notifyQueue
                            .removeAndReturn(notifyQueue.size() - 1);
                    wifiLock.exitWrite();
                    toNotify->accessPointRemoved(removedAP);
                    wifiLock.enterWrite();
                }
            }
        }
        wifiLock.exitWrite();
    });
}

/*
 * Whenever a wifi operation is attempted, the timer is set to
 * the appropriate timeout period. If the timer goes off before 
 * the NetworkInterface responds, the WifiStateManager will
 * assume that the wifi operation failed.
 */
void WifiStateManager::NetworkInterface::timerCallback()
{
    wifiLock.enterWrite();
    stopTimer();
    bool wifiEnabled = isWifiEnabled();
    bool wifiConnected = isWifiConnected();
    WifiState currentState = wifiState;
    wifiLock.exitWrite();
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
