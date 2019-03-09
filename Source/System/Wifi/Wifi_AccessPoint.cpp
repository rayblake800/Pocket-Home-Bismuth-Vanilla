#define WIFI_IMPLEMENTATION
#include "Wifi_AccessPoint.h"
#include "Wifi_AP_Data.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_APHash.h"
#include "Wifi_LibNM_APMode.h"
#include "Wifi_LibNM_SSID.h"
#include "Wifi_LibNM_SecurityType.h"
#include "Wifi_LibNM_ContextTest.h"

/*
 * Creates new access point data from a LibNM access point object.
 */
Wifi::AccessPoint::AccessPoint(const LibNM::SSID ssid,
            const LibNM::APHash apHash,
            const LibNM::SecurityType securityType,
            const int strength)
{
    getDataReference() = new AP::Data(ssid, apHash, securityType, strength);
}

/*
 * Initializes the AccessPoint with another AccessPoint's data.
 */
Wifi::AccessPoint::AccessPoint(const AccessPoint& rhs) : Nullable(rhs) { }

/*
 * Compares AccessPoint objects using their hash values.
 */
bool Wifi::AccessPoint::operator==(const AccessPoint& rhs) const
{
    return getHashValue() == rhs.getHashValue();
}

/*
 * Compares AccessPoint objects using their hash values.
 */
bool Wifi::AccessPoint::operator!=(const AccessPoint& rhs) const
{
    return getHashValue() != rhs.getHashValue();
}

/*
 * Checks if this AccessPoint represents a particular LibNM::AccessPoint.
 */
bool Wifi::AccessPoint::operator==(const LibNM::AccessPoint& rhs) const
{
    return getHashValue() == rhs.generateHash();
}

/*
 * Checks if this AccessPoint does not represent a particular 
 * LibNM::AccessPoint.
 */
bool Wifi::AccessPoint::operator!=(const LibNM::AccessPoint& rhs) const
{
    return getHashValue() != rhs.generateHash();
}

/*
 * Compares AccessPoint objects using their hash values so they can be sorted.
 */
bool Wifi::AccessPoint::operator<(const AccessPoint& rhs) const
{
    if(isNull())
    {
        return false;
    }
    if(rhs.isNull())
    {
        return true;
    }
    return getHashValue() < rhs.getHashValue();
}

/*
 * Gets the access point's primary identifier.
 */
Wifi::LibNM::SSID Wifi::AccessPoint::getSSID() const
{
    if(isNull())
    {
        return LibNM::SSID();
    }
    return getData()->getSSID();
}

/*
 * Gets the access point's signal strength.
 */
unsigned int Wifi::AccessPoint::getSignalStrength() const
{
    if(isNull())
    {
        return 0;
    }
    return getData()->getSignalStrength();
}

/*
 * Checks whether this access point is compatible with a saved network 
 * connection.
 */
bool Wifi::AccessPoint::hasSavedConnection() const
{
    if(isNull())
    {
        return false;
    }
    return getData()->hasSavedConnection();
}

/*
 * Gets the last recorded time the system was connected using this access 
 * point's connection.
 */
juce::int64 Wifi::AccessPoint::getLastConnectionTime() const
{
    if(isNull())
    {
        return 0;
    }
    return getData()->getLastConnectionTime();
}

/*
 * Gets the access point's general security type.
 */
Wifi::LibNM::SecurityType Wifi::AccessPoint::getSecurityType() const
{
    if(isNull())
    {
        return LibNM::SecurityType::unsecured;
    }
    return getData()->getSecurityType();
}

/*
 * Checks if a security key is formatted correctly for this access point's 
 * security type.
 */
bool Wifi::AccessPoint::isValidKeyFormat(const juce::String psk) const
{
    if(isNull())
    {
        return false;
    }
    return getData()->isValidKeyFormat(psk);
}

/*
 * Gets the hash value used to identify and sort the access point.
 */
Wifi::LibNM::APHash Wifi::AccessPoint::getHashValue() const
{
    if(isNull())
    {
        return LibNM::APHash();
    }
    return getData()->getHashValue();
}

/*
 * Gets a string representation of the AccessPoint for debug use.
 */
juce::String Wifi::AccessPoint::toString() const
{
    if(getData() == nullptr)
    {
        return "NULL";
    }
    return getData()->toString();
}

/*
 * Updates the access point's signal strength.
 */
void Wifi::AccessPoint::setSignalStrength(const unsigned int newStrength)
{
    if(!isNull())
    {
        getData()->setSignalStrength(newStrength);
    }
}

/*
 * Sets the AccessPoint object's record of whether it has a compatible saved 
 * connection.
 */
void Wifi::AccessPoint::setHasSavedConnection(const bool isSaved)
{
    if(!isNull())
    {
        getData()->setHasSavedConnection(isSaved);
    }
}

/*
 * Stores the last time the system was connected to a network using a connection
 * that is compatible with this access point.
 */
void Wifi::AccessPoint::setLastConnectionTime(const juce::int64 newTime)
{
    if(!isNull())
    {
        getData()->setLastConnectionTime(newTime);
    }
}
