#include "NetworkInterface.h"


NetworkInterface::NetworkInterface(
        juce::ScopedPointer<SharedResource>& instanceHolder,
        juce::ReadWriteLock& wifiLock) : 
SharedResource(instanceHolder, wifiLock) { }

/*
 * Gets the current state of the wifi device.
 */
WifiState NetworkInterface::getWifiState()
{
    return wifiState;
}


/*
 * Update the current wifi state and notify all listeners.
 */
void NetworkInterface::setWifiState(WifiState state)
{    
    using namespace juce;
    if (state != wifiState)
    {
        DBG("setWifiState: Setting wifi state to "
                << wifiStateString(wifiState));
        wifiState = state;
        std::function<void()> setState = buildAsyncFunction(
                SharedResource::write, [this, state]()
                {
                    jassert(notifyQueue.isEmpty());
                    notifyQueue = listeners;
                    while(!notifyQueue.isEmpty())
                    {
                        Listener* toNotify = notifyQueue
                                .removeAndReturn(notifyQueue.size() - 1);
                        toNotify->wifiStateChanged(state);
                    }
                });
        MessageManager::callAsync(setState);
    }
}

/*
 * Add a listener to the list of objects receiving updates whenever Wifi state 
 * changes.
 */
void NetworkInterface::addListener
(NetworkInterface::Listener* listener)
{
    listeners.add(listener);
}

/*
 * Searches the list of registered listeners for a particular one, and removes
 * it if it's found. 
 */
void NetworkInterface::removeListener
(Listener* listener)
{
    listeners.removeAllInstancesOf(listener);
}


/*
 * On destruction, listeners will remove themselves from
 * their NetworkInterface if necessary.
 */
NetworkInterface::Listener::~Listener()
{
    auto networkInterface = 
    WifiStateManager stateManager;
    stateManager.removeListener(this);
}

/*
 * This method queries the wifi device to ensure that the current tracked wifi 
 * state actually matches the device state.  If necessary, this will update the 
 * WifiStateManager's current state.  The NetworkInterface should call this 
 * method whenever it has reason to believe the wifi state may be out of date.  
 */
void NetworkInterface::confirmWifiState()
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
void NetworkInterface::signalWifiConnecting()
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
 * WifiStateManager's wifiLock.
 */
void NetworkInterface::signalWifiConnected()
{
    DBG("NetworkInterface::" << __func__ 
            << ": connection established during state "
            << wifiStateString(getWifiState()));
    setWifiState(connected);
}

/*
 * Whenever the wifi device fails to connect to an access point, the 
 * NetworkInterface should call this to  notify its WifiStateManager. This 
 * method acquires the WifiStateManager's wifiLock.
 */
void NetworkInterface::signalConnectionFailed()
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
 * This method acquires the WifiStateManager's wifiLock.
 */
void NetworkInterface::signalWifiDisconnected()
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
 * wifiLock.
 */
void NetworkInterface::signalWifiEnabled()
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
 * wifiLock.
 */
void NetworkInterface::signalWifiDisabled()
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
void NetworkInterface::signalPskNeeded()
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
void NetworkInterface::signalAPAdded
(const WifiAccessPoint& addedAP)
{
    using namespace juce;
    MessageManager::callAsync([this, addedAP]()
    {
        WifiStateManager stateManager;
        if(stateManager.validNetworkInterface(this))
        {
            wifiLock.enterWrite();
            if (!addedAP.isNull())
            {
                jassert(notifyQueue.isEmpty());
                notifyQueue = listeners;
                while(this == WifiStateManager::sharedResource.get()
                        && !notifyQueue.isEmpty())
                {
                    Listener* toNotify = notifyQueue
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
void NetworkInterface::signalAPRemoved
(const WifiAccessPoint& removedAP)
{
    using namespace juce;
    MessageManager::callAsync([this, removedAP]()
    {
        WifiStateManager stateManager;
        if(stateManager.validNetworkInterface(this))
        {
            wifiLock.enterWrite();
            if (!removedAP.isNull())
            {
                jassert(notifyQueue.isEmpty());
                notifyQueue = listeners;
                while(this == WifiStateManager::sharedResource.get()
                        && !notifyQueue.isEmpty())
                {
                    Listener* toNotify = notifyQueue
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
void NetworkInterface::timerCallback()
{
    WifiStateManager stateManager;
    if(stateManager.validNetworkInterface(this))
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
                DBG("" << __func__
                        << ": wifi connection timed out.");
                setWifiState(enabled);
                return;
            case disconnecting:
                if (wifiConnected)
                {
                    DBG("" << __func__
                          << ": disconnecting failed, this shouldn't"
                          << " be possible!");
                }
                else
                {
                    DBG("" << __func__
                        << ": finished disconnecting, but no signal was "
                        << "received");
                    setWifiState(enabled);
                }
                return;
            case turningOn:
                if (wifiEnabled)
                {
                    DBG("" << __func__
                            << ": failed to enable wifi!");
                }
                else
                {
                    DBG("" << __func__
                       << ": finished turning on wifi, but no signal was "
                       << "received");
                    setWifiState(wifiEnabled ? enabled : disabled);
                }
                return;
            case turningOff:
                if (wifiEnabled)
                {
                    DBG("" << __func__
                            << ": failed to disable wifi!");
                }
                else
                {

                    DBG("" << __func__
                      << ": finished turning off wifi, but no signal was "
                      << "received");
                    setWifiState(wifiEnabled ? enabled : disabled);
                }
                return;
            case disabled:
            case enabled:
            case connected:
                DBG("NetworkInterface::" << __func__
                        << ": unexpected timeout from state "
                        << wifiStateString(currentState));
        }
    }
}


