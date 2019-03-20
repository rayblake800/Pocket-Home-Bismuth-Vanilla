#pragma once
/**
 * @file  Wifi_AccessPoint.h
 *
 * @brief  Holds data representing a wireless network visible through one or
 *         more nearby Wifi access points.
 */

#ifndef WIFI_IMPLEMENTATION
  #define WIFI_IMPLEMENTATION
  #define WIFI_TEMP
#endif
#include "Wifi_AP_Data.h"
#ifdef WIFI_TEMP
  #undef WIFI_IMPLEMENTATION
  #undef WIFI_TEMP
#endif

#include "Util_Nullable.h"
#include "JuceHeader.h"

namespace Wifi 
{ 
    class AccessPoint; 
    enum class SecurityType;
    namespace APList { class Module; }
    namespace Connection { namespace Saved { class Module; } }
    namespace Signal { class DeviceModule; }
    namespace LibNM
    {
        class SSID;
        class APHash;
        enum class SecurityType;
        class Connection;
        class AccessPoint;
    }
    
    /* Restricted property update interfaces used by the AccessPoint class: */
    namespace APInterface
    {
        class SignalStrength;
        class SavedConnection;
    }
}

/**
 * @brief  Allows only the APList::Module to update AccessPoint signal strength.
 */
class Wifi::APInterface::SignalStrength
{
public:
    virtual ~SignalStrength() { }
protected:
    SignalStrength() { }
    friend class APList::Module;
    virtual void setSignalStrength(const unsigned int newStrength) = 0;
};

/**
 * @brief  Allows only the Connection::Saved::Module and the 
 *         Signal::DeviceModule to update AccessPoint saved connection 
 *         state.
 */
class Wifi::APInterface::SavedConnection
{
public:
    virtual ~SavedConnection() { }
protected:
    SavedConnection() { }
    friend class Connection::Saved::Module;
    friend class Signal::DeviceModule;
    virtual void setHasSavedConnection(const bool isSaved) = 0;
    virtual void setLastConnectionTime(const juce::int64 newTime) = 0;
};

/**
 * @brief  Holds shared data describing a Wifi network and all visible access 
 *         points that share that network.
 *
 *  Wifi::AccessPoint represents one or more LibNM::AccessPoints, which in 
 * turn represent a Wifi access point found by NetworkManager through the 
 * network device. Wifi::AccessPoint objects hold shared, reference counted 
 * data. This allows access point data to be freely shared between threads, 
 * while keeping interaction with LibNM confined to the LibNM thread resource.
 *
 *  AccessPoint data is mostly immutable. Only the signal strength, saved
 * connection state, and last connection time may be updated.
 */
class Wifi::AccessPoint : 
    public Util::Nullable<juce::ReferenceCountedObjectPtr<AP::Data>>,
    public APInterface::SignalStrength, public APInterface::SavedConnection
{
public:

    /**
     * @brief  Creates the access point object from LibNM access point data.
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
    AccessPoint(const LibNM::SSID ssid, const LibNM::APHash apHash,
            const LibNM::SecurityType securityType, const int strength);

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
     * @brief  Checks if this AccessPoint represents a particular 
     *         LibNM::AccessPoint.
     *
     * @param rhs  A LibNM access point to check against this AccessPoint.
     *
     * @return     Whether rhs is one of the LibNM::AccessPoint objects
     *             represented by this AccessPoint.
     */
    bool operator==(const LibNM::AccessPoint& rhs) const;
    
    /**
     * @brief  Checks if this AccessPoint does not represent a particular 
     *         LibNM::AccessPoint.
     *
     * @param rhs  A LibNM access point to check against this AccessPoint.
     *
     * @return     Whether rhs is not one of the LibNM::AccessPoint objects
     *             represented by this AccessPoint.
     */
    bool operator!=(const LibNM::AccessPoint& rhs) const;

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
     * @brief  Checks whether this access point is compatible with a saved 
     *         network connection.
     *
     * @return  Whether any saved network connection is compatible with this
     *          access point.
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
     * @brief  Gets a string representation of the AccessPoint for debug use.
     *
     * @return  A printable string describing this object.
     */
    juce::String toString() const;

protected:
    /**
     * @brief  Updates the access point's signal strength.
     *
     * @param newStrength  The new signal strength, as a percentage value
     *                     between 0 and 100.
     */
    virtual void setSignalStrength(const unsigned int newStrength) override;

    /**
     * @brief  Sets the AccessPoint object's record of whether it has a
     *         compatible saved connection.
     *
     * @param isSaved  The value that hasSavedConnection() calls should return.
     */
    virtual void setHasSavedConnection(const bool isSaved) override;

    /**
     * @brief  Stores the last time the system was connected to a network using
     *         a connection that is compatible with this access point.
     *
     * @param newTime  The new connection time to store, expressed as the number
     *                 of milliseconds since the Unix epoch.
     */
    virtual void setLastConnectionTime(const juce::int64 newTime) override;
};
