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

class WifiAccessPoint
{
public:

    /**
     * @param accessPoint     A LibNM access point object.
     * 
     * @param savedConnection  A saved connection that is compatible with this 
     *                         access point, or nullptr if none is available.
     */
    WifiAccessPoint(NMAccessPoint* accessPoint,
            NMConnection* savedConnection = nullptr);

    /**
     * Copies an existing access point object, registering a new signal 
     * handler for the new access point instance.
     * 
     * @param toCopy
     */
    WifiAccessPoint(const WifiAccessPoint& toCopy);


    /**
     * Unregisters the signal handler, if one exists
     */
    virtual ~WifiAccessPoint();

    /**
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
     * Creates a void WifiAccessPoint, representing the absence of a 
     * wifi access point.
     */
    WifiAccessPoint();

    enum SecurityType
    {
        none,
        securedWEP,
        securedWPA,
        securedRSN
    };

    /**
     * @return true iff this WifiAccessPoint is void.  All WifiAccessPoint
     *          objects created with the default constructor will be void.
     */
    bool isVoid() const;

    /**
     * @return the SSID identifying the access point. 
     */
    const String& getSSID() const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return ssid;
    }

    /**
     * @return the access point's BSSID
     */
    const String& getBSSID() const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return bssid;
    }

    /**
     * @return the wifi signal strength, between 0 and 100 
     */
    int getSignalStrength() const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return signalStrength;
    }

    /**
     * @return the wifi access point frequency.
     */
    unsigned long getFrequency() const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return frequency;
    }

    /**
     * @return the maximum access point bit rate in kb/s.
     */
    unsigned long getMaxBitrate() const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return maxBitrate;
    }

    /**
     * @return true iff this access point requires a security key. 
     */
    bool getRequiresAuth() const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return security != none;
    }

    /**
     * @return a unique hash value that can be used to identify this access
     *          point.
     */
    const String& getHash() const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return hash;
    }

    /**
     * @return the access point SSID, for debugging use only.
     */
    const String& toString() const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return ssid;
    }

    /**
     * Checks if this access point corresponds to a saved network connection.
     */
    bool isSavedConnection() const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return connectionSaved;
    }

    /**
     * Checks if there is a psk for this connection saved by the network
     * manager.
     */
    bool hasSavedPsk() const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return pskSaved;
    }

    /**
     * Sets if this connection is known to be saved by the network manager.
     */
    void setSaved(bool isSaved)
    {
        const ScopedWriteLock updateLock(networkUpdateLock);
        connectionSaved = isSaved;
    }

    /**
     * Sets if this connection is known to have a psk saved with the network
     * manager.
     */
    bool setPskSaved(bool hasPsk)
    {
        const ScopedWriteLock updateLock(networkUpdateLock);
        pskSaved = hasPsk;
    }

    /**
     * Updates the access point signal strength.
     *  
     * @param strength  This should be between 0 and 100, read from the network
     *                  manager.
     */
    void setSignalStrength(int strength)
    {
        const ScopedReadLock readLock(networkUpdateLock);
        signalStrength = strength;
    }
    
    /**
     * Copies over all data from rhs, creating new NMAccessPoint signal 
     * handlers if necessary. 
     * 
     * @param rhs
     */
    void operator=(const WifiAccessPoint& rhs);

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator==(NMAccessPoint* rhs) const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return nmAP == rhs || hash == generateHash(rhs);
    };

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator!=(NMAccessPoint* rhs) const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return nmAP != rhs || hash != generateHash(rhs);
    };

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator==(const WifiAccessPoint& rhs) const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return nmAP == rhs.nmAP || hash == rhs.hash;
    };

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator!=(const WifiAccessPoint& rhs) const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return hash != rhs.hash;
    };
    
    /**
     * Checks if this access point is compatible with a network connection.
     */
    bool operator==(NMConnection* rhs) const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return nmAP != nullptr && nm_access_point_connection_valid(nmAP,rhs);
    }
     bool operator!=(NMConnection* rhs) const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return nmAP == nullptr || !nm_access_point_connection_valid(nmAP,rhs);
    }

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator<(const WifiAccessPoint rhs) const
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return hash.compare(rhs.hash) < 0;
    }

    NM80211Mode getMode()
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return apMode;
    }

    NM80211ApFlags getFlags()
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return apFlags;
    }

    NM80211ApSecurityFlags getWPAFlags()
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return wpaFlags;
    }

    NM80211ApSecurityFlags getRSNFlags()
    {
        const ScopedReadLock readLock(networkUpdateLock);
        return rsnFlags;
    }

private:
    /**
     * Gets an SSID byte array from a saved connection or access point.  If
     * possible, the value from the saved connection will be used first.
     * 
     * @param ap    If the connection was null or had no SSID, the SSID will be
     *              read from this libNM access point.
     * 
     * @param conn  A saved connection, or nullptr.
     * 
     * @return      The SSID byte array, or nullptr if no SSID was found.
     */
    static const GByteArray* getSSIDBytes
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
     * Generates a hash value for a NMAccessPoint that will be unique to that
     * access point's connection.
     * 
     * @param ap     A libNM access point.
     * 
     * @param conn   A saved connection compatible with this access point, or
     *               nullptr if no such connection is known.
     */
    static String generateHash(NMAccessPoint* ap, NMConnection* conn = nullptr);

    /**
     * A callback to remove the stored NMAccessPoint if it is destroyed.  This
     * should only be called on the GLib event loop by GLib library code.
     * 
     * @param toUpdate
     * @param removed
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

    int signalStrength;
    unsigned long frequency;
    unsigned long maxBitrate;

    bool connectionSaved = false;
    bool pskSaved = false;

    NM80211Mode apMode;
    NM80211ApFlags apFlags;
    NM80211ApSecurityFlags wpaFlags;
    NM80211ApSecurityFlags rsnFlags;
    gulong updateSignalId = 0;
    NMAccessPoint* nmAP = nullptr;

};
