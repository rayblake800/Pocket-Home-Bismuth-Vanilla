#include <map>
#include "JuceHeader.h"
#include "AssetFiles.h"
#include "WifiStatusJson.h"
#include "TempTimer.h"

WifiStatusJson::WifiStatusJson() : connectedAP(WifiAccessPoint()) { }

WifiStatusJson::~WifiStatusJson() { }

/**
 * 
 */
bool WifiStatusJson::isWifiDeviceEnabled()
{
    ScopedLock lock(wifiLock);
    return enabled;
}

/**
 */
bool WifiStatusJson::isWifiConnecting()
{
    ScopedLock lock(wifiLock);
    return !waitingToConnect.isNull();
}

/**
 * 
 */
bool WifiStatusJson::isWifiConnected()
{
    ScopedLock lock(wifiLock);
    return connected;
}

/**
 * 
 */
WifiAccessPoint WifiStatusJson::getConnectedAP()
{
    ScopedLock lock(wifiLock);
    if (!connected)
    {
        return WifiAccessPoint();
    }
    return connectedAP;
}

/**
 * 
 */
WifiAccessPoint WifiStatusJson::getConnectingAP()
{
    ScopedLock lock(wifiLock);
    return waitingToConnect;
}

/**
 * 
 */
Array<WifiAccessPoint> WifiStatusJson::getVisibleAPs()
{
    Array<WifiAccessPoint> accessPoints;
    auto json = JSON::parse(AssetFiles::findAssetFile("wifi.json"));

    for (const auto &apJson : *json.getArray())
    {
        WifiAccessPoint ap(apJson["name"],
                apJson["strength"],
                apJson["auth"],
                apJson["name"]);
        accessPoints.add(ap);
    }
    DBG("WifiStatusJson::" << __func__ << ": APs found:" << accessPoints.size());
    return accessPoints;
}

/**
 * 
 */
void WifiStatusJson::connectToAccessPoint(WifiAccessPoint toConnect,
        String psk)
{
    ScopedLock lock(wifiLock);
    DBG("WifiStatusJson::" << __func__ << ": trying to connect to "
            << toConnect.getSSID());
    if (turningOff)
    {
        DBG("WifiStatusJson::" << __func__
                << ": can't connect, wifi is turning off");
    }
    else if (turningOn)
    {
        DBG("WifiStatusJson::" << __func__
                << ": can't connect, wifi still turning on");
    }
    else if (!enabled)
    {
        DBG("WifiStatusJson::" << __func__ << ": can't connect, wifi isn't on");
    }
    else if (!waitingToConnect.isNull())
    {
        if (waitingToConnect != toConnect)
        {
            DBG("WifiStatusJson::" << __func__
                    << ": canceling, a connection to " <<
                    waitingToConnect.getSSID() << " is pending ");
        }
        else
        {
            DBG("WifiStatusJson::" << __func__ << ": already connecting to "
                    << toConnect.getSSID());
        }
    }
    else if (connected)
    {
        DBG("WifiStatusJson::" << __func__ << ": already connected to "
                << toConnect.getSSID());
    }
    else
    {
        waitingToConnect = toConnect;
        // try to connect to ap, dispatch events on success and failure
        TempTimer::initTimer(Random().nextInt(15000) + 9000,
                [this, psk]()
                {
                    ScopedLock lock(wifiLock);
                    bool isTestCred = (waitingToConnect.getSSID() == "MyFi");
                    if (!isTestCred)
                    {
                        DBG("WifiStatusJson::" << __func__
                                << ": failed to connect");
                                connected = false;
                                waitingToConnect = WifiAccessPoint();
                                signalConnectionFailed();
                        return;
                    }

                    if (!psk.isEmpty())
                    {
                        DBG("WifiStatusJson::" << __func__
                                << ": connected with psk");
                    }
                    else
                    {
                        DBG("WifiStatusJson::" << __func__ << ": connected");
                    }
                    connected = true;
                    connectedAP = waitingToConnect;
                    waitingToConnect = WifiAccessPoint();
                    jassert(!connectedAP.isNull());
                    signalWifiConnected(connectedAP);
                });
    }
}

void WifiStatusJson::disconnect()
{
    ScopedLock lock(wifiLock);
    if (!connected)
    {
        DBG("WifiStatusJson::" << __func__ << ": no connection to kill");
    }
    else
    {
        TempTimer::initTimer(Random().nextInt(6000), [this]()
        {
            ScopedLock lock(wifiLock);
            DBG("WifiStatusJson::" << __func__ << ": wifi disconnected");
            connectedAP = WifiAccessPoint();
            connected = false;
            signalWifiDisconnected();
        });

    }
}

void WifiStatusJson::enableWifi()
{

    ScopedLock lock(wifiLock);
    if (turningOn)
    {
        DBG("WifiStatusJson::" << __func__ << ": already enabling wifi!");
    }
    else if (turningOff)
    {
        DBG("WifiStatusJson::" << __func__
                << ": can't enable wifi, busy disabling");
    }
    else if (!enabled)
    {

        DBG("WifiStatusJson::" << __func__ << ": enabling wifi...");
        turningOn = true;
        turningOff = false;
        TempTimer::initTimer(Random().nextInt(6000), [this]()
        {
            ScopedLock lock(wifiLock);
            if (turningOn)
            {
                DBG("WifiStatusJson::" << __func__ << ": wifi enabled");
                        turningOn = false;
                        enabled = true;
                        signalWifiEnabled();
            }
        });
    }
}

void WifiStatusJson::disableWifi()
{
    if (turningOff)
    {
        DBG("WifiStatusJson::" << __func__ << ": already disabling wifi!");
    }
    else if (enabled)
    {

        DBG("WifiStatusJson::" << __func__ << ": disabling wifi...");
        turningOn = false;
        turningOff = true;
        TempTimer::initTimer(Random().nextInt(6000), [this]()
        {
            ScopedLock lock(wifiLock);
            if (turningOff)
            {
                DBG("WifiStatusJson::" << __func__ << ": wifi disabled");
                        turningOff = false;
                        enabled = false;
                        signalWifiDisabled();
            }
        });
    }
}
