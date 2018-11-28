#include "LibNM/APHash.h"

/*
 * Generates a Wifi access point identifier using the access point's SSID, mode, 
 * and security settings.
 */
 LibNM::APHash::APHash(const GByteArray* ssid, 
         NM80211Mode mode, 
         NM80211ApSecurityFlags securityFlags)
{
}

/*
 * Creates an APHash as a copy of another hash value.
 */
LibNM::APHash::APHash(const APHash& toCopy) : hashValue(toCopy.hashValue) { }

/*
 * Checks if another APHash is equivalent to this one.
 */
bool LibNM::APHash::operator==(const APHash& rhs) const
{
    return hashValue == rhs.hashValue;
}

/*
 * Checks if another APHash is not equivalent to this one.
 */
bool LibNM::APHash::operator!=(const APHash& rhs) const
{
    return hashValue != rhs.hashValue;
}

/*
 * Compares this APHash with another, so hash values can be sorted.
 */
bool LibNM::APHash::operator<(const APHash& rhs) const
{
    return hashValue < rhs.hashValue;
}
