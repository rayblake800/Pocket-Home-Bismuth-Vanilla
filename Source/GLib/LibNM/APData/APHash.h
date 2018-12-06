#pragma once
#include "JuceHeader.h"
#include "LibNM/LibNM.h"
#include <gio/gio.h>
#include <NetworkManager.h>

/**
 * @brief  Generates and holds a unique value to identify a Wifi connection 
 *         point. 
 *
 *  Access points will share an APHash if and only if both have the same SSID,
 * access point mode, and security settings. APHash values are immutable, and
 * should be used to compare and sort access point objects.
 *
 *  In most circumstances, access points with the same SSID, mode, and security 
 * settings are treated as interchangeable, to allow devices to easily stay 
 * connected to a network with multiple Wifi routers. However, it is possible 
 * for two access points to exist with the same APHash but different passwords, 
 * so this should be considered when handling connection failures.
 */
class LibNM::APHash
{
public:
    /**
     * @brief  Generates a Wifi access point identifier using the access point's
     *         SSID, mode, and security settings.
     *
     * @param ssid           The raw SSID bytestring of a Wifi access point. 
     *                       This may possibly contain unprintable characters.
     *
     * @param mode           The access point's mode of operation.
     *
     * @param securityType   Indicates the type of security, if any, used to
     *                       restrict access to the wifi access point.
     */
    APHash(const GByteArray* ssid,
        APMode mode,
        SecurityType securityType);

    /**
     * @brief  Creates an APHash as a copy of another hash value.
     *
     * @param toCopy  Another access point hash to copy.
     */
    APHash(const APHash& toCopy);

    /**
     * @brief  Checks if another APHash is equivalent to this one.
     *
     * @param rhs  Another hash value to compare with this APHash.
     *
     * @return     True if and only if both hash values are identical. 
     */
    bool operator==(const APHash& rhs) const;

    /**
     * @brief  Checks if another APHash is not equivalent to this one.
     *
     * @param rhs  Another hash value to compare with this APHash.
     *
     * @return     True if and only if both hash values are not identical. 
     */
    bool operator!=(const APHash& rhs) const;

    /**
     * @brief  Compares this APHash with another, so hash values can be sorted.
     *
     * @param rhs  Another hash value to compare with this APHash.
     *
     * @return     True if and only if this hash value comes before rhs when
     *             sorted alphabetically.
     */
    bool operator<(const APHash& rhs) const;

private:
    /* The fixed hash value. */
    juce::Identifier hashValue;
};
