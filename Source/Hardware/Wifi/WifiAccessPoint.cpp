#include "nm-utils.h"
#include "JuceHeader.h"
#include "Utils.h"
#include "GLibSignalHandler.h"
#include "WifiAccessPoint.h"
#include "LibNM/NMPPAccessPoint.h"
    
/*
 * Create an access point object using LibNM access point data.
 */
WifiAccessPoint::WifiAccessPoint(NMPPAccessPoint accessPoint) : 
nmAccessPoint(accessPoint)
{
    if(nmAccessPoint.isNull())
    {
        return;
    }
    ssid = nmAccessPoint.getSSIDText();
    bssid = nmAccessPoint.getBSSID();
    apMode = nmAccessPoint.getMode();
    apFlags = nmAccessPoint.getFlags();
    wpaFlags = nmAccessPoint.getWPAFlags();
    rsnFlags = nmAccessPoint.getRSNFlags();
    hash = generateHash(nmAccessPoint.getSSID(),
            apMode,apFlags,wpaFlags,rsnFlags);
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
    signalStrength = nmAccessPoint.getSignalStrength()
    frequency = nmAccessPoint.getFrequency();
    maxBitrate = nmAccessPoint.getMaxBitrate();
    nmAccessPoint.addListener(this);
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
    security = requiresAuth ? securedWPA : none;
}
    
/*
 * Checks if this object contains a valid wifi access point or not.
 */
bool WifiAccessPoint::isNull() const
{
    const ScopedReadLock readLock(networkUpdateLock);
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
bool WifiAccessPoint::isConnectionCompatible(NMPPConnection connection) const
{
    return nmAccessPoint.isValidConnection(connection);
}
    
/*
 * Checks if this access point and another both share a network
 */
bool WifiAccessPoint::sharesConnectionWith(const WifiAccessPoint& otherAP) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return hash == otherAP->hash;
}

/*
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
NMPPConnection WifiAccessPoint::createConnection(String psk)
{
    const ScopedReadLock readLock(networkUpdateLock);
    NMPPConnection newConnection;
    if(security == securedWEP)
    {
        if(!newConnection.addWEPSettings(psk))
        {
            DBG("NMPPConnection::" << __func__ 
                    << ": failed to create connection.");
            return newConnection;
        }
    }
    else if(security != none)
    {
        if(!newConnection.addWPASettings(psk))
        {
            DBG("NMPPConnection::" << __func__ 
                    << ": failed to create connection.");
            return newConnection;         
        }
    }
    newConnection.addWifiSettings(nmAccessPoint.getSSID());
    return newConnection;
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
 * Returns true iff this WifiAccessPoint has rhs as its nmAccessPoint.
 */
bool WifiAccessPoint::operator==(NMPPAccessPoint rhs) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return nmAccessPoint == rhs;
}

/*
 * Returns true iff this WifiAccessPoint does not have rhs as its 
 * nmAccessPoint.
 */
bool WifiAccessPoint::operator!=(NMPPAccessPoint rhs) const
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
const NMPPAccessPoint& WifiAccessPoint::getNMAccessPoint()
{
    return nmAccessPoint;
}
 
/**
 * Gets the network connection DBus path associated with this access point.
 * 
 * @return  The saved path value, or the empty string if there is no saved
 *          connection.
 */
String WifiAccessPoint::getConnectionPath() const
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