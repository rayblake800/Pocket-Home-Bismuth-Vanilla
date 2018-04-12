/**
 * @file WifiAccessPoint
 * 
 * WifiAccessPoint stores all relevant data about a single wifi access point.
 */
#pragma once

#ifdef LINUX
#include <NetworkManager.h>
#include <nm-access-point.h>
#include <nm-connection.h>
#endif

#include "JuceHeader.h"

class WifiAccessPoint
{
public:


#ifdef LINUX
    /**
     * @param accessPoint     A LibNM access point object.
     * 
     * @param savedConnection  A saved connection that is compatible with this 
     *                         access point, or nullptr if none is available.
     */
    WifiAccessPoint(NMAccessPoint* accessPoint,
            NMConnection* savedConnection = nullptr);
#endif  

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

    virtual ~WifiAccessPoint() { }

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
     * @return  the wifi access point frequency.
     */
    unsigned long getFrequency();

    /**
     * @return  the maximum access point bitrate in kb/s
     */
    unsigned long getMaxBitrate();

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator==(const WifiAccessPoint rhs) const
    {
        return hash == rhs.hash;
    };

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator!=(const WifiAccessPoint rhs) const
    {
        return hash != rhs.hash;
    };

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator<(const WifiAccessPoint rhs) const
    {
        return hash.compare(rhs.hash) < 0;
    }

    /**
     * @return the SSID identifying the access point. 
     */
    const String& getSSID() const
    {
        return ssid;
    }

    /**
     * @return the access point's BSSID
     */
    const String& getBSSID() const
    {
        return bssid;
    }

    /**
     * @return the wifi signal strength, between 0 and 100 
     */
    int getSignalStrength() const
    {
        return signalStrength;
    }

    /**
     * @return the wifi access point frequency.
     */
    unsigned long getFrequency() const
    {
        return frequency;
    }

    /**
     * @return the maximum access point bit rate in kb/s.
     */
    unsigned long getMaxBitrate() const
    {
        return maxBitrate;
    }

    /**
     * @return true iff this access point requires a security key. 
     */
    bool getRequiresAuth() const
    {
        return security != none;
    }

    /**
     * @return a unique hash value that can be used to identify this access
     *          point.
     */
    const String& getHash() const
    {
        return hash;
    }

    /**
     * @return the access point SSID, for debugging use only.
     */
    const String& toString() const
    {
        return ssid;
    }

    /**
     * Checks if this access point corresponds to a saved network connection.
     */
    bool isSavedConnection() const
    {
        return connectionSaved;
    }

    /**
     * Checks if there is a psk for this connection saved by the network
     * manager.
     */
    bool hasSavedPsk() const
    {
        return pskSaved;
    }

    /**
     * Sets if this connection is known to be saved by the network manager.
     */
    void setSaved(bool isSaved)
    {
        connectionSaved = isSaved;
    }

    /**
     * Sets if this connection is known to have a psk saved with the network
     * manager.
     */
    bool setPskSaved(bool hasPsk)
    {
        pskSaved = hasPsk;
    }
    
    /**
     * Updates the access point signal strength.
     *  
     * @param strength  This should be between 0 and 100, read from the network
     *                  manager.
     */
    void setSignalStrength(int strength){
        signalStrength = strength;
    }

#ifdef LINUX

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator==(NMAccessPoint* rhs) const
    {
        return hash == generateHash(rhs);
    };

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator!=(NMAccessPoint* rhs) const
    {
        return hash != generateHash(rhs);
    };

    NM80211Mode getMode()
    {
        return apMode;
    }

    NM80211ApFlags getFlags()
    {
        return apFlags;
    }

    NM80211ApSecurityFlags getWPAFlags()
    {
        return wpaFlags;
    }

    NM80211ApSecurityFlags getRSNFlags()
    {
        return rsnFlags;
    }
#endif

private:
    String ssid;
    String bssid;
    SecurityType security;
    String hash;

    int signalStrength;
    unsigned long frequency;
    unsigned long maxBitrate;

    bool connectionSaved = false;
    bool pskSaved = false;

#ifdef LINUX

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
    NM80211Mode apMode;
    NM80211ApFlags apFlags;
    NM80211ApSecurityFlags wpaFlags;
    NM80211ApSecurityFlags rsnFlags;
    
    static void strengthUpdateCallback(WifiAccessPoint* self);
    
    NMAccessPoint* nmAP = nullptr;
#endif

};
