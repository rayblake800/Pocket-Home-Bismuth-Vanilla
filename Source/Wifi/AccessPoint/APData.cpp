#define WIFI_IMPLEMENTATION
#include "Wifi/AccessPoint/APData.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/Data/SecurityType.h"

/* 
 * Creates the access point data object from a LibNM access point.
 */
Wifi::APData::APData(const LibNM::AccessPoint nmAccessPoint, 
        const LibNM::APHash apHash) : hash(apHash)
{
    LibNM::ThreadHandler nmThread;
    nmThread.call([this, &nmAccessPoint]()
    {
        ssid = nmAccessPoint.getSSID();
        securityType = nmAccessPoint.getSecurityType();
        signalStrength.set(nmAccessPoint.getSignalStrength());
    });
}

/*
 * Compares APData objects using their hash values.
 */
bool Wifi::APData::operator==(const APData& rhs) const
{
    return hash == rhs.hash;
}

/*
 * Compares APData objects using their hash values.
 */
bool Wifi::APData::operator!=(const APData& rhs) const
{
    return hash != rhs.hash;
}

/*
 * Compares APData objects using their hash values so they can be sorted.
 */
bool Wifi::APData::operator<(const APData& rhs) const
{
    return hash < rhs.hash;
}

/*
 * Gets the access point's primary identifier.
 */
LibNM::SSID Wifi::APData::getSSID() const
{
    return ssid;
}

/*
 * Gets the access point's signal strength.
 */
unsigned int Wifi::APData::getSignalStrength() const
{
    return signalStrength.get();
}

/*
 * Gets the access point's general security type.
 */
LibNM::SecurityType Wifi::APData::getSecurityType() const
{
    return securityType;
}

/*
 * Checks if a security key is formatted correctly for this access point's 
 * security type.
 */
bool Wifi::APData::isValidKeyFormat(const juce::String psk) const
{
    const int length = psk.length();
    switch(securityType)
    {
        case LibNM::SecurityType::unsecured:
            return length == 0;
        case LibNM::SecurityType::securedWEP:
            return length == 10 || length == 26 || length == 5 || length == 13;
        case LibNM::SecurityType::securedWPA:
        case LibNM::SecurityType::securedRSN:
            return length >= 8;
    }
    // isValidKeyFormat() should be returning before this point.
    jassertfalse;
    return false;
}

/*
 * Gets the hash value used to identify and sort the access point.
 */
LibNM::APHash Wifi::APData::getHashValue() const
{
    return hash;
}

/*
 * Updates the access point's signal strength.
 */
void Wifi::APData::setSignalStrength(const unsigned int newStrength)
{
    signalStrength.set(newStrength);
}
