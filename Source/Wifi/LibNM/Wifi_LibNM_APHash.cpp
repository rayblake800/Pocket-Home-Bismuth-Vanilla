#include "Wifi_LibNM_APHash.h"
#include "Wifi_LibNM_APMode.h"
#include "Wifi_LibNM_SecurityType.h"
#include "Wifi_LibNM_ContextTest.h"

/*
 * Generates a Wifi access point identifier using the access point's SSID, mode, 
 * and security settings.
 */
Wifi::LibNM::APHash::APHash(const GByteArray* ssid, 
         APMode mode, 
         SecurityType securityType)
{
    if(ssid == nullptr)
    {
        hashValue = juce::Identifier::null;
        jassertfalse;
        return;
    }

    unsigned char input[66] = {0};
    memcpy(input, ssid->data, ssid->len);
    // Use mode and securityType as bitflags
    input[32] |= (unsigned char) mode;
    input[32] |= (unsigned char) securityType;
    /* duplicate it */
    memcpy(&input[33], &input[0], 32);
    juce::String inputStr;
    for(int i = 0; i < 66; i++)
    {
        inputStr += input[i];
    }
    char* hashString = g_compute_checksum_for_data
            (G_CHECKSUM_MD5, input, sizeof (input));
    hashValue = hashString;
}

/*
 * Creates an APHash as a copy of another hash value.
 */
Wifi::LibNM::APHash::APHash(const APHash& toCopy) : 
        hashValue(toCopy.hashValue) { }

/*
 * Creates an APHash from a saved hash value string.
 */
Wifi::LibNM::APHash::APHash(const juce::String hashString) : 
        hashValue(hashString) { }

/*
 * Checks if another APHash is equivalent to this one.
 */
bool Wifi::LibNM::APHash::operator==(const APHash& rhs) const
{
    return hashValue == rhs.hashValue;
}

/*
 * Checks if another APHash is not equivalent to this one.
 */
bool Wifi::LibNM::APHash::operator!=(const APHash& rhs) const
{
    return hashValue != rhs.hashValue;
}

/*
 * Compares this APHash with another, so hash values can be sorted.
 */
bool Wifi::LibNM::APHash::operator<(const APHash& rhs) const
{
    return hashValue < rhs.hashValue;
}

/*
 * Checks if this hash object is invalid.
 */
bool Wifi::LibNM::APHash::isNull() const
{
    return hashValue.isNull();
}

/*
 * Gets a string representation of the hash value.
 */
juce::String Wifi::LibNM::APHash::toString() const
{
    return hashValue.toString();
}
