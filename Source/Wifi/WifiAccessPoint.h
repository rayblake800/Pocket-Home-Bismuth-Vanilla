/**
 * @file WifiAccessPoint
 * 
 * WifiAccessPoint stores all relevant data about a single wifi access point.
 * Once created, access point data cannot be changed.
 */
#pragma once

class WifiAccessPoint {
public:
    /**
     * @param ssid access point name
     * @param signalStrength represents wifi signal strength as an integer
     * between 0 and 100
     * @param requiresAuth true iff this access point requires a password to
     * connect
     * @param hash a hashed string value unique to this access point.
     */
    WifiAccessPoint(String ssid, int signalStrength, bool requiresAuth,
            String hash);
    
    /**
     * Creates a null WifiAccessPoint, that represents the absence of a 
     * wifi access Point.
     */
    WifiAccessPoint();

    virtual ~WifiAccessPoint();

    /**
     * @return a WifiAccessPoint object that represents the absence of a
     * wifi access point.
     */
    static const WifiAccessPoint null();

    /**
     * @return true iff this WifiAccessPoint is null.  All WifiAccessPoint
     * objects created with the default constructor will be null.
     */
    bool isNull() const;

    bool operator==(const WifiAccessPoint rhs) const {
        return hash == rhs.hash;
    };

    bool operator!=(const WifiAccessPoint rhs) const {
        return hash != rhs.hash;
    };

    const String& getSSID() const;

    int getSignalStrength() const;
    
    bool getRequiresAuth() const;
    
    const String& getHash() const;
private:
    String ssid;
    int signalStrength;
    bool requiresAuth;
    String hash;
};
