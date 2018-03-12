#include <map>

#include "../Utils.h"

#include "WifiStatusJson.h"
#include "../JuceLibraryCode/JuceHeader.h"

WifiStatusJson::WifiStatusJson() : connectedAP(WifiAccessPoint::null()){ }

WifiStatusJson::~WifiStatusJson() { }

Array<WifiAccessPoint> WifiStatusJson::nearbyAccessPoints()
{
    Array<WifiAccessPoint> accessPoints;
    auto json = JSON::parse(assetFile("wifi.json"));

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

WifiAccessPoint WifiStatusJson::connectedAccessPoint() const
{
    if (!connected)
    {
        return WifiAccessPoint::null();
    }
    return connectedAP;
}

bool WifiStatusJson::isEnabled() const
{
    return enabled;
}

bool WifiStatusJson::isConnected() const
{
    return connected;
}

void WifiStatusJson::enableWifi()
{
    if (!enabled)
    {
        notifyListenersWifiBusy();
        notifyListenersWifiEnabled();
    }
}

void WifiStatusJson::disableWifi()
{
    if (enabled)
    {
        notifyListenersWifiBusy();
        enabled = false;
        notifyListenersWifiDisabled();
    }
}

void WifiStatusJson::setConnectedAccessPoint(const WifiAccessPoint& ap, String psk)
{
    notifyListenersWifiBusy();

    // try to connect to ap, dispatch events on success and failure
    bool isTestCred = (ap.getSSID() == "MyFi");
    if (!isTestCred)
    {
        DBG("WifiStatusJson::setConnectedAccessPoint - failed ");
        connected = false;
        notifyListenersWifiFailedConnect();
        return;
    }

    if (psk.isEmpty())
    {
        DBG("WifiStatusJson::setConnectedAccessPoint - connect with psk");
        connected = true;
        connectedAP = ap;
        notifyListenersWifiConnected();
    }
    else
    {
        DBG("WifiStatusJson::setConnectedAccessPoint - connect");
        connected = true;
        connectedAP = ap;
        notifyListenersWifiConnected();
    }
}

void WifiStatusJson::disconnect()
{
    connected = false;
    notifyListenersWifiDisconnected();
}
