#include "NetworkInterface.h"
#include "WifiState.h"

/* SharedResource type key */
const juce::Identifier NetworkInterface::resourceKey = "NetworkInterface";

NetworkInterface::NetworkInterface() : SharedResource(resourceKey) { }

NetworkInterface::Listener::Listener() : ResourceHandler<NetworkInterface>
        (NetworkInterface::resourceKey,
         []()->SharedResource* { return nullptr; }) { }
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
                    foreachHandler([this, state](Handler* handler)
                    {
                        
                        Listener* toNotify = dynamic_cast<Listener*>(handler);
                        if(toNotify != nullptr)
                        {
                            toNotify->wifiStateChanged(state);
                        }
                    });
                });
        MessageManager::callAsync(setState);
    }
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
    bool wifiConnecting = isWifiConnecting();
    bool wifiConnected = isWifiConnected();
    WifiState state = getWifiState();
    
    if(!wifiDeviceFound())
    {
        if (state != WifiState::missingNetworkDevice)
        {
            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but should be missingNetworkDevice.");
            setWifiState(WifiState::missingNetworkDevice);
        }
        return;    
    }
    if (!isWifiEnabled())
    {
        if (state != WifiState::turningOn && state != WifiState::disabled)
        {
            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is actually disabled."); 
            setWifiState(WifiState::disabled);
        }
        return;
    }
    if (wifiConnecting)
    {
        if (state != WifiState::connecting && state != WifiState::turningOff)
        {

            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is connecting.");
            setWifiState(WifiState::connecting);
            if (!isTimerRunning())
            {
                startTimer(wifiConnectionTimeout);
            }
        }
    }
    else if (wifiConnected)
    {
        if (state != WifiState::connected && state != WifiState::disconnecting)
        {

            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is connected.");
            setWifiState(WifiState::connected);
            if (!isTimerRunning())
            {
                startTimer(wifiConnectionTimeout);
            }
        }
    }
    else
    {
        if (state != WifiState::enabled && state != WifiState::turningOff)
        {
            DBG("NetworkInterface::" << __func__ << ": state was "
                    << wifiStateString(state)
                    << ", but wifi is enabled/not connected.");
            setWifiState(WifiState::enabled);
        }
    }
}

/*
 * Whenever the wifi device detects a connection being established, the 
 * NetworkInterface should call this to notify its WifiStateManager.
 */
void NetworkInterface::signalWifiConnecting()
{
    WifiState wifiState = getWifiState();
    if (wifiState != WifiState::connecting)
    {
        DBG("NetworkInterface::" << __func__
                << ": started to connect during state "
                << wifiStateString(wifiState));
        startTimer(wifiConnectionTimeout);
        setWifiState(WifiState::connecting);
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
    setWifiState(WifiState::connected);
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
        case WifiState::connecting:
            DBG("NetworkInterface::" << __func__ << ": connection failed.");
            stopTimer();
            setWifiState(WifiState::enabled);
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
    //DBG("NetworkInterface::" << __func__ << ": wifi disconnected");
    stopTimer();
    WifiState currentState = getWifiState();
    
    switch (currentState)
    {
        case WifiState::connecting:
        case WifiState::disconnecting:
        case WifiState::connected:
        case WifiState::missingPassword:
            setWifiState(WifiState::enabled);
            return;
        case WifiState::turningOff:
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
    stopTimer();
    setWifiState(WifiState::enabled);
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
    stopTimer();
    setWifiState(WifiState::disabled);
}

/*
 * When wifi is connecting and a missing psk is required, the 
 * NetworkInterface should call this to notify its WifiStateManager.
 */
void NetworkInterface::signalPskNeeded()
{
    using namespace juce;
    //DBG("NetworkInterface::" << __func__ << ": missing password");
    stopTimer();
    setWifiState(WifiState::missingPassword);
}
     
/*
 * Whenever a new wifi access point is detected, the NetworkInterface
 * should call this to notify its WifiStateManager.
 */
void NetworkInterface::signalAPAdded
(const WifiAccessPoint& addedAP)
{
    using namespace juce;
    std::function<void()> sendSignal = buildAsyncFunction(LockType::write,
            [this, addedAP]
            {
                foreachHandler([this, addedAP](Handler* handler)
                {
                    Listener* toNotify = dynamic_cast<Listener*>(handler);
                    if(toNotify != nullptr && !addedAP.isNull())
                    {
                        toNotify->accessPointAdded(addedAP);
                    }
                });
            });
    MessageManager::callAsync(sendSignal);
}

/*
 * Whenever a wifi access point loses visibility, the NetworkInterface
 * should call this to notify its WifiStateManager.
 */
void NetworkInterface::signalAPRemoved
(const WifiAccessPoint& removedAP)
{
    using namespace juce;
    std::function<void()> sendSignal = buildAsyncFunction(LockType::write,
            [this, removedAP]
            {
                foreachHandler([this, removedAP](Handler* handler)
                {
                    Listener* toNotify = dynamic_cast<Listener*>(handler);
                    if(toNotify != nullptr && !removedAP.isNull())
                    {
                        toNotify->accessPointRemoved(removedAP);
                    }
                });
            });
    MessageManager::callAsync(sendSignal);
}


/*
 * Whenever a wifi operation is attempted, the timer is set to
 * the appropriate timeout period. If the timer goes off before 
 * the NetworkInterface responds, the WifiStateManager will
 * assume that the wifi operation failed.
 */
void NetworkInterface::timerCallback()
{
    stopTimer();
    bool wifiEnabled = isWifiEnabled();
    bool wifiConnected = isWifiConnected();
    WifiState currentState = wifiState;
    switch (currentState)
    {
        case WifiState::connecting:
            DBG("" << __func__
                    << ": wifi connection timed out.");
            setWifiState(WifiState::enabled);
            return;
        case WifiState::disconnecting:
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
                setWifiState(WifiState::enabled);
            }
            return;
        case WifiState::turningOn:
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
                setWifiState(wifiEnabled ?
                        WifiState::enabled : WifiState::disabled);
            }
            return;
        case WifiState::turningOff:
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
                setWifiState(wifiEnabled ?
                        WifiState::enabled : WifiState::disabled);
            }
            return;
        case WifiState::disabled:
        case WifiState::enabled:
        case WifiState::connected:
            DBG("NetworkInterface::" << __func__
                    << ": unexpected timeout from state "
                    << wifiStateString(currentState));
    }
}


