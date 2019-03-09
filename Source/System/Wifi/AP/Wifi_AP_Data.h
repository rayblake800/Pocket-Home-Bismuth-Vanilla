#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_AP_Data.h
 *
 * @brief  Holds shared, reference counted data describing a Wifi access point.
 */

#include "Wifi_LibNM_APHash.h"
#include "Wifi_LibNM_SSID.h"
#include "Wifi_LibNM_SecurityType.h"
#include "JuceHeader.h"

namespace Wifi { namespace AP { class Data; } }
namespace LibNM { class AccessPoint; }

class Wifi::AP::Data : public juce::ReferenceCountedObject
{
public:
    /**
     * @brief  Creates the access point data object from a LibNM access point.
     *
     * @param ssid             The access point's SSID.
     *
     * @param apHash           The access point's hash value.
     *
     * @param securityType     The type of security settings used by the access
     *                         point.
     *
     * @param strength         The access point's initial signal strength.
     */
    Data(const LibNM::SSID ssid,
            const LibNM::APHash apHash,
            const LibNM::SecurityType securityType,
            const int strength);

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
     * @brief  Checks if a saved network connection exists that is compatible
     *         with this access point.
     *
     * @return  Whether a valid saved connection was found.
     */
    bool hasSavedConnection() const;

    /**
     * @brief  Gets the last recorded time the system was connected using this
     *         access point's connection.
     *
     * @return  The last connection time as the number of milliseconds since the
     *          Unix epoch, or 0 if no record exists of the system using this 
     *          access point's connection.
     */
    juce::int64 getLastConnectionTime() const;

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

    /**
     * @brief  Sets the AccessPoint data object's record of whether it has a
     *         compatible saved connection.
     *
     * @param isSaved  The value that hasSavedConnection() calls should return.
     */
    void setHasSavedConnection(const bool isSaved);

    /**
     * @brief  Stores the last time the system was connected to a network using
     *         a connection that is compatible with this access point.
     *
     * @param newTime  The new connection time to store, expressed as the number
     *                 of milliseconds since the Unix epoch.
     */
    void setLastConnectionTime(const juce::int64 newTime);

    /**
     * @brief  Checks if this data object has no valid data.
     *
     * @return  Whether this object contains no access point data.
     */
    bool isNull() const;

    /**
     * @brief  Gets a string representation of the AP data object for debug use.
     *
     * @return  A printable string describing this object.
     */
    juce::String toString() const;

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

    /* Tracks if a saved connection exists that is compatible with this
       access point. */
    juce::Atomic<unsigned int> connectionSaved;

    /* The last recorded time this access point was connected. */
    juce::Atomic<juce::int64> connectionTime;
};
