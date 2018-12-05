#pragma once
#include "Wifi/Wifi.h"
#include "Util/Nullable.h"
#include "JuceHeader.h"

/**
 * @file  Wifi/SavedState/AccessPoint
 *
 * @brief  Holds shared data describing a wifi access point.
 *
 *  Wifi::AccessPoint represents a LibNM::AccessPoint, which in turn represents
 * a Wifi access point found by NetworkManager through the network device. 
 * Wifi::AccessPoint objects hold shared, reference counted data. This allows
 * access point data to be freely shared between threads, while keeping 
 * interaction with LibNM confined to the LibNM thread resource.
 *
 *  AccessPoint data is mostly immutable. Only the signal strength may be 
 * updated, and only the APList may update it.
 */

namespace Wifi { class APData; }
namespace LibNM { class AccessPoint; }
namespace LibNM { class APHash; }
namespace LibNM { enum class SecurityType; }

class Wifi::AccessPoint : 
    public Nullable<juce::ReferenceCountedObjectPtr<APData>>
{
public:
    /* Only the APList may update access point data. */
    friend class APList;

    /**
     * @brief  Creates new access point data from a LibNM access point object.
     *
     * @param nmAccessPoint  The LibNM access point the data should represent.
     */
    AccessPoint(const LibNM::AccessPoint nmAccessPoint);

    /**
     * @brief  Initializes the AccessPoint with another AccessPoint's data.
     *
     * @param rhs  An access point that will share data with the new 
     *             AccessPoint.
     */
    AccessPoint(const AccessPoint& rhs);

    /**
     * @brief  Creates a null access point.
     */
    AccessPoint() { }

    virtual ~AccessPoint() { }

    /**
     * @brief  Compares AccessPoint objects using their hash values.
     *
     * @param rhs  The AccessPoint object to compare with this one.
     *
     * @return     Whether this and rhs have identical hash values.
     */
    bool operator==(const AccessPoint& rhs) const;
    
    /**
     * @brief  Compares AccessPoint objects using their hash values.
     *
     * @param rhs  The AccessPoint object to compare with this one.
     *
     * @return     Whether this and rhs do not have identical hash values.
     */
    bool operator!=(const AccessPoint& rhs) const;
    
    /**
     * @brief  Compares AccessPoint objects using their hash values so they can 
     *         be sorted.
     *
     * @param rhs  The AccessPoint object to compare with this one.
     *
     * @return     Whether this access point's hash value should come before the 
     *             rhs AccessPoint's hash value.
     */
    bool operator<(const AccessPoint& rhs) const;
    
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

private:
    /**
     * @brief  Updates the access point's signal strength.
     *
     * @param newStrength  The new signal strength, as a percentage value
     *                     between 0 and 100.
     */
    void setSignalStrength(const unsigned int newStrength);
};


