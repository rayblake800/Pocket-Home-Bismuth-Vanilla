#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once

/**
 * @file  Wifi_AP_Data.h
 *
 * @brief  Holds shared, reference counted data describing a Wifi access point.
 */

#include "LibNM/Data/APHash.h"
#include "LibNM/Data/SSID.h"
#include "JuceHeader.h"

namespace Wifi { namespace AP { class Data; } }
namespace LibNM { class AccessPoint; }
class Wifi::AP::Data : public juce::ReferenceCountedObject
{
public:
    /**
     * @brief  Creates the access point data object from a LibNM access point.
     *
     * @param nmAccessPoint    The LibNM access point this Data represents. 
     *
     * @param apHash           The access point's hash value.
     */
    Data(const LibNM::AccessPoint nmAccessPoint, const LibNM::APHash apHash);

    virtual ~Data() { }

    /**
     * @brief  Compares Data objects using their hash values.
     *
     * @param rhs  The Data object to compare with this one.
     *
     * @return     Whether this and rhs have identical hash values.
     */
    bool operator==(const Data& rhs) const;
    
    /**
     * @brief  Compares Data objects using their hash values.
     *
     * @param rhs  The Data object to compare with this one.
     *
     * @return     Whether this and rhs do not have identical hash values.
     */
    bool operator!=(const Data& rhs) const;
    
    /**
     * @brief  Compares Data objects using their hash values so they can be
     *         sorted.
     *
     * @param rhs  The Data object to compare with this one.
     *
     * @return     Whether this access point's hash value should come before the 
     *             rhs Data's hash value.
     */
    bool operator<(const Data& rhs) const;

    /**
     * @brief  Gets the access point's primary identifier.
     *
     * @return  The access point's SSID value.
     */
    LibNM::SSID getSSID() const;

    /**
     * @brief  Gets the access point's signal strength.
     *
     * @return  The access point signal strength, represented as a percentage
     *          between 0 and 100.
     */
    unsigned int getSignalStrength() const;

    /**
     * @brief  Gets the access point's general security type.
     *
     * @return  The type of security, if any, protecting the access point. 
     */
    LibNM::SecurityType getSecurityType() const;

    /**
     * @brief  Checks if a security key is formatted correctly for this access
     *         point's security type.
     *
     * @param psk  A possible access point security key. 
     *
     * @return     Whether the psk is a valid security key for this access
     *             point's security type.
     */
    bool isValidKeyFormat(const juce::String psk) const;

    /**
     * @brief  Gets the hash value used to identify and sort the access point.
     *
     * @return  The access point's APHash value. 
     */
    LibNM::APHash getHashValue() const;

    /**
     * @brief  Updates the access point's signal strength.
     *
     * @param newStrength  The new signal strength, as a percentage value
     *                     between 0 and 100.
     */
    void setSignalStrength(const unsigned int newStrength);

private:
    /* The hash value used to compare access points and identify access points
       with equivalent connections. */
    LibNM::APHash hash;

    /* The access point's displayed name and primary ID. */
    LibNM::SSID ssid;

    /* The access point's basic security type. */
    LibNM::SecurityType securityType;

    /* The access point's signal strength, between 0 and 100. */
    juce::Atomic<unsigned int> signalStrength;
};
