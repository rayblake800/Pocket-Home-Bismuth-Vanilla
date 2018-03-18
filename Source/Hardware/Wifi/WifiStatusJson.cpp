#include <map>
#include "JuceHeader.h"
#include "AssetFiles.h"
#include "WifiStatusJson.h"
#include "TempTimer.h"

WifiStatusJson::WifiStatusJson() : connectedAP(WifiAccessPoint()) { }

WifiStatusJson::~WifiStatusJson() { }

Array<WifiAccessPoint> WifiStatusJson::nearbyAccessPoints()
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
    return accessPoints;
}

WifiAccessPoint WifiStatusJson::getConnectedAccessPoint() const
{
    ScopedLock lock(wifiLock);
    if (!connected)
    {
        return WifiAccessPoint();
    }
    return connectedAP;
}

bool WifiStatusJson::isEnabled() const
{
    ScopedLock lock(wifiLock);
    return enabled;
}

bool WifiStatusJson::isConnected() const
{
    ScopedLock lock(wifiLock);
    return connected;
}

/**
 * @return true iff wifi is currently being enabled.
 */
bool WifiStatusJson::isTurningOn() const
{
    ScopedLock lock(wifiLock);
    return turningOn;
}

/**
 * @return true iff wifi is currently being disabled.
 */
bool WifiStatusJson::isTurningOff() const
{
    ScopedLock lock(wifiLock);
    return turningOff;
}

/**
 * @return true iff wifi is attempting to connect to an access point.
 */
bool WifiStatusJson::isConnecting() const
{
    ScopedLock lock(wifiLock);
    return connecting;
}

/**
 * @return true iff wifi is attempting to disconnect from an access point. 
 */
bool WifiStatusJson::isDisconnecting() const
{
    ScopedLock lock(wifiLock);
    return disconnecting;
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
        DBG("WifiStatusJson::" << __func__ << ": can't enable wifi, busy disabling");
    }
    else if (enabled)
    {
        DBG("WifiStatusJson::" << __func__ << ": wifi is already on.");
    }
    else
    {
        DBG("WifiStatusJson::" << __func__ << ": starting to turn on wifi");
        turningOn = true;
        notifyListeners(wifiTurningOff);

        TempTimer::initTimer(Random().nextInt(15000) + 1000,
                [this]()
                {
                    ScopedLock lock(wifiLock);
                    turningOn = false;
                    enabled = true;
                    DBG("WifiStatusJson::" << __func__ << ": wifi turned on");
                    notifyListeners(wifiEnabled);
                    if (!waitingToConnect.isNull())
                    {
                        ScopedUnlock connectionUnlock(wifiLock);
                        setConnectedAccessPoint(waitingToConnect, waitingPsk);
                    }
                });
    }
}

void WifiStatusJson::disableWifi()
{
    ScopedLock lock(wifiLock);
    if (turningOn)
    {
        DBG("WifiStatusJson::" << __func__ << ": can't disable, busy enabling");
    }
    else if (turningOff)
    {
        DBG("WifiStatusJson::" << __func__ << ": already disabling wifi!");
    }
    else if (!enabled)
    {
        DBG("WifiStatusJson::" << __func__ << ": wifi is already off.");
    }
    else
    {
        DBG("WifiStatusJson::" << __func__ << ": starting to turn off wifi");
        turningOff = true;
        notifyListeners(wifiTurningOff);

        TempTimer::initTimer(Random().nextInt(15000) + 1000,
                [this]()
                {
                    ScopedLock lock(wifiLock);
                    turningOff = false;
                    enabled = false;
                    connectedAP = WifiAccessPoint();
                    waitingToConnect = WifiAccessPoint();
                    waitingPsk = "";
                    DBG("WifiStatusJson::" << __func__ << ": wifi turned off");
                    notifyListeners(wifiDisabled);
                });
    }
}

void WifiStatusJson::setConnectedAccessPoint
(const WifiAccessPoint& ap, String psk)
{
    ScopedLock lock(wifiLock);
    DBG("WifiStatusJson::" << __func__ << ": trying to connect to "
            << ap.getSSID());
    if (turningOff)
    {
        DBG("WifiStatusJson::" << __func__
                << ": can't connect, wifi is turning off");
    }
    if (turningOn)
    {
        DBG("WifiStatusJson::" << __func__
                << ": can't connect, wifi still turning on");
        waitingToConnect = ap;
        waitingPsk = psk;
    }
    else if (!enabled)
    {
        DBG("WifiStatusJson::" << __func__ << ": can't connect, wifi isn't on");
    }
    else if (connecting)
    {
        if (waitingToConnect != ap)
        {
            DBG("WifiStatusJson::" << __func__
                    << ": cancelling, a connection to " <<
                    waitingToConnect.getSSID() << " is pending ");
        }
        else
        {
            DBG("WifiStatusJson::" << __func__ << ": already connecting to"
                    << ap.getSSID());
        }
    }
    else if (connected)
    {
        DBG("WifiStatusJson::" << __func__ << ": already connected to "
                << ap.getSSID());
        if (ap != connectedAP)
        {
            DBG("WifiStatusJson::" << __func__
                    << ": disconnecting and scheduling new connection");
            waitingToConnect = ap;
            waitingPsk = psk;
            ScopedUnlock unlock(wifiLock);
            disconnect();
        }
    }
    else
    {
        connecting = true;
        waitingToConnect = ap;
        waitingPsk = psk;
        notifyListeners(wifiConnecting);
        //check for timeout
        TempTimer::initTimer(20000, [this]()
        {
            ScopedLock lock(wifiLock);
            if (!connected && connecting)
            {
                DBG("WifiStatusJson::" << __func__ << ": connection attempt to "
                        << waitingToConnect.getSSID() << " timed out.");
                        connecting = false;
                        waitingToConnect = WifiAccessPoint();
                        waitingPsk = "";
            }
        });

        // try to connect to ap, dispatch events on success and failure
        TempTimer::initTimer(Random().nextInt(15000) + 9000,
                [this]()
                {
                    ScopedLock lock(wifiLock);
                    if (!connecting)
                    {
                        //already timed out
                        return;
                    }
                    connecting = false;
                    bool isTestCred = (ap.getSSID() == "MyFi");
                    if (!isTestCred)
                    {
                        DBG("WifiStatusJson::" << __func__
                                << ": failed to connect");
                                connected = false;
                                notifyListeners(wifiConnectionFailed);
                        return;
                    }

                    if (psk.isEmpty())
                    {
                        DBG("WifiStatusJson::" << __func__ << ": connected with psk");
                                connected = true;
                                connectedAP = ap;
                                notifyListeners(wifiConnected);
                    }
                    else
                    {
                        DBG("WifiStatusJson::" << __func__ << ": connected");
                                connected = true;
                                connectedAP = ap;
                                notifyListeners(wifiConnected);
                    }

                });

    }
}

void WifiStatusJson::disconnect()
{
    connected = false;
    notifyListeners(wifiDisconnected);
}
