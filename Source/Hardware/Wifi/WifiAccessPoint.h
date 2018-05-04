#pragma once
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
    WifiAccessPoint() { }
    
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
     * Represents the main categories of wifi access point security.
     */
    enum SecurityType
    {
        none,
        securedWEP,
        securedWPA,
        securedRSN
    };

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
     * Gets the access point's service set identifier, its primary name.
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
    int getSignalStrength() const;

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
     * @return  true iff the connection is compatible, and the WifiAccessPoint
     *          doesn't have a different non-null saved connection.  
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
    NMPPConnection createConnection(String psk = String());

    /**
     * @return the access point device mode 
     */
    NM80211Mode getMode() const;

    /**
     * @return basic Wifi security flags 
     */
    NM80211ApFlags getFlags() const;

    /**
     * @return all WPA security flags. 
     */
    NM80211ApSecurityFlags getWPAFlags() const;

    /**
     * @return all RSN security flags 
     */
    NM80211ApSecurityFlags getRSNFlags() const;
    
    /**
     * Assigns another access point's data to this access point.
     */
    bool operator=(const WifiAccessPoint& rhs);

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
     * Gets the network connection DBus path associated with this access point.
     * 
     * @return  The saved path value, or the empty string if there is no saved
     *          connection.
     */
    const String& getConnectionPath() const;
    
    /**
     * Stores a network connection DBus path associated with this access point.
     *  
     * @param path  A valid DBus path to store.
     */
    void setConnectionPath(String path);

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
    
    //Prevent concurrent access when being updated with new network data
    ReadWriteLock networkUpdateLock;

    //Access point name
    String ssid;
    //Access point hardware ID string
    String bssid;
    //Access point security strength
    SecurityType security;
    //Identifying hash unique to this APs connection
    String hash;

    //saved signal strength, between 0 and 100
    int signalStrength;
    unsigned long frequency;
    unsigned long maxBitrate;

    NM80211Mode apMode;
    NM80211ApFlags apFlags;
    NM80211ApSecurityFlags wpaFlags;
    NM80211ApSecurityFlags rsnFlags;

    NMPPAccessPoint nmAccessPoint;
    String connectionPath;
#if JUCE_DEBUG
    bool fakeConnection = false;
#endif
};
