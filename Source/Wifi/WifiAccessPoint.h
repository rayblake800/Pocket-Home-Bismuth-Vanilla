#pragma once
#include <atomic>
#include <NetworkManager.h>
#include "NMPPAccessPoint.h"
#include "JuceHeader.h"

/**
 * @file WifiAccessPoint
 * 
 * @brief Stores all relevant data about a single wifi access point.
 */

class WifiAccessPoint : public NMPPAccessPoint::Listener
{
public:
    
    /**
     * Create a null access point object.
     */
    WifiAccessPoint();
    
    /**
     * Create a wifi access point copying data from another access point.
     * 
     * @param toCopy  An access point object to copy.
     */
    WifiAccessPoint(const WifiAccessPoint& toCopy);

    /**
     * Create an access point object using LibNM access point data.
     * 
     * @param accessPoint      A LibNM access point object.
     */
    WifiAccessPoint(const NMPPAccessPoint& accessPoint);

    /**
     * Create a wifi access point from pre-generated data.  This is intended for
     * debugging purposes only.
     * 
     * @param ssid            The access point name.
     * 
     * @param signalStrength  Represents wifi signal strength as an integer
     *                         between 0 and 100.
     * 
     * @param requiresAuth    Sets if this access point requires a password to
     *                         connect
     * 
     * @param hash            A hashed string value unique to this access 
     *                         point.
     */
    WifiAccessPoint(String ssid, int signalStrength, bool requiresAuth,
            String hash);

    virtual ~WifiAccessPoint();
    
    /**
     * Checks if this object contains a valid wifi access point or not.
     * 
     * @return  true iff this object has no valid access point. 
     */
    bool isNull() const;

    /**
     * Gets the access point's primary name.
     * 
     * @return the SSID identifying the access point. 
     */
    const String& getSSID() const;

    /**
     * Gets the MAC address of the wifi access point.
     * 
     * @return the access point's BSSID
     */
    const String& getBSSID() const;

    /**
     * Gets the signal strength of the wifi access point.
     * 
     * @return the wifi signal strength, between 0 and 100 
     */
    unsigned int getSignalStrength() const;

    /**
     * Gets the wifi access point frequency in (TODO: what format? MHz? 
     * documentation is unclear, do some tests and figure it out.)
     * 
     * @return the wifi access point frequency.
     */
    unsigned long getFrequency() const;

    /**
     * Gets the access point's maximum data transfer bit rate.
     * 
     * @return the maximum access point bit rate in kb/s.
     */
    unsigned long getMaxBitrate() const;

    /**
     * Checks if this access point requires a security key.
     * 
     * @return true iff this access point requires a security key. 
     */
    bool getRequiresAuth() const;
    
    /**
     * Returns a string identifying this object for debug purposes.
     * 
     * @return the access point SSID, for debugging use only.
     */
    const String& toString() const;

    /**
     * Checks if this access point is compatible with a given connection.
     * 
     * @return  true iff the connection could be activated using this access
     *          point.
     */
    bool isConnectionCompatible(const NMPPConnection& connection) const;

    /**
     * Checks if this access point and another both share a network connection.
     * 
     * @param otherAP
     * 
     * @return true if their hash values match, and they don't both have saved
     *         nonmatching connection objects.
     */
    bool sharesConnectionWith(const WifiAccessPoint& otherAP) const;

    /**
     * Create a new connection object that could be used to connect with this
     * access point.
     * 
     * @param psk  The security key needed to connect to the access point.  If
     *             this access point is unsecured, this parameter will be
     *             ignored.
     * 
     * @return  a connection that can be used to connect with this access point,
     *          or a null connection object if this access point is null or
     *          the psk was invalid.
     */
    NMPPConnection createConnection(String psk = String()) const;
    
    /**
     * Attempts to add wireless security settings to a connection intended for
     * this access point.  This will fail if the security key is not valid for
     * the access point security type.
     * 
     * @param connection  The connection object to modify.
     * 
     * @param psk         The access point's security key.
     * 
     * @return  true iff the settings were applied successfully.
     */
    bool setConnectionSecurity
    (NMPPConnection& connection, const String& psk) const;

    /**
     * Returns the 802.11 mode of this access point.
     * 
     * @return the access point device mode 
     */
    NM80211Mode getMode() const;

    /**
     * Returns access point flags describing the access point's basic security
     * capabilities.
     * 
     * @return basic Wifi security flags 
     */
    NM80211ApFlags getFlags() const;

    /**
     * Returns flags describing the access point's WPA security capabilities. 
     * 
     * @return all WPA security flags. 
     */
    NM80211ApSecurityFlags getWPAFlags() const;

    /**
     * Returns flags describing the access point's RSN security capabilities.
     * 
     * @return all RSN security flags 
     */
    NM80211ApSecurityFlags getRSNFlags() const;
    
    /**
     * Checks if a psk is formatted correctly for this access point's security.
     * This will not check if the key is actually correct, just if it is a valid
     * length.
     * 
     * @param psk  A security key to test.
     * 
     * @return  true iff psk is a valid key for this access point's security
     *          type.  
     */
    bool isValidKeyFormat(const String& psk) const;
    
    /**
     * Assigns another access point's data to this access point.
     */
    WifiAccessPoint& operator=(const WifiAccessPoint& rhs);

    /**
     * Returns true iff this WifiAccessPoint has rhs as its nmAccessPoint.
     */
    bool operator==(const NMPPAccessPoint& rhs) const;

    /**
     * Returns true iff this WifiAccessPoint does not have rhs as its 
     * nmAccessPoint.
     */
    bool operator!=(const NMPPAccessPoint& rhs) const;

    /**
     * Returns true iff this access point and rhs share a nmAccessPoint.
     */
    bool operator==(const WifiAccessPoint& rhs) const;

    /**
     * Returns true iff this access point and rhs do not share a nmAccessPoint.
     */
    bool operator!=(const WifiAccessPoint& rhs) const;

    //LibNM data access methods:
    /**
     * @return this access point's LibNM access point object 
     */
    const NMPPAccessPoint& getNMAccessPoint() const;
   
    /**
     * Saves a path to a DBus active connection object that is using this
     * access point.
     * 
     * @param path  The active connection path value to store with this access 
     *              point.  The caller is responsible for ensuring this value
     *              is valid.
     */
    void setActiveConnectionPath(const String path);
    
    /**
     * Saves a path to a DBus saved connection object that is compatible with
     * this access point.
     * 
     * @param path  The saved connection path value to store with this access 
     *              point.  The caller is responsible for ensuring this value
     *              is valid.
     */
    void setSavedConnectionPath(const String path);
    
    /**
     * Returns the active connection path stored with this access point.
     * 
     * @return  the path value stored with setActiveConnectionPath()
     */
    const String& getActiveConnectionPath() const;
    
    /**
     * Returns the saved connection path stored with this access point.
     * 
     * @return  the path value stored with setSavedConnectionPath()
     */
    const String& getSavedConnectionPath() const;

private:
    /**
     * Signals that the access point's signal strength has been updated.
     * 
     * @param updatedAP    The updated wifi access point.
     * 
     * @param newStrength  The access point's new signal strength, ranging 
     *                     from zero to 100.
     */
    void signalStrengthChanged(NMPPAccessPoint* updatedAP,
            unsigned int newStrength) override;

    /**
     * Generates a hash value for a list of access point parameters that will
     * be unique to that access point's connection.
     */
    static String generateHash(
            const GByteArray* ssid,
            NM80211Mode mode,
            guint32 flags,
            guint32 wpa_flags,
            guint32 rsn_flags);
    
    /**
     * Represents the main categories of wifi access point security.
     */
    enum SecurityType
    {
        none,
        securedWEP,
        securedWPA,
        securedRSN
    };

    //Access point name
    String ssid;
    //Access point hardware ID string
    String bssid;
    //Access point security protocol
    SecurityType security;
    //Identifying hash unique to this APs connection
    String hash;

    //saved signal strength, between 0 and 100
    std::atomic<unsigned int> signalStrength;
    unsigned long frequency;
    unsigned long maxBitrate;

    NM80211Mode apMode;
    NM80211ApFlags apFlags;
    NM80211ApSecurityFlags wpaFlags;
    NM80211ApSecurityFlags rsnFlags;

    NMPPAccessPoint nmAccessPoint;
    String activeConnectionPath;
    String savedConnectionPath;
#if JUCE_DEBUG
    bool fakeConnection = false;
#endif
};
