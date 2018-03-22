#include "JuceHeader.h"
#include "Utils.h"
#include "WifiAccessPoint.h"

WifiAccessPoint::WifiAccessPoint
(String ssid, int signalStrength, bool requiresAuth, String hash) :
ssid(ssid),
signalStrength(median<int>(0, signalStrength, 100)),
requiresAuth(requiresAuth),
hash(hash) { }

/**
 * Creates a null WifiAccessPoint, representing the absence of a wifi access 
 * point.
 */
WifiAccessPoint::WifiAccessPoint() :
ssid(String()),
signalStrength(-1),
requiresAuth(false),
hash(String()) { }

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
