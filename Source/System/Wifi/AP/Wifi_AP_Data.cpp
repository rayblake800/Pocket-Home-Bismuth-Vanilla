#define WIFI_IMPLEMENTATION
#include "Wifi_AP_Data.h"

// Creates the access point data object from a LibNM access point.
Wifi::AP::Data::Data(
        const LibNM::SSID ssid,
        const LibNM::APHash apHash,
        const LibNM::SecurityType securityType,
        const int strength) :
    ssid(ssid),
    hash(apHash),
    securityType(securityType)
{
    connectionSaved.set(false);
    signalStrength.set(strength);
}


// Compares Data objects using their hash values.
bool Wifi::AP::Data::operator== (const Data& rhs) const
{
    return hash == rhs.hash;
}


// Compares AP::Data objects using their hash values.
bool Wifi::AP::Data::operator!= (const Data& rhs) const
{
    return hash != rhs.hash;
}


// Compares AP::Data objects using their hash values so they can be sorted.
bool Wifi::AP::Data::operator< (const Data& rhs) const
{
    return hash < rhs.hash;
}


// Gets the access point's primary identifier.
Wifi::LibNM::SSID Wifi::AP::Data::getSSID() const
{
    return ssid;
}


// Gets the access point's signal strength.
unsigned int Wifi::AP::Data::getSignalStrength() const
{
    return signalStrength.get();
}


// Checks if a saved network connection exists that is compatible with this
// access point.
bool Wifi::AP::Data::hasSavedConnection() const
{
    return connectionSaved.get();
}


// Gets the last recorded time the system was connected using this access
// point's connection.
juce::int64 Wifi::AP::Data::getLastConnectionTime() const
{
    return connectionTime.get();
}


// Gets the access point's general security type.
Wifi::LibNM::SecurityType Wifi::AP::Data::getSecurityType() const
{
    return securityType;
}


// Checks if a security key is formatted correctly for this access point's
// security type.
bool Wifi::AP::Data::isValidKeyFormat(const juce::String psk) const
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


// Gets the hash value used to identify and sort the access point.
Wifi::LibNM::APHash Wifi::AP::Data::getHashValue() const
{
    return hash;
}


// Updates the access point's signal strength.
void Wifi::AP::Data::setSignalStrength(const unsigned int newStrength)
{
    signalStrength.set(newStrength);
}


// Sets the AccessPoint data object's record of whether it has a compatible
// saved connection.
void Wifi::AP::Data::setHasSavedConnection(const bool isSaved)
{
    connectionSaved.set(isSaved);
}


// Stores the last time the system was connected to a network using a
// connection that is compatible with this access point.
void Wifi::AP::Data::setLastConnectionTime(const juce::int64 newTime)
{
    connectionTime.set(newTime);
}


// Checks if this data object has no valid data.
bool Wifi::AP::Data::isNull() const
{
    return hash.isNull();
}


// Gets a string representation of the AP data object for debug use.
juce::String Wifi::AP::Data::toString() const
{
    juce::String dataString = " (";
    dataString += ssid.toString();
    dataString += ", strength ";
    dataString += juce::String((int) signalStrength.get());
    dataString += ", ";
    switch(securityType)
    {
        case LibNM::SecurityType::unsecured:
            dataString += "unsecured";
            break;
        case LibNM::SecurityType::securedWEP:
            dataString += "securedWEP";
            break;
        case LibNM::SecurityType::securedWPA:
            dataString += "securedWPA";
            break;
        case LibNM::SecurityType::securedRSN:
            dataString += "securedRSN";
    }
    dataString += ", Hash ";
    dataString += hash.toString();
    dataString += ")";
    return dataString;
}

