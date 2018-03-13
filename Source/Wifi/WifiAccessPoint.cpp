#include "../Utils.h"
#include "WifiAccessPoint.h"

WifiAccessPoint::WifiAccessPoint
(String ssid, int signalStrength, bool requiresAuth, String hash) :
ssid(ssid),
signalStrength(median<int>(0, signalStrength, 100)),
requiresAuth(requiresAuth),
hash(hash) { }

/**
 * Creates a null WifiAccessPoint, that represents the absence of a 
 * wifi access Point.
 */
WifiAccessPoint::WifiAccessPoint() :
ssid(String()),
signalStrength(-1),
requiresAuth(false),
hash(String()) { }

WifiAccessPoint::~WifiAccessPoint() { }

/**
 * @return true iff this WifiAccessPoint is null.
 */
bool WifiAccessPoint::isNull() const
{
    return ssid.isEmpty()
            && signalStrength == -1
            && !requiresAuth
            && hash.isEmpty();
}

const String& WifiAccessPoint::getSSID() const
{
    return ssid;
}

int WifiAccessPoint::getSignalStrength() const
{
    return signalStrength;
}

bool WifiAccessPoint::getRequiresAuth() const
{
    return requiresAuth;
}

const String& WifiAccessPoint::getHash() const
{
    return hash;
}

String WifiAccessPoint::toString() const 
{
    return ssid;
}