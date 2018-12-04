#include "Wifi/SavedState/APData.h"
#include "LibNM/NMObjects/AccessPoint.h"
#include "LibNM/ThreadHandler.h"

/* 
 * Creates the access point data object from a LibNM access point.
 */
Wifi::APData::APData(const LibNM::AccessPoint nmAccessPoint, 
        const LibNM::APHash apHash) : hash(apHash)
{
    LibNM::ThreadHandler nmThread;
    nmThread.call([this, &nmAccessPoint]()
    {
        ssid = nmAccessPoint.getSSIDText();
        bssid = nmAccessPoint.getBSSID();
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
juce::String Wifi::APData::getSSID() const
{
    return ssid;
}

/*
 * Gets the access point's hardware identifier.
 */
juce::String Wifi::APData::getBSSID() const
{
    return bssid;
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
 * Updates the access point's signal strength.
 */
void Wifi::APData::setSignalStrength(const unsigned int newStrength)
{
    signalStrength.set(newStrength);
}
