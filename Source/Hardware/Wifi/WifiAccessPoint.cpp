#include "JuceHeader.h"
#include "Utils.h"
#include "WifiAccessPoint.h"


#include "GLibSignalHandler.h"
#include "nm-utils.h"

WifiAccessPoint::WifiAccessPoint
(String ssid, int signalStrength, bool requiresAuth, String hash) :
ssid(ssid),
signalStrength(median<int>(0, signalStrength, 100)),
hash(hash)
{
    security = requiresAuth ? securedWPA : none;
}

WifiAccessPoint::WifiAccessPoint
(NMAccessPoint* accessPoint, NMConnection* savedConnection)
{
    if(accessPoint == nullptr)
    {
        return;
    }
    updateSignalId = g_signal_connect_swapped(
            accessPoint,
            "notify::" NM_ACCESS_POINT_STRENGTH,
            G_CALLBACK(strengthUpdateCallback),
            this);
    g_object_weak_ref(G_OBJECT(accessPoint),
            (GWeakNotify) apDestroyedCallback, this);
    nmAP = accessPoint;
    DBG("AP=" << String::toHexString((unsigned long) accessPoint)
            << " signal handler=" << String(updateSignalId));
    
    GLibSignalHandler glibHandler;
    glibHandler.gLibCall([this, accessPoint, savedConnection]()
    {;
        const GByteArray* ssidBytes = getSSIDBytes(accessPoint, savedConnection);
        if(ssidBytes != nullptr)
        {
            char* utfSSID = nm_utils_ssid_to_utf8(ssidBytes);
            if (utfSSID != nullptr)
            {
                ssid = utfSSID;
                g_free(utfSSID);
                utfSSID = nullptr;
            }
        }
        
        if(savedConnection != nullptr)
        {
            connectionSaved = true;
            NMSettingWirelessSecurity* securitySettings =
                    nm_connection_get_setting_wireless_security
                    (savedConnection);
            if(securitySettings != nullptr 
               && nm_setting_wireless_security_get_psk
               (securitySettings) != nullptr)
            {
                pskSaved = true;
            }
        }
        
        bssid = nm_access_point_get_bssid(accessPoint);
        apMode = nm_access_point_get_mode(accessPoint);
        apFlags = nm_access_point_get_flags(accessPoint);
        wpaFlags = nm_access_point_get_wpa_flags(accessPoint);
        rsnFlags = nm_access_point_get_rsn_flags(accessPoint);
        hash = generateHash(ssidBytes,apMode,apFlags,wpaFlags,rsnFlags);

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

        signalStrength = nm_access_point_get_strength(accessPoint);
        frequency = nm_access_point_get_frequency(accessPoint);
        maxBitrate = nm_access_point_get_max_bitrate(accessPoint);
    });
}

    
/**
 * Copies an existing access point object, registering a new signal 
 * handler for the new access point instance.
 */
WifiAccessPoint::WifiAccessPoint(const WifiAccessPoint& toCopy):
ssid(toCopy.ssid),
bssid(toCopy.bssid),
security(toCopy.security),
hash(toCopy.hash),
signalStrength(toCopy.signalStrength),
frequency(toCopy.frequency),
maxBitrate(toCopy.maxBitrate),
connectionSaved(toCopy.connectionSaved),
pskSaved(toCopy.pskSaved),
apMode(toCopy.apMode),
apFlags(toCopy.apFlags),
rsnFlags(toCopy.rsnFlags),
nmAP(toCopy.nmAP)
{
    if(nmAP != nullptr)
    {
        updateSignalId = g_signal_connect_swapped(
                nmAP,
                "notify::" NM_ACCESS_POINT_STRENGTH,
                G_CALLBACK(strengthUpdateCallback),
                this);
        g_object_weak_ref(G_OBJECT(nmAP),
                (GWeakNotify) apDestroyedCallback, this);
        
        DBG("AP=" << String::toHexString((unsigned long) nmAP)
                << " signal handler=" << String(updateSignalId));
    }
}

    
/**
 * Unregisters the signal handler, if one exists
 */
WifiAccessPoint::~WifiAccessPoint()
{
    if(updateSignalId > 0)
    {
        g_signal_handler_disconnect(nmAP,updateSignalId);
    }
    if(nmAP != nullptr)
    {
        g_object_weak_unref(G_OBJECT(nmAP), (GWeakNotify) apDestroyedCallback,
                this);
    }
}
/**
 * Creates a void WifiAccessPoint, representing the absence of a wifi access 
 * point.
 */
WifiAccessPoint::WifiAccessPoint() :
signalStrength(-1),
security(none) 
{ }

/**
 * @return true iff this WifiAccessPoint is void.
 */
bool WifiAccessPoint::isVoid() const
{
    return ssid.isEmpty()
            && signalStrength == -1
            && hash.isEmpty();
}

/**
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


/**
 * Gets an SSID byte array from a saved connection or access point.  If
 * possible, the value from the saved connection will be used first.
 */
const GByteArray* WifiAccessPoint::getSSIDBytes(NMAccessPoint* ap, NMConnection* conn)
{
    const GByteArray* ssidBytes = nullptr;
    GLibSignalHandler glibHandler;
    glibHandler.gLibCall([conn, ap, &ssidBytes]()
    {
        if (conn != nullptr)
        {
            NMSettingWireless* wifiSettings =
                    nm_connection_get_setting_wireless(conn);
            if (wifiSettings != nullptr)
            {
                ssidBytes = nm_setting_wireless_get_ssid(wifiSettings);
            }
        }
        if(ssidBytes == nullptr && ap != nullptr)
        {
            ssidBytes = nm_access_point_get_ssid(ap);
        }
    });
    return ssidBytes;
}


/**
 * Generates a hash value for a NMAccessPoint that will be unique to that
 * access point's connection.
 */
String WifiAccessPoint::generateHash(NMAccessPoint* ap, NMConnection* conn)
{
    GLibSignalHandler glibHandler;
    String hash;
    glibHandler.gLibCall([ap, conn, &hash]()
    {
        const GByteArray* rawSSID = getSSIDBytes(ap, conn);
        if(rawSSID != nullptr)
        {
            NM80211Mode mode = nm_access_point_get_mode(ap);
            guint32 flags = nm_access_point_get_flags(ap);
            guint32 wpa_flags = nm_access_point_get_wpa_flags(ap);
            guint32 rsn_flags = nm_access_point_get_rsn_flags(ap);
            hash = generateHash(rawSSID, mode, flags, wpa_flags, rsn_flags);
        }
    });
    return hash;
}

void WifiAccessPoint::apDestroyedCallback(WifiAccessPoint* toUpdate,
        GObject* removed)
{
    jassert((NMAccessPoint*) removed == toUpdate->nmAP);
    DBG("AP " << toUpdate->ssid << " destroyed");
    toUpdate->nmAP = nullptr;
    toUpdate->signalStrength = 0;
    toUpdate->updateSignalId = 0;
}

void WifiAccessPoint::strengthUpdateCallback(WifiAccessPoint* toUpdate)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    DBG("Wifi AP strength update: for " << toUpdate->ssid);
}
