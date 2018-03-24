/**
 * @file WifiAccessPoint
 * 
 * WifiAccessPoint stores all relevant data about a single wifi access point.
 * Once created, access point data cannot be changed.
 */
#pragma once
#include "JuceHeader.h"

class WifiAccessPoint {
public:
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
     * Creates a null WifiAccessPoint, representing the absence of a 
     * wifi access point.
     */
    WifiAccessPoint();

    virtual ~WifiAccessPoint() { }

    /**
     * @return true iff this WifiAccessPoint is null.  All WifiAccessPoint
     *          objects created with the default constructor will be null.
     */
    bool isNull() const;

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator==(const WifiAccessPoint rhs) const {
        return hash == rhs.hash;
    };

    /**
     * Compares two WifiAccessPoint objects using their hash values.
     */
    bool operator!=(const WifiAccessPoint rhs) const {
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
        return requiresAuth;
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
    int signalStrength;
    bool requiresAuth;
    String hash;
};
