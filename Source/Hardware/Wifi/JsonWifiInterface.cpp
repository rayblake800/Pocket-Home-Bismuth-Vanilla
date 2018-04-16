#include <map>
#include "JuceHeader.h"
#include "AssetFiles.h"
#include "JsonWifiInterface.h"
#include "TempTimer.h"

JsonWifiInterface::JsonWifiInterface(CriticalSection& wifiLock) : 
WifiStateManager::NetworkInterface(wifiLock),
wifiLock(wifiLock) 
{
    auto json = JSON::parse(AssetFiles::findAssetFile("wifi.json"));

    for (const auto &apJson : *json.getArray())
    {
        visibleAPs.add(new WifiAccessPoint(
                apJson["name"],
                apJson["strength"],
                apJson["auth"],
                apJson["name"]));
    }
    confirmWifiState();
}

JsonWifiInterface::~JsonWifiInterface() { }


bool JsonWifiInterface::wifiDeviceFound()
{
    return true;
}

/**
 * Checks if the simulated wifi device is enabled.
 */
bool JsonWifiInterface::isWifiEnabled()
{
    ScopedLock lock(wifiLock);
    return enabled;
}

/**
 * Checks if a connection event is currently being simulated.
 */
bool JsonWifiInterface::isWifiConnecting()
{
    ScopedLock lock(wifiLock);
    return waitingToConnect != nullptr;
}

/**
 *  Checks if a simulated connection has been created.
 */
bool JsonWifiInterface::isWifiConnected()
{
    ScopedLock lock(wifiLock);
    return connected;
}

        
/**
 * Checks if the simulated connection requires a psk
 */
bool JsonWifiInterface::isPSKNeeded()
{ 
    ScopedLock lock(wifiLock);
    return waitingToConnect->getRequiresAuth();
}

/**
 *  Returns the connected access point.
 */
WifiAccessPoint::Ptr JsonWifiInterface::getConnectedAP()
{
    ScopedLock lock(wifiLock);
    if (!connected)
    {
        return nullptr;
    }
    return connectedAP;
}

/**
 * Returns the connecting access point.
 */
WifiAccessPoint::Ptr JsonWifiInterface::getConnectingAP()
{
    ScopedLock lock(wifiLock);
    return waitingToConnect;
}

/**
 * Returns the entire list of access points defined in wifi.json.
 */
Array<WifiAccessPoint::Ptr> JsonWifiInterface::getVisibleAPs()
{
    Array<WifiAccessPoint::Ptr> accessPoints;
    if (isWifiEnabled())
    {
        return visibleAPs;
    }
    return {};
}

/**
 * Triggers a wifi connection event.  This will set a timer to simulate
 * the connection's success or failure after a randomized delay of nine to
 * twenty-four seconds.
 */
void JsonWifiInterface::connectToAccessPoint(WifiAccessPoint::Ptr toConnect,
        String psk)
{
    ScopedLock lock(wifiLock);
    if(toConnect == nullptr)
    {
        DBG("JsonWifiInterface::" << __func__
                << ": can't connect to null access point");
        return;
        
    }
    DBG("JsonWifiInterface::" << __func__ << ": trying to connect to "
            << toConnect->getSSID());
    if (turningOff)
    {
        DBG("JsonWifiInterface::" << __func__
                << ": can't connect, wifi is turning off");
    }
    else if (turningOn)
    {
        DBG("JsonWifiInterface::" << __func__
                << ": can't connect, wifi still turning on");
    }
    else if (!enabled)
    {
        DBG("JsonWifiInterface::" << __func__ 
                << ": can't connect, wifi isn't on");
    }
    else if (waitingToConnect != nullptr)
    {
        if (waitingToConnect != toConnect)
        {
            DBG("JsonWifiInterface::" << __func__
                    << ": canceling, a connection to " <<
                    waitingToConnect->getSSID() << " is pending ");
        }
        else
        {
            DBG("JsonWifiInterface::" << __func__ << ": already connecting to "
                    << toConnect->getSSID());
        }
    }
    else if (connected)
    {
        DBG("JsonWifiInterface::" << __func__ << ": already connected to "
                << toConnect->getSSID());
    }
    else
    {
        //stop the connection timeout timer, since there are no simulated
        //connection steps generated to reset it
        stopTimer();
        waitingToConnect = toConnect;
        // try to connect to ap, dispatch events on success and failure
        TempTimer::initTimer(Random().nextInt(15000) + 9000,
                [this, psk]()
                {
                    ScopedLock lock(wifiLock);
                    bool isTestCred = (waitingToConnect->getSSID() == "MyFi");
                    if (!isTestCred)
                    {
                        DBG("JsonWifiInterface::" << __func__
                                << ": failed to connect");
                                connected = false;
                                waitingToConnect = nullptr;
                                signalConnectionFailed();
                        return;
                    }

                    if (!psk.isEmpty())
                    {
                        DBG("JsonWifiInterface::" << __func__
                                << ": connected with psk");
                    }
                    else
                    {
                        DBG("JsonWifiInterface::" << __func__ 
                                << ": missing psk! (any is valid)");
                        signalPskNeeded();
                        return;
                    }
                    connected = true;
                    connectedAP = waitingToConnect;
                    waitingToConnect = nullptr;
                    if(connectedAP == nullptr)
                    {
                        signalWifiDisconnected();
                    }
                    else
                    {
                        signalWifiConnected(connectedAP);
                    }                
                });
    }
}


/**
 * If a connection is pending, cancel it.
 */
void JsonWifiInterface::stopConnecting()
{
    ScopedLock lock(wifiLock);
    if(waitingToConnect == nullptr)
    {
         DBG("JsonWifiInterface::" << __func__ << ": no connection to cancel");
    }
    else
    {
        waitingToConnect = nullptr;
    }
}

/**
 * Triggers a simulated wifi disconnection event.  If a simulated connection
 * exists, after a randomized delay of no more than six seconds, a 
 * disconnection event will trigger.
 */
void JsonWifiInterface::disconnect()
{
    ScopedLock lock(wifiLock);
    if (!connected)
    {
        DBG("JsonWifiInterface::" << __func__ << ": no connection to kill");
    }
    else
    {
        TempTimer::initTimer(Random().nextInt(6000), [this]()
        {
            ScopedLock lock(wifiLock);
            DBG("JsonWifiInterface::" << __func__ << ": wifi disconnected");
            connectedAP = nullptr;
            connected = false;
            signalWifiDisconnected();
        });

    }
}

/**
 * Turns on the simulated wifi device.  This will trigger a wifi enabled
 * event after a randomized delay of no more than six seconds.
 */
void JsonWifiInterface::enableWifi()
{

    ScopedLock lock(wifiLock);
    if (turningOn)
    {
        DBG("JsonWifiInterface::" << __func__ << ": already enabling wifi!");
    }
    else if (turningOff)
    {
        DBG("JsonWifiInterface::" << __func__
                << ": can't enable wifi, busy disabling");
    }
    else if (!enabled)
    {

        DBG("JsonWifiInterface::" << __func__ << ": enabling wifi...");
        turningOn = true;
        turningOff = false;
        TempTimer::initTimer(Random().nextInt(6000), [this]()
        {
            ScopedLock lock(wifiLock);
            if (turningOn)
            {
                DBG("JsonWifiInterface::" << __func__ << ": wifi enabled");
                        turningOn = false;
                        enabled = true;
                        signalWifiEnabled();
            }
        });
    }
}

/**
 * Turns off the simulated wifi device.  This will trigger a wifi disabled
 * event after a randomized delay of no more than six seconds.
 */
void JsonWifiInterface::disableWifi()
{
    if (turningOff)
    {
        DBG("JsonWifiInterface::" << __func__ << ": already disabling wifi!");
    }
    else if (enabled)
    {

        DBG("JsonWifiInterface::" << __func__ << ": disabling wifi...");
        turningOn = false;
        turningOff = true;
        TempTimer::initTimer(Random().nextInt(6000), [this]()
        {
            ScopedLock lock(wifiLock);
            if (turningOff)
            {
                DBG("JsonWifiInterface::" << __func__ << ": wifi disabled");
                        turningOff = false;
                        enabled = false;
                        signalWifiDisabled();
            }
        });
    }
}
