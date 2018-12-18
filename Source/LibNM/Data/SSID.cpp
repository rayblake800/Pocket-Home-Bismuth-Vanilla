#include "LibNM/Data/SSID.h"
#include <nm-utils.h>

/*
 * Creates this SSID copying another SSID.
 */
LibNM::SSID::SSID(const SSID& toCopy)
{
    *this = toCopy.getByteArray();
}

/*
 * Creates this SSID from a SSID byte string.
 */
LibNM::SSID::SSID(const GByteArray* toCopy)
{
    *this = toCopy;
}

/*
 * Frees all SSID data on destruction.
 */
LibNM::SSID::~SSID()
{
    *this = nullptr;
}

/*
 * Gets a text representation of this SSID.
 */
juce::String LibNM::SSID::toString() const
{
    juce::String ssidText;
    if(ssidBytes != nullptr)
    {
        char* utfSSID = nm_utils_ssid_to_utf8(ssidBytes);
        if (utfSSID != nullptr)
        {
            ssidText = utfSSID;
            g_free(utfSSID);
            utfSSID = nullptr;
        }
    }
    return ssidText;
}

/*
 * Gets this SSID's internal SSID byte string.
 */
GByteArray* LibNM::SSID::getByteArray() const
{
    return ssidBytes;
}

/*
 * Assigns another SSID's data to this SSID.
 */
LibNM::SSID& LibNM::SSID::operator=(const SSID& toCopy)
{
    return *this = toCopy.getByteArray();
}

/*
 * Assigns a copy of an SSID bytestring to this SSID.
 */
LibNM::SSID& LibNM::SSID::operator=(GByteArray* toAssign)
{
    if(ssidBytes != nullptr)
    {
        g_byte_array_unref(ssidBytes);
        ssidBytes = nullptr;
    }

    if(toAssign != nullptr)
    {
        ssidBytes = g_byte_array_sized_new(toAssign->len);
        g_byte_array_append(ssidBytes, toAssign->data, toAssign->len);
    }
}

/*
 * Checks if two SSIDs are equivalent.
 */
bool LibNM::SSID::operator==(const SSID& rhs) const
{
    return *this == rhs.getByteArray();
}

/*
 * Checks if two SSIDs are not equivalent.
 */
bool LibNM::SSID::operator!=(const SSID& rhs) const
{
    return !(*this == rhs.getByteArray());
}


/*
 * Compares this object with another SSID alphabetically.
 */
bool LibNM::SSID::operator<(const SSID& rhs) const
{
    if(ssidBytes == nullptr)
    {
        return false;
    }
    if(rhs.ssidBytes == nullptr)
    {
        return true;
    }
    const int comparedBytes = std::min(ssidBytes->len, rhs.ssidBytes->len);
    for(int i = 0; i < comparedBytes; i++)
    {
        if(ssidBytes->data[i] != rhs.ssidBytes->data[i])
        {
            return ssidBytes->data[i] < rhs.ssidBytes->data[i];
        }
    }
    return ssidBytes->len < rhs.ssidBytes->len;
}

/*
 * Checks if a SSID and a raw SSID byte string are equivalent.
 */
bool LibNM::SSID::operator==(GByteArray* rhs) const
{
    if(ssidBytes == nullptr)
    {
        return rhs == nullptr;
    }
    if(rhs == nullptr)
    {
        return ssidBytes == nullptr;
    }
    if(ssidBytes->len != rhs->len)
    {
        return false;
    }
    for(int i = 0; i < ssidBytes->len; i++)
    {
        if(ssidBytes->data[i] != rhs->data[i])
        {
            return false;
        }
    }
    return true;
}

/*
 * Checks if a SSID and a raw SSID byte string are not equivalent.
 */
bool LibNM::SSID::operator!=(GByteArray* rhs) const
{
    return !(*this == rhs);
}
