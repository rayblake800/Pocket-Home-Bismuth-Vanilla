#include <map>

#include "../Utils.h"

#include "WifiStatusJson.h"
#include "../JuceLibraryCode/JuceHeader.h"

WifiStatusJson::WifiStatusJson() : connectedAP(WifiAccessPoint::null()) { }

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
        notifyListeners(wifiBusy);
        enabled = true;
        notifyListeners(wifiEnabled);
    }
}

void WifiStatusJson::disableWifi()
{
    if (enabled)
    {
        notifyListeners(wifiBusy);
        enabled = false;
        notifyListeners(wifiDisabled);
    }
}

void WifiStatusJson::setConnectedAccessPoint(const WifiAccessPoint& ap, String psk)
{
    notifyListeners(wifiBusy);
    // try to connect to ap, dispatch events on success and failure
    bool isTestCred = (ap.getSSID() == "MyFi");
    if (!isTestCred)
    {
        DBG("WifiStatusJson::" << __func__ << ": failed to connect");
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
}

void WifiStatusJson::disconnect()
{
    connected = false;
        notifyListeners(wifiDisconnected);
}
