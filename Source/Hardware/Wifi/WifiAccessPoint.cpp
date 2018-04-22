#include "nm-utils.h"
#include "JuceHeader.h"
#include "Utils.h"
#include "GLibSignalHandler.h"
#include "WifiAccessPoint.h"
    
/**
 * Create an access point object using LibNM access point data and optional
 * saved connection data.
 */
WifiAccessPoint::WifiAccessPoint
(NMAccessPoint* accessPoint, SavedConnection savedConnection) : 
nmAccessPoint(accessPoint),
savedConnection(savedConnection),
networkConnection(savedConnection.getNMConnection())
{
    if(nmAccessPoint == nullptr)
    {
        return;
    }
    registerSignalHandlers();
    GLibSignalHandler glibHandler;
    glibHandler.gLibCall([this]()
    {
        const ScopedWriteLock initLock(networkUpdateLock);
        const GByteArray* ssidBytes = getSSIDBytes();
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
        bssid = nm_access_point_get_bssid(nmAccessPoint);
        apMode = nm_access_point_get_mode(nmAccessPoint);
        apFlags = nm_access_point_get_flags(nmAccessPoint);
        wpaFlags = nm_access_point_get_wpa_flags(nmAccessPoint);
        rsnFlags = nm_access_point_get_rsn_flags(nmAccessPoint);
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

        signalStrength = nm_access_point_get_strength(nmAccessPoint);
        frequency = nm_access_point_get_frequency(nmAccessPoint);
        maxBitrate = nm_access_point_get_max_bitrate(nmAccessPoint);
    });
}

    
/**
 *
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

/**
 * Unregisters the signal handler, if one exists
 */
WifiAccessPoint::~WifiAccessPoint()
{
    const ScopedWriteLock destroyLock(networkUpdateLock);
    if(updateSignalId > 0)
    {
        g_signal_handler_disconnect(nmAccessPoint,updateSignalId);
    }
    if(nmAccessPoint != nullptr)
    {
        g_object_weak_unref(G_OBJECT(nmAccessPoint),
                (GWeakNotify) apDestroyedCallback,
                this);
    }
    if(networkConnection != nullptr)
    {
        g_object_weak_unref(G_OBJECT(networkConnection),
                (GWeakNotify) apDestroyedCallback,
                this);
    }
}

/**
 * Gets the access point's service set identifier, its primary name.
 */
const String& WifiAccessPoint::getSSID() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return ssid;
}

/**
 * Gets the MAC address of the wifi access point.
 */
const String& WifiAccessPoint::getBSSID() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return bssid;
}

/**
 * Gets the signal strength of the wifi access point.
 */
int WifiAccessPoint::getSignalStrength() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return signalStrength;
}

/**
 * Gets the wifi access point frequency in (TODO: what format? MHz? 
 * documentation is unclear, do some tests and figure it out.)
 */
unsigned long WifiAccessPoint::getFrequency() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return frequency;
}

/**
 * Gets the access point's maximum data transfer bit rate.
 */
unsigned long WifiAccessPoint::getMaxBitrate() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return maxBitrate;
}
 
/**
 * Checks if this access point requires a security key.
 */
bool WifiAccessPoint::getRequiresAuth() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return security != none;
}

/**
 * Checks if this access point corresponds to a saved network connection.
 */
bool WifiAccessPoint::hasSavedConnection() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return savedConnection.isValid();
}

/**
 * Checks if there is a psk for this connection saved by the network
 * manager.
 */
bool WifiAccessPoint::hasSavedPsk() const
{
    bool pskFound = false;
    if(hasSavedConnection() && getRequiresAuth())
    {
        const ScopedReadLock readLock(networkUpdateLock);
        GLibSignalHandler glibHandler;
        glibHandler.gLibCall([this, &pskFound]()
        {
            NMSettingWirelessSecurity * securitySettings =
                    nm_connection_get_setting_wireless_security(
                    networkConnection);
            if(securitySettings != nullptr)
            {
                const char* key = nullptr;
                if(security == securedWEP)
                {
                    guint32 keyIndex = 
                            nm_setting_wireless_security_get_wep_tx_keyidx(
                            securitySettings);
                    key = nm_setting_wireless_security_get_wep_key(
                            securitySettings, keyIndex);
                }
                else
                {
                    key = nm_setting_wireless_security_get_psk
                            (securitySettings);
                }
                pskFound = (key != nullptr);            
            }
        });
    }
    return pskFound;
}


/**
 * Returns a string identifying this object for debug purposes.
 */
const String& WifiAccessPoint::toString() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return ssid;
}

        
/**
 * Checks if this access point is compatible with a given connection.
 * 
 * @return  true iff the connection is compatible, and the WifiAccessPoint
 *          doesn't have a different non-null saved connection.  
 */
bool WifiAccessPoint::isConnectionCompatible(NMConnection* connection) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    if(nmAccessPoint == nullptr)
    {
        return false;
    }
    if(networkConnection == connection)
    {
        return true;
    }
    if((networkConnection != nullptr && connection != nullptr)
        && networkConnection != connection)
    {
        return false;
    }
    bool isValid = false;
    GLibSignalHandler glibHandler;
    glibHandler.gLibCall([this, &isValid, connection]()
    {
        GError* error = nullptr;
        if(nm_connection_verify(connection, &error))
        {
            isValid = nm_access_point_connection_valid(nmAccessPoint, 
                    connection);
        }
        else
        {
            DBG("WifiAccessPoint::isConnectionCompatible: invalid connection!");
            if(error != nullptr)
            {
                DBG("Error: " << String(error->message));
                g_error_free(error);
            }
                   
        }
    });
    return isValid;
}
    
/**
 * Checks if this access point and another both share a network
 */
bool WifiAccessPoint::sharesConnectionWith(WifiAccessPoint::Ptr otherAP) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    if(this == otherAP)
    {
        return true;
    }
    NMConnection* otherConnection = otherAP->getNMConnection();
    if(networkConnection == nullptr || otherConnection == nullptr)
    {
        return hash == otherAP->hash;
    }
    return isConnectionCompatible(otherConnection);
}


/**
 * @return the access point device mode 
 */
NM80211Mode WifiAccessPoint::getMode() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return apMode;
}

/**
 * @return basic Wifi security flags 
 */
NM80211ApFlags WifiAccessPoint::getFlags() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return apFlags;
}

/**
 * @return all WPA security flags. 
 */
NM80211ApSecurityFlags WifiAccessPoint::getWPAFlags() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return wpaFlags;
}

/**
 * @return all RSN security flags 
 */
NM80211ApSecurityFlags WifiAccessPoint::getRSNFlags() const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return rsnFlags;
}

    
/**
 * Returns true iff this WifiAccessPoint has rhs as its nmAccessPoint.
 */
bool WifiAccessPoint::operator==(NMAccessPoint* rhs) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return nmAccessPoint == rhs;
}

/**
 * Returns true iff this WifiAccessPoint does not have rhs as its 
 * nmAccessPoint.
 */
bool WifiAccessPoint::operator!=(NMAccessPoint* rhs) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    return nmAccessPoint != rhs;
}


/**
 * Returns true iff this access point and rhs share a nmAccessPoint.
 */
bool WifiAccessPoint::operator==(const WifiAccessPoint& rhs) const
{
    const ScopedReadLock readLock(networkUpdateLock);
    const ScopedReadLock rhsLock(rhs.networkUpdateLock);
    if(nmAccessPoint == nullptr && rhs.nmAccessPoint == nullptr)
    {
        return hash == rhs.hash;
    }
    return nmAccessPoint == rhs.nmAccessPoint;
}

/**
 * Returns true iff this access point and rhs do not share a nmAccessPoint.
 */
bool WifiAccessPoint::operator!=(const WifiAccessPoint& rhs) const
{
    return !(*this == rhs);
}

/**
 * @return this access point's LibNM access point object 
 */
NMAccessPoint* WifiAccessPoint::getNMAccessPoint()
{
    return nmAccessPoint;
}

/**
 * Gets this access point's connection object.  If it has no connection
 * object, a new one will be created and returned.
 * 
 * @return this access point's connection object.
 */
NMConnection* WifiAccessPoint::getNMConnection()
{
    if(!hasSavedConnection() && *this != nullptr)
    {
        const ScopedWriteLock updateLock(networkUpdateLock);
        networkConnection = nm_connection_new();
        NMSettingWireless* wifiSettings
                = (NMSettingWireless*) nm_setting_wireless_new();
        g_object_set(wifiSettings,
                NM_SETTING_WIRELESS_SSID,
                nm_access_point_get_ssid(nmAccessPoint),
                NM_SETTING_WIRELESS_HIDDEN,
                false,
                nullptr);
        nm_connection_add_setting(networkConnection, NM_SETTING(wifiSettings));
        registerSignalHandlers();
    }
    const ScopedReadLock readLock(networkUpdateLock);
    return networkConnection;
}

    
/**
 * Assigns a new connection to this access point. This will fail if a valid
 * connection is already assigned, or if the new connection is not valid
 * for this access point.
 */
bool WifiAccessPoint::setNMConnection(NMConnection* newConnection)
{
    const ScopedWriteLock updateLock(networkUpdateLock);
    if(!hasSavedConnection() && isConnectionCompatible(newConnection))
    {
        if(networkConnection != nullptr)
        {
            g_object_weak_unref(G_OBJECT(networkConnection),
                (GWeakNotify) apDestroyedCallback,
                this);
        }
        networkConnection = newConnection;
        savedConnection = SavedConnection
                (nm_connection_get_path(networkConnection));
        registerSignalHandlers();
        return true;
    }
    return false;
}
 
/**
 * Saves a new security key for this access point's connection.
 * 
 * @param psk  The new security key.  This key will only be saved if it is
 *             valid for the access point's security type.
 */
void WifiAccessPoint::setPsk(String psk)
{
    if(*this == nullptr)
    {
        DBG("WifiAccessPoint::" << __func__ << ": NMAccessPoint is null!");
        return;
    }
    else if(psk.isEmpty())
    {
        DBG("WifiAccessPoint::" << __func__ << ": psk is empty!");
        return;
    }
    else if(!getRequiresAuth())
    {
        DBG("WifiAccessPoint::" << __func__ << ": AP is unsecured!");
        return;
    }
    if(!hasSavedConnection())
    {
        getNMConnection();
    }
    
    GLibSignalHandler glibHandler;
    glibHandler.gLibCall([this, &psk]()
    {
        const ScopedWriteLock updateLock(networkUpdateLock);
        NMSettingWirelessSecurity* securitySettings
                    = (NMSettingWirelessSecurity*)
                    nm_setting_wireless_security_new();
        nm_connection_add_setting(networkConnection,
                    NM_SETTING(securitySettings));
        GVariantDict* settingsDict = nullptr;
        if(hasSavedConnection() && psk.isNotEmpty())
        {
            settingsDict = g_variant_dict_new(nullptr);
        }
        if(security == securedWEP)
        {
            DBG("WifiAccessPoint::" << __func__
                    << ": access point has WEP security");
            /* WEP */
            nm_setting_wireless_security_set_wep_key
                    (securitySettings, 0, psk.toRawUTF8());
            if(settingsDict != nullptr)
            {
                g_variant_dict_insert_value(settingsDict,
                        NM_SETTING_WIRELESS_SECURITY_WEP_KEY0,
                        g_variant_new_variant(
                        g_variant_new_string(psk.toRawUTF8())));
            }
            //valid key format: length 10 or length 26
            const char* keyType = nullptr;
            if (psk.length() == 10 || psk.length() == 26)
            {
                keyType = (const char*) NM_WEP_KEY_TYPE_KEY;
            }
            //valid passphrase format: length 5 or length 14
            else if (psk.length() == 5 || psk.length() == 13)
            {
                keyType = (const char*) NM_WEP_KEY_TYPE_PASSPHRASE;
            }
            else
            {
                DBG("WifiAccessPoint::" << __func__
                        << ": User input invalid WEP Key type, "
                        << "psk.length() = " << psk.length()
                        << ", not in [5,10,13,26]");
            }
            if(keyType != nullptr)
            {
                g_object_set(G_OBJECT(securitySettings),
                        NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE,
                        keyType, nullptr);
                if(settingsDict != nullptr)
                {
                    g_variant_dict_insert_value(settingsDict,
                            NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE,
                            g_variant_new_variant(
                            g_variant_new_string(keyType)));
                }
            }
        }
        else
        {
            DBG("LibNMHandler::" << __func__
                    << ": access point has WPA security");
            g_object_set(G_OBJECT(securitySettings),
                    NM_SETTING_WIRELESS_SECURITY_PSK,
                    psk.toRawUTF8(), nullptr);
            if(settingsDict != nullptr)
            {
                g_variant_dict_insert_value(settingsDict,
                        NM_SETTING_WIRELESS_SECURITY_PSK,
                        g_variant_new_variant(
                        g_variant_new_string(psk.toRawUTF8())));
            }
        }
        if(settingsDict != nullptr)
        {
            savedConnection.updateWifiSecurity
                    (g_variant_dict_end(settingsDict));
            settingsDict = nullptr;
        }
    });
    
}
 
/**
 * Removes the security key saved for this access point.  This should only 
 * be done when the saved key turns out to be invalid. 
 */
void WifiAccessPoint::removePsk()
{
    if(hasSavedConnection())
    {
        GLibSignalHandler glibHandler;
        glibHandler.gLibCall([this]()
        {
            const ScopedWriteLock updateLock(networkUpdateLock);
            nm_connection_remove_setting(networkConnection,
                        NM_TYPE_SETTING_WIRELESS_SECURITY);
            savedConnection.removeSecurityKey();

        });
    }
}

/**
 * Gets an SSID byte array from a saved connection or access point.  If
 * possible, the value from the saved connection will be used first.
 */
const GByteArray* WifiAccessPoint::getSSIDBytes()
{
    const GByteArray* ssidBytes = nullptr;
    GLibSignalHandler glibHandler;
    glibHandler.gLibCall([this, &ssidBytes]()
    {
        if (networkConnection != nullptr)
        {
            NMSettingWireless* wifiSettings =
                    nm_connection_get_setting_wireless(networkConnection);
            if (wifiSettings != nullptr)
            {
                ssidBytes = nm_setting_wireless_get_ssid(wifiSettings);
            }
        }
        if(ssidBytes == nullptr && nmAccessPoint != nullptr)
        {
            ssidBytes = nm_access_point_get_ssid(nmAccessPoint);
        }
    });
    return ssidBytes;
}

/**
 * If the NMAccessPoint is non-null and signal handlers aren't already
 * registered, this will register this object to receive updates when
 * access point signal strength changes or the NMAccessPoint is destroyed.
 */
void WifiAccessPoint::registerSignalHandlers()
{
    if(nmAccessPoint != nullptr && updateSignalId == 0)
    {
        updateSignalId = g_signal_connect_swapped(
                nmAccessPoint,
                "notify::" NM_ACCESS_POINT_STRENGTH,
                G_CALLBACK(strengthUpdateCallback),
                this);
        g_object_weak_ref(G_OBJECT(nmAccessPoint),
                (GWeakNotify) apDestroyedCallback, this);  
        //DBG("AP=" << String::toHexString((unsigned long) nmAP)
        //        << " signal handler=" << String(updateSignalId));
    }
    if(networkConnection != nullptr)
    {
        g_object_weak_ref(G_OBJECT(networkConnection),
                (GWeakNotify) apDestroyedCallback, this);  
    }
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
 * If this object's NMAccessPoint or NMConnection is deleted, this will 
 * remove the pointer to the deleted object and set it to null.  This should
 * only be called on the GLib event loop by GLib library code.
 */
void WifiAccessPoint::apDestroyedCallback(WifiAccessPoint* toUpdate,
        GObject* removed)
{
    if((NMAccessPoint*) removed == toUpdate->nmAccessPoint)
    {
        const ScopedWriteLock destroyLock(toUpdate->networkUpdateLock);
        toUpdate->nmAccessPoint = nullptr;
        toUpdate->signalStrength = 0;
        toUpdate->updateSignalId = 0;
    }
    else if((NMConnection*) removed == toUpdate->networkConnection)
    {
        toUpdate->networkConnection = nullptr;
    }
    else
    {
        DBG("WifiAccessPoint::"<<__func__<<": destroyed object=0x"
                << String::toHexString((unsigned long) removed)
                << " but saved AP=0x"
                << String::toHexString((unsigned long) toUpdate->nmAccessPoint)         
                << " and saved connection=0x"
                << String::toHexString((unsigned long) toUpdate->networkConnection));
    }
    //TODO: implement listeners, notify of AP loss
}


/**
 * A callback to update signal strength whenever NMAccessPoint strength
 * changes.  This should only be called on the GLib event loop by GLib 
 * library code.
 */
void WifiAccessPoint::strengthUpdateCallback(WifiAccessPoint* toUpdate)
{
    const ScopedWriteLock updateLock(toUpdate->networkUpdateLock);
    g_assert(g_main_context_is_owner(g_main_context_default()));
    int strength = nm_access_point_get_strength(toUpdate->nmAccessPoint);
    if(strength != toUpdate->signalStrength)
    {
        toUpdate->signalStrength = strength;
        //TODO: implement listeners, notify of strength update
    }
}
