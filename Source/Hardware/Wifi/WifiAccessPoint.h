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

    bool operator==(const WifiAccessPoint rhs) const {
        return hash == rhs.hash;
    };

    bool operator!=(const WifiAccessPoint rhs) const {
        return hash != rhs.hash;
    };

    const String& getSSID() const
    {
        return ssid;
    }

    int getSignalStrength() const
    {
        return signalStrength;
    }
    
    bool getRequiresAuth() const
    {
        return requiresAuth;
    }
    
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
