/**
 * @file WifiAccessPoint
 * 
 * WifiAccessPoint stores all relevant data about a single wifi access point.
 */
#pragma once

#define LINUX true //should only be explicitly defined within netbeans

#ifdef LINUX
#include <NetworkManager.h>
#endif

#include "JuceHeader.h"

class WifiAccessPoint
{
public:


#ifdef LINUX
    /**
     * @param accessPoint  A LibNM access point object.
     */
    WifiAccessPoint(NMAccessPoint* accessPoint);
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
     * Update the access point signal strength.
     */
    void updateSignalStrength();

    /**
     * @return  the wifi access point frequency.
     */
    unsigned long getFrequency();

    /**
     * @return  the maximum access point bitrate in kb/s
     */
    unsigned long getMaxBitrate();

#ifdef LINUX
    /**
     * Given a linked list of connections, return an array of all
     * connections that are compatible with this access point;
     * 
     * @param connections  A list of network connections.
     * 
     * @return            all connection in the list that can be activated
     *                     with this access point.
     */
    Array<NMConnection*> filterConnections(const GSList* connections);
    
    /**
     * @return true iff the connection could be activated with this access 
     *          point.
     */
    bool isValidConnection(NMConnection* connection);
#endif

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
     * @return the SSID identifying the access point. 
     */
    const String& getSSID() const
    {
        return ssid;
    }

    /**
     * @return the access point's BSSID
     */
    const String& getSSID() const
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

private:
    String ssid;
    String bssid;
    SecurityType security;
    String hash;

    int signalStrength;

#ifdef LINUX
    NMAccessPoint* nmAP = nullptr;
#endif

};
