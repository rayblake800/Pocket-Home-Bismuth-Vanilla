#include "LibNM/APHash.h"

/*
 * Generates a Wifi access point identifier using the access point's SSID, mode, 
 * and security settings.
 */
 LibNM::APHash::APHash(const GByteArray* ssid, 
         Mode mode, 
         SecurityType securityType)
{
    unsigned char input[66];
    memset(&input[0], 0, sizeof (input));
    if (ssid != nullptr)
    {
        memcpy(input, ssid->data, ssid->len);
    }
    // Use mode and securityType as bitflags
    input[32] |= (unsigned char) mode;
    input[32] |= (unsigned char) securityType;
    /* duplicate it */
    memcpy(&input[33], &input[0], 32);
    return g_compute_checksum_for_data
            (G_CHECKSUM_MD5, input, sizeof (input));
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
