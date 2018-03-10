#include <map>

#include "../Utils.h"

#include "WifiStatusJson.h"
#include "../JuceLibraryCode/JuceHeader.h"

WifiStatusJson::WifiStatusJson() : listeners() { }

WifiStatusJson::~WifiStatusJson() { }

Array<WifiAccessPoint> WifiStatusJson::nearbyAccessPoints()
{
    Array<WifiAccessPoint> accessPoints;
    auto json = JSON::parse(assetFile("wifi.json"));

    for (const auto &apJson : *json.getArray())
    {
        WifiAccessPoint ap;
        ap.ssid = apJson["name"];
        ap.signalStrength = apJson["strength"];
        ap.requiresAuth = apJson["auth"];
        ap.hash = apJson["name"];
        accessPoints.add(ap);
    }
    return accessPoints;
}

WifiAccessPoint WifiStatusJson::connectedAccessPoint() const
{
    if (!connected)
    {
        return WifiAccessPoint::null;
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

void WifiStatusJson::addListener(Listener* listener)
{
    listeners.add(listener);
}

void WifiStatusJson::clearListeners()
{
    listeners.clear();
}

// TODO: direct action should not be named set, e.g. enable/disable/disconnect
// otherwise easily confused with setters thats wrap members, which are slightly different idiom

void WifiStatusJson::setEnabled()
{
    if (!enabled)
    {
        for (const auto& listener : listeners)
        {
            listener->handleWifiBusy();
        }

        enabled = true;
        for (const auto& listener : listeners)
        {
            listener->handleWifiEnabled();
        }
    }
}

void WifiStatusJson::setDisabled()
{
    if (enabled)
    {
        for (const auto& listener : listeners)
        {
            listener->handleWifiBusy();
        }

        enabled = false;
        for (const auto& listener : listeners)
        {
            listener->handleWifiDisabled();
        }
    }
}

void WifiStatusJson::setConnectedAccessPoint(const WifiAccessPoint& ap, String psk)
{
    for (const auto& listener : listeners)
    {
        listener->handleWifiBusy();
    }

    // try to connect to ap, dispatch events on success and failure
    bool isTestCred = ap.ssid == "MyFi";
    if (!isTestCred)
    {
        DBG("WifiStatusJson::setConnectedAccessPoint - failed ");
        connected = false;
        for (const auto& listener : listeners)
        {
            listener->handleWifiFailedConnect();
        }
        return;
    }

    if (psk.isEmpty())
    {
        DBG("WifiStatusJson::setConnectedAccessPoint - connect with psk");
        connected = true;
        connectedAP = ap;
        for (const auto& listener : listeners)
        {
            listener->handleWifiConnected();
        }
    }
    else
    {
        DBG("WifiStatusJson::setConnectedAccessPoint - connect");
        connected = true;
        connectedAP = ap;
        for (const auto& listener : listeners)
        {
            listener->handleWifiConnected();
        }
    }
}

void WifiStatusJson::setDisconnected()
{
    connected = false;
    for (Listener* listener : listeners)
    {
        listener->handleWifiDisconnected();
    }
}

void WifiStatusJson::initializeStatus()
{
    connected = false;
}
