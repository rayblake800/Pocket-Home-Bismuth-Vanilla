#include "WifiStateManager.h"

//ResourceManager shared object and lock;
static juce::ScopedPointer<NetworkInterface> networkInterface;
static juce::ReadWriteLock stateLock;

WifiStateManager::WifiStateManager
(std::function<NetworkInterface*()> createWifiResource) :
SharedResource::Handler<NetworkInterface>(NetworkInterface::resourceKey,
        [&createWifiResource]()
        {
            return createWifiResource();
        }) { }

/*
 * Gets the current state of the wifi device.
 */
WifiState WifiStateManager::getWifiState()
{
    auto wifiInterface = getReadLockedResource();
    return wifiInterface->getWifiState();
}

/*
 * Gets the connected or connecting wifi access point.
 */
WifiAccessPoint WifiStateManager::getActiveAP()
{
    
    auto wifiInterface = getReadLockedResource();
    return wifiInterface->getActiveAP();
}

/*
 * Gets all access points visible to the wifi device.
 */
juce::Array<WifiAccessPoint> WifiStateManager::getVisibleAPs()
{
    auto wifiInterface = getReadLockedResource();
    return wifiInterface->getVisibleAPs();
}

/*
 * Checks if wifi is enabled.
 */
bool WifiStateManager::isEnabled()
{
    auto wifiInterface = getReadLockedResource();
    return wifiInterface->isWifiEnabled();
}

/*
 * Checks if the wifi device is connected to an access point.
 */
bool WifiStateManager::isConnected()
{
    auto wifiInterface = getReadLockedResource();
    return wifiInterface->isWifiConnected();
}

/*
 * Asynchronously scans nearby wifi access points, updating the access point 
 * list.
 */
void WifiStateManager::scanAccessPoints()
{
    auto networkInterface = getWriteLockedResource();
    networkInterface->scanAccessPoints();
}



/*
 * Attempts to open a connection to a wifi access point. This will fail if 
 * wifi is disabled, the access point is invalid, or the psk is wrong.
 */
void WifiStateManager::connectToAccessPoint(const WifiAccessPoint& toConnect,
        juce::String psk)
{
    using namespace juce;
    auto wifiInterface = getWriteLockedResource();
    switch(getAPState(toConnect))
    {
        case AccessPointState::nullAP:
            DBG("WifiStateManager::" << __func__
                    << ": Tried to connect to null access point!");
            return;
        case AccessPointState::missingAP:
            DBG("WifiStateManager::" << __func__
                    << ": Access point " << toConnect.getSSID()
                    << "is no longer visible");
            return;
        case AccessPointState::connectedAP:
            DBG("WifiStateManager::" << __func__
                    << ": already connected to " << toConnect.getSSID());
            return;
        case AccessPointState::connectingAP:
            DBG("WifiStateManager::" << __func__
                    << ": already connecting to " << toConnect.getSSID());
            return;
    }
    
    WifiState wifiState = wifiInterface->getWifiState();
    switch (wifiState)
    {
        case WifiState::connecting:
        case WifiState::connected:
            DBG("WifiStateManager::" << __func__
                    << ": closing previous connection");
            disconnect();
            break;
        case WifiState::disconnecting:
        case WifiState::missingPassword:
        case WifiState::enabled:
            break;
        default:
            DBG("WifiStateManager::" << __func__ << ": Can't connect to "
                    << toConnect.getSSID() << " from wifi state "
                    << wifiStateString(wifiState));
            return;
    }
    DBG("WifiStateManager::" << __func__ << ": Connecting to access "
            << "point " << toConnect.getSSID());
    wifiInterface->connectToAccessPoint(toConnect, psk);
    wifiInterface->startTimer(wifiInterface->wifiConnectionTimeout);
}

/*
 * If currently connected to an access point, that connection will be closed or 
 * canceled.
 */
void WifiStateManager::disconnect()
{
    using namespace juce;
    auto wifiInterface = getWriteLockedResource();
    WifiState wifiState = wifiInterface->getWifiState();
    switch (wifiState)
    {
        case WifiState::connecting:
        case WifiState::missingPassword:
        case WifiState::connected:
            DBG("WifiStateManager::" << __func__ << ": Disconnecting... ");
            wifiInterface->setWifiState(WifiState::disconnecting);
            wifiInterface->startTimer(wifiInterface->wifiConnectionTimeout);
            wifiInterface->disconnect();
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
    if(getWifiState() != WifiState::missingNetworkDevice)
    {
        auto wifiInterface = getWriteLockedResource();
        WifiState wifiState = wifiInterface->getWifiState();
        switch (wifiState)
        {
            case WifiState::disabled:
                DBG("WifiStateManager::" << __func__ 
                        << ": enabling wifi");
                wifiInterface->setWifiState(WifiState::turningOn);
                wifiInterface->startTimer
                        (wifiInterface->wifiEnabledChangeTimeout);
                wifiInterface->enableWifi();
                return;
            case WifiState::turningOn:
                DBG("WifiStateManager::" << __func__
                        << ": wifi is already being enabled");
                return;
            case WifiState::missingNetworkDevice:
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
    auto wifiInterface = getWriteLockedResource();
    WifiState wifiState = wifiInterface->getWifiState();
    switch (wifiState)
    {
        case WifiState::turningOff:
            DBG("WifiStateManager::" << __func__
                    << ": wifi is already turning off.");
            return;          
        case WifiState::disabled:
        case WifiState::missingNetworkDevice:
            DBG("WifiStateManager::" << __func__
                    << ": wifi is already disabled");
            return;
        default:
            DBG("WifiStateManager::" << __func__ << ": disabling wifi");
            wifiInterface->setWifiState(WifiState::turningOff);
            wifiInterface->startTimer(wifiInterface->wifiEnabledChangeTimeout);
            wifiInterface->disableWifi();
    }
}
 
/*
 * Finds the current network state of an access point object.
 */
AccessPointState WifiStateManager::getAPState
(const WifiAccessPoint& accessPoint)
{
    using namespace juce;
    if(accessPoint.isNull())
    {
        return AccessPointState::nullAP;
    }
    auto wifiInterface = getWriteLockedResource();
    return wifiInterface->getAPState(accessPoint);
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
    if(getAPState(accessPoint) == AccessPointState::connectedAP)
    {
        return Time::getCurrentTime();
    }
    auto wifiInterface = getReadLockedResource();
    return wifiInterface->lastConnectionTime(accessPoint);
}
