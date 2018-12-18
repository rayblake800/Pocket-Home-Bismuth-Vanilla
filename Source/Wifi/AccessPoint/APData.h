#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once

/**
 * @file  Wifi/SavedState/APData.h
 *
 * @brief  Holds data describing a Wifi access point.
 */

#include "LibNM/Data/APHash.h"
#include "JuceHeader.h"

namespace Wifi { class APData; }
namespace LibNM { class AccessPoint; }
class Wifi::APData : public juce::ReferenceCountedObject
{
public:
    /**
     * @brief  Creates the access point data object from a LibNM access point.
     *
     * @param nmAccessPoint    The LibNM access point this APData represents. 
     *
     * @param apHash           The access point's hash value.
     */
    APData(const LibNM::AccessPoint nmAccessPoint, const LibNM::APHash apHash);

    virtual ~APData() { }

    /**
     * @brief  Compares APData objects using their hash values.
     *
     * @param rhs  The APData object to compare with this one.
     *
     * @return     Whether this and rhs have identical hash values.
     */
    bool operator==(const APData& rhs) const;
    
    /**
     * @brief  Compares APData objects using their hash values.
     *
     * @param rhs  The APData object to compare with this one.
     *
     * @return     Whether this and rhs do not have identical hash values.
     */
    bool operator!=(const APData& rhs) const;
    
    /**
     * @brief  Compares APData objects using their hash values so they can be
     *         sorted.
     *
     * @param rhs  The APData object to compare with this one.
     *
     * @return     Whether this access point's hash value should come before the 
     *             rhs APData's hash value.
     */
    bool operator<(const APData& rhs) const;

    /**
     * @brief  Gets the access point's primary identifier.
     *
     * @return  The access point's SSID value, as a printable string.
     */
    juce::String getSSID() const;

    /**
     * @brief  Gets the access point's hardware identifier.
     *
     * @return  The access point's BSSID value, which is usually its MAC 
     *          address. 
     */
    juce::String getBSSID() const;

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
    juce::String ssid;

    /* The access point's base hardware ID. */
    juce::String bssid;

    /* The access point's basic security type. */
    LibNM::SecurityType securityType;

    /* The access point's signal strength, between 0 and 100. */
    juce::Atomic<unsigned int> signalStrength;
};
