#define WIFI_IMPLEMENTATION
#include "Wifi/AccessPoint/AccessPoint.h"
#include "Wifi/AccessPoint/APData.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/Data/APHash.h"
#include "LibNM/Data/APMode.h"
#include "LibNM/Data/SecurityType.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/ContextTest.h"


/*
 * Creates new access point data from a LibNM access point object.
 */
Wifi::AccessPoint::AccessPoint(const LibNM::AccessPoint nmAccessPoint)
{
    ASSERT_NM_CONTEXT
    LibNM::APHash hash = nmAccessPoint.generateHash();
    LibNM::ThreadHandler nmThread;
    getDataReference() = new APData(nmAccessPoint, hash);
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
    if(isNull())
    {
        return rhs.isNull();
    }
    if(rhs.isNull())
    {
        return false;
    }
    return *getData() == *rhs.getData();
}

/*
 * Compares AccessPoint objects using their hash values.
 */
bool Wifi::AccessPoint::operator!=(const AccessPoint& rhs) const
{
    return !(*this == rhs);
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
    return *getData() < *rhs.getData();
}

/*
 * Gets the access point's primary identifier.
 */
LibNM::SSID Wifi::AccessPoint::getSSID() const
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
 * Gets the access point's general security type.
 */
LibNM::SecurityType Wifi::AccessPoint::getSecurityType() const
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
LibNM::APHash Wifi::AccessPoint::getHashValue() const
{
    if(isNull())
    {
        return LibNM::APHash(nullptr, LibNM::APMode::unknown,
                LibNM::SecurityType::unsecured); 
    }
    return getData()->getHashValue();
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
