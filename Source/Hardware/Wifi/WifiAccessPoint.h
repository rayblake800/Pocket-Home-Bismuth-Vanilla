/**
 * @file WifiAccessPoint
 * 
 * WifiAccessPoint stores all relevant data about a single wifi access point.
 */
#pragma once
#include <NetworkManager.h>
#include <nm-access-point.h>
#include <nm-connection.h>
#include "JuceHeader.h"

class WifiAccessPoint : public ReferenceCountedObject
{
public:
    typedef ReferenceCountedObjectPtr<WifiAccessPoint> Ptr;
    
    /**
     * Create an access point object using LibNM access point data and optional
     * saved connection data.
     * 
     * @param accessPoint      A LibNM access point object.
     * 
     * @param savedConnection  A saved connection that is compatible with this 
     *                         access point, or nullptr if none is available.
     */
    WifiAccessPoint(NMAccessPoint* accessPoint,
            NMConnection* savedConnection = nullptr);

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

    /**
     * Unregisters the signal handler, if one exists
     */
    virtual ~WifiAccessPoint();

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
     * Checks if this access point corresponds to a saved network connection.
     */
    bool hasSavedConnection() const;

    /**
     * Checks if there is a psk for this connection saved by the network
     * manager.
     */
    bool hasSavedPsk() const;

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
    bool isConnectionCompatible(NMConnection* connection) const;
    
    /**
     * Checks if this access point and another both share a network
     * 
     * @param otherAP
     * 
     * @return true if their hash values match, and they don't both have saved
     *         nonmatching connection objects.
     */
    bool sharesConnectionWith(WifiAccessPoint::Ptr otherAP) const;

      
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
     * Returns true iff this WifiAccessPoint has rhs as its nmAccessPoint.
     */
    bool operator==(NMAccessPoint* rhs) const;

    /**
     * Returns true iff this WifiAccessPoint does not have rhs as its 
     * nmAccessPoint.
     */
    bool operator!=(NMAccessPoint* rhs) const;

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
    NMAccessPoint* getNMAccessPoint();
    
    /**
     * Gets this access point's connection object.  If it has no connection
     * object, a new one will be created and returned.
     * 
     * @return this access point's connection object.
     */
    NMConnection* getNMConnection();
    
    /**
     * Assigns a new connection to this access point. This will fail if a valid
     * connection is already assigned, or if the new connection is not valid
     * for this access point.
     *
     * @param newConnection
     * 
     * @return true iff the new connection was set.
     */
    bool setNMConnection(NMConnection* newConnection);
    
    /**
     * Saves a new security key for this access point's connection.
     * 
     * @param psk  The new security key.  This key will only be saved if it is
     *             valid for the access point's security type.
     */
    void setPsk(String psk);
    
    /**
     * Removes the security key saved for this access point.  This should only 
     * be done when the saved key turns out to be invalid. 
     */
    void removePsk();

private:
    /**
     * If the NMAccessPoint is non-null and signal handlers aren't already
     * registered, this will register this object to receive updates when
     * access point signal strength changes or the NMAccessPoint is destroyed.
     * 
     * This method assumes that the WifiAccessPoint is already locked for
     * writing.
     */
    void registerSignalHandlers();
    
    /**
     * Gets an SSID byte array from the saved connection or access point.  If
     * possible, the value from the saved connection will be used first.
     */
    const GByteArray* getSSIDBytes
    (NMAccessPoint* ap, NMConnection* conn = nullptr);

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
     * If this object's NMAccessPoint or NMConnection is deleted, this will 
     * remove the pointer to the deleted object and set it to null.  This should
     * only be called on the GLib event loop by GLib library code.
     * 
     * @param toUpdate  A WifiAccessPoint that is holding a pointer to the 
     *                  object that will be deleted.
     * 
     * @param removed   This should be either the WifiAccessPoint's 
     *                  NMAccessPoint, or its NMConnection.
     */
    static void apDestroyedCallback
    (WifiAccessPoint* toUpdate, GObject* removed);

    /**
     * A callback to update signal strength whenever NMAccessPoint strength
     * changes.  This should only be called on the GLib event loop by GLib 
     * library code.
     * 
     * @param toUpdate
     */
    static void strengthUpdateCallback(WifiAccessPoint* toUpdate);

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
    gulong updateSignalId = 0;
    
    NMAccessPoint* nmAccessPoint = nullptr;
    NMConnection*  networkConnection = nullptr;
};
