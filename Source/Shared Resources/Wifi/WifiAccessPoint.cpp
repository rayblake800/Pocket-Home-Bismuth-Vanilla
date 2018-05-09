#include "nm-utils.h"
#include "JuceHeader.h"
#include "Utils.h"
#include "GLibSignalHandler.h"
#include "WifiAccessPoint.h"
#include "NMPPAccessPoint.h"
    

    
/*
 * Create a wifi access point copying data from another access point.
 */
WifiAccessPoint::WifiAccessPoint(const WifiAccessPoint& toCopy)
{
    *this = toCopy;}

/*
 * Create an access point object using LibNM access point data.
 */
WifiAccessPoint::WifiAccessPoint(const NMPPAccessPoint& accessPoint) : 
nmAccessPoint(accessPoint)
{
    if(nmAccessPoint.isNull())
    {
        return;
    }
    ssid           = nmAccessPoint.getSSIDText();
    bssid          = nmAccessPoint.getBSSID();
    apMode         = nmAccessPoint.getMode();
    apFlags        = nmAccessPoint.getFlags();
    wpaFlags       = nmAccessPoint.getWPAFlags();
    rsnFlags       = nmAccessPoint.getRSNFlags();
    hash           = generateHash(nmAccessPoint.getSSID(),
                            apMode,apFlags,wpaFlags,rsnFlags);
    signalStrength = nmAccessPoint.getSignalStrength();
    frequency      = nmAccessPoint.getFrequency();
    maxBitrate     = nmAccessPoint.getMaxBitrate();
    //get security type
    if (rsnFlags != NM_802_11_AP_SEC_NONE)
    {
        security = securedRSN;
    }
    else if (wpaFlags != NM_802_11_AP_SEC_NONE)
    {
        security = securedWPA;
    }
    else
    {
        security = (apFlags == NM_802_11_AP_FLAGS_NONE) ?
                none : securedWEP;
    }
    nmAccessPoint.addSignalHandler(this);
}

    
/*
 * Create a wifi access point from pre-generated data.  This is intended for
 * debugging purposes only.
 */
WifiAccessPoint::WifiAccessPoint
(String ssid, int signalStrength, bool requiresAuth, String hash) :
ssid(ssid),
signalStrength(median<int>(0, signalStrength, 100)),
hash(hash)
{
    
#if JUCE_DEBUG
    fakeConnection = true;
#endif
    security = requiresAuth ? securedWPA : none;
}


WifiAccessPoint::~WifiAccessPoint() { }
    
/*
 * Checks if this object contains a valid wifi access point or not.
 */
bool WifiAccessPoint::isNull() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    
#if JUCE_DEBUG
    if(fakeConnection)
    {
        return ssid.isEmpty();
    }
#endif
    return nmAccessPoint.isNull();
}

/*
 * Gets the access point's service set identifier, its primary name.
 */
const String& WifiAccessPoint::getSSID() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return ssid;
}

/*
 * Gets the MAC address of the wifi access point.
 */
const String& WifiAccessPoint::getBSSID() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return bssid;
}

/*
 * Gets the signal strength of the wifi access point.
 */
int WifiAccessPoint::getSignalStrength() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return signalStrength;
}

/*
 * Gets the wifi access point frequency in (TODO: what format? MHz? 
 * documentation is unclear, do some tests and figure it out.)
 */
unsigned long WifiAccessPoint::getFrequency() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return frequency;
}

/*
 * Gets the access point's maximum data transfer bit rate.
 */
unsigned long WifiAccessPoint::getMaxBitrate() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return maxBitrate;
}
 
/*
 * Checks if this access point requires a security key.
 */
bool WifiAccessPoint::getRequiresAuth() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return security != none;
}

/*
 * Returns a string identifying this object for debug purposes.
 */
const String& WifiAccessPoint::toString() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return ssid;
}
        
/*
 * Checks if this access point is compatible with a given connection.
 */
bool WifiAccessPoint::isConnectionCompatible
(const NMPPConnection& connection) const
{
    return !isNull() && !connection.isNull()
	    && nmAccessPoint.isValidConnection(connection);
}
    
/*
 * Checks if this access point and another both share a network
 */
bool WifiAccessPoint::sharesConnectionWith(const WifiAccessPoint& otherAP) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return hash == otherAP.hash;
}

/*
 * Create a new connection object that could be used to connect with this
 * access point.
 */
NMPPConnection WifiAccessPoint::createConnection(String psk) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    NMPPConnection newConnection;
    newConnection.addWifiSettings(nmAccessPoint.getSSID());
    if(!setConnectionSecurity(newConnection, psk))
    {
        return NMPPConnection();
    }
    return newConnection;
}
 
/*
 * Attempts to add wireless security settings to a connection intended for
 * this access point.  This will fail if the security key is not valid for 
 * the access point security type.
 */
bool WifiAccessPoint::setConnectionSecurity
(NMPPConnection& connection, const String& psk) const
{
    if(security == securedWEP)
    {
        if(!connection.addWEPSettings(psk))
        {
            DBG("WifiAccessPoint::" << __func__ 
                    << ": failed to set WEP key.");
            return false;
        }
    }
    else if(security != none)
    {
        if(!connection.addWPASettings(psk))
        {
            DBG("WifiAccessPoint::" << __func__ 
                    << ": failed to set WPA key.");
            return false;         
        }
    }
    return true;
}

/*
 * @return the access point device mode 
 */
NM80211Mode WifiAccessPoint::getMode() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return apMode;
}

/*
 * @return basic Wifi security flags 
 */
NM80211ApFlags WifiAccessPoint::getFlags() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return apFlags;
}

/*
 * @return all WPA security flags. 
 */
NM80211ApSecurityFlags WifiAccessPoint::getWPAFlags() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return wpaFlags;
}

/*
 * @return all RSN security flags 
 */
NM80211ApSecurityFlags WifiAccessPoint::getRSNFlags() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return rsnFlags;
}

  
/*
 * Checks if a psk is formatted correctly for this access point's security.
 * This will not check if the key is actually correct, just if it is a valid
 * length.
 */
bool WifiAccessPoint::isValidKeyFormat(const String& psk) const
{
    int length = psk.length();
    switch(security)
    {
        case none:
            return length == 0;
        case securedWEP:
            return length == 10 || length == 26 || length == 5 || length == 13;
        case securedWPA:
        case securedRSN:
            return length >= 8;
    }
}
    
/*
 * Assigns another access point's data to this access point.
 */
bool WifiAccessPoint::operator=(const WifiAccessPoint& rhs)
{
    const ScopedReadLock readLock(rhs.networkUpdateLock);
    
    ssid           = rhs.ssid;
    bssid          = rhs.bssid;
    security       = rhs.security;
    hash           = rhs.hash;
    signalStrength = rhs.signalStrength;
    frequency      = rhs.frequency;
    maxBitrate     = rhs.maxBitrate;
    apMode         = rhs.apMode;
    apFlags        = rhs.apFlags;
    wpaFlags       = rhs.wpaFlags;
    rsnFlags       = rhs.rsnFlags;
    nmAccessPoint  = rhs.nmAccessPoint;
    connectionPath = rhs.connectionPath;
#if JUCE_DEBUG
    fakeConnection = rhs.fakeConnection;
#endif
    if(!nmAccessPoint.isNull())
    {
        nmAccessPoint.addSignalHandler(this);
    }
}

/*
 * Returns true iff this WifiAccessPoint has rhs as its nmAccessPoint.
 */
bool WifiAccessPoint::operator==(const NMPPAccessPoint& rhs) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return nmAccessPoint == rhs;
}

/*
 * Returns true iff this WifiAccessPoint does not have rhs as its 
 * nmAccessPoint.
 */
bool WifiAccessPoint::operator!=(const NMPPAccessPoint& rhs) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return nmAccessPoint != rhs;
}


/*
 * Returns true iff this access point and rhs share a nmAccessPoint.
 */
bool WifiAccessPoint::operator==(const WifiAccessPoint& rhs) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    const ScopedReadLock rhsLock(rhs.networkUpdateLock);
    return hash == rhs.hash;
}

/*
 * Returns true iff this access point and rhs do not share a nmAccessPoint.
 */
bool WifiAccessPoint::operator!=(const WifiAccessPoint& rhs) const
{
    return !(*this == rhs);
}

/*
 * @return this access point's LibNM access point object 
 */
const NMPPAccessPoint& WifiAccessPoint::getNMAccessPoint() const
{
    return nmAccessPoint;
}
 
/**
 * Gets the network connection DBus path associated with this access point.
 * 
 * @return  The saved path value, or the empty string if there is no saved
 *          connection.
 */
const String& WifiAccessPoint::getConnectionPath() const
{
    return connectionPath;
}

/**
 * Stores a network connection DBus path associated with this access point.
 *  
 * @param path  A valid DBus path to store.
 */
void WifiAccessPoint::setConnectionPath(String path)
{
    connectionPath = path;
}

/*
 * Signals that the access point's signal strength has been updated.
 */
void WifiAccessPoint::signalStrengthChanged(NMPPAccessPoint* updatedAP,
        unsigned int newStrength)
{
    const ScopedWriteLock updateLock(networkUpdateLock);
    signalStrength = newStrength;
}


/*
 * Generates a hash value for a list of access point parameters that will
 * be unique to that access point's connection.
 */
String WifiAccessPoint::generateHash(
        const GByteArray* ssid,
        NM80211Mode mode,
        guint32 flags,
        guint32 wpa_flags,
        guint32 rsn_flags)
{
    unsigned char input[66];
    memset(&input[0], 0, sizeof (input));
    if (ssid != nullptr)
    {
        memcpy(input, ssid->data, ssid->len);
    }
    if (mode == NM_802_11_MODE_INFRA)
    {
        input[32] |= (1 << 0);
    }
    else if (mode == NM_802_11_MODE_ADHOC)
    {
        input[32] |= (1 << 1);
    }
    else
    {
        input[32] |= (1 << 2);
    }
    /* Separate out no encryption, WEP-only, and WPA-capable */
    if (!(flags & NM_802_11_AP_FLAGS_PRIVACY)
        && (wpa_flags == NM_802_11_AP_SEC_NONE)
        && (rsn_flags == NM_802_11_AP_SEC_NONE))
    {
        input[32] |= (1 << 3);
    }
    else if ((flags & NM_802_11_AP_FLAGS_PRIVACY)
             && (wpa_flags == NM_802_11_AP_SEC_NONE)
             && (rsn_flags == NM_802_11_AP_SEC_NONE))
    {
        input[32] |= (1 << 4);
    }
    else if (!(flags & NM_802_11_AP_FLAGS_PRIVACY)
             && (wpa_flags != NM_802_11_AP_SEC_NONE)
             && (rsn_flags != NM_802_11_AP_SEC_NONE))
    {
        input[32] |= (1 << 5);
    }
    else
    {
        input[32] |= (1 << 6);
    }
    /* duplicate it */
    memcpy(&input[33], &input[0], 32);
    return g_compute_checksum_for_data
            (G_CHECKSUM_MD5, input, sizeof (input));
}
