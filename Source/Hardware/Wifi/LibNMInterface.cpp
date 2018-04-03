#include "LibNMInterface.h"
#include <map>
#include <nm-utils.h>
#include <nm-remote-connection.h>
#include "JuceHeader.h"

#if JUCE_DEBUG

/**
 * Converts a NMDeviceState to a String for debug output.
 */
String deviceStateString(NMDeviceState state)
{
    switch (state)
    {
        case NM_DEVICE_STATE_ACTIVATED:
            return "NM_DEVICE_STATE_ACTIVATED";
        case NM_DEVICE_STATE_PREPARE:
            return "NM_DEVICE_STATE_PREPARE";
        case NM_DEVICE_STATE_CONFIG:
            return "NM_DEVICE_STATE_CONFIG";
        case NM_DEVICE_STATE_IP_CONFIG:
            return "NM_DEVICE_STATE_IP_CONFIG";
        case NM_DEVICE_STATE_IP_CHECK:
            return "NM_DEVICE_STATE_IP_CHECK";
        case NM_DEVICE_STATE_SECONDARIES:
            return "NM_DEVICE_STATE_SECONDARIES";
        case NM_DEVICE_STATE_NEED_AUTH:
            return "NM_DEVICE_STATE_NEED_AUTH";
        case NM_DEVICE_STATE_DISCONNECTED:
            return "NM_DEVICE_STATE_DISCONNECTED";
        case NM_DEVICE_STATE_DEACTIVATING:
            return "NM_DEVICE_STATE_DEACTIVATING";
        case NM_DEVICE_STATE_FAILED:
            return "NM_DEVICE_STATE_FAILED";
        case NM_DEVICE_STATE_UNKNOWN:
            return "NM_DEVICE_STATE_UNKNOWN";
        case NM_DEVICE_STATE_UNMANAGED:
            return "NM_DEVICE_STATE_UNMANAGED";
        case NM_DEVICE_STATE_UNAVAILABLE:
            return "NM_DEVICE_STATE_UNAVAILABLE";
        default:
            return "Invalid state!";
    }
}
#endif

LibNMInterface::LibNMInterface()
{
    nmSignalSubscribe();
    updateAllWifiData();
}

LibNMInterface::~LibNMInterface()
{
    nmSignalUnsubscribe();
}

/**
 * Check the NMDevice state to see if wifi is enabled.
 */
bool LibNMInterface::isWifiEnabled()
{
    const ScopedLock lock(wifiLock);
    switch (lastNMState)
    {
        case NM_DEVICE_STATE_DISCONNECTED:
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_NEED_AUTH:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
        case NM_DEVICE_STATE_ACTIVATED:
        case NM_DEVICE_STATE_DEACTIVATING:
        case NM_DEVICE_STATE_FAILED:
            return true;
    }
    return false;
}

/**
 * @return true iff the wifi device is currently connecting to an access point.
 */
bool LibNMInterface::isWifiConnecting()
{
    const ScopedLock lock(wifiLock);
    switch (lastNMState)
    {
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_NEED_AUTH:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
            return true;
    }
    return false;
}

/**
 * Check the NMDevice state to see if wifi is connected.
 */
bool LibNMInterface::isWifiConnected()
{
    const ScopedLock lock(wifiLock);
    DBG("LibNMInterface::" << __func__ << ": state is "
            << deviceStateString(lastNMState));
    if (lastNMState == NM_DEVICE_STATE_ACTIVATED && !connectedAP.isNull())
    {
        return true;
    }
    return false;
}

/**
 * Request information on the connected access point from the NMDevice.
 */
WifiAccessPoint LibNMInterface::getConnectedAP()
{
    const ScopedLock lock(wifiLock);
    return connectedAP;
}

/**
 * Request information on the connecting access point from the NMDevice.
 */
WifiAccessPoint LibNMInterface::getConnectingAP()
{
    const ScopedLock lock(wifiLock);
    return connectingAP;
}

/**
 * Request information on all wifi access points detected by the NMDevice.
 */
Array<WifiAccessPoint> LibNMInterface::getVisibleAPs()
{
    const ScopedLock lock(wifiLock);
    return visibleAPs;
}

/**
 * Asynchronously attempt to connect to a wifi access point.
 */
void LibNMInterface::connectToAccessPoint(const WifiAccessPoint& toConnect,
        String psk)
{
    const ScopedLock lock(wifiLock);
    pendingConnect = toConnect;
    pendingPsk = psk;
    DBG("LibNMInterface::" << __func__ << ": trying to connect to "
            << toConnect.getSSID());
    g_idle_add((GSourceFunc) &asyncConnect, this);
}

/**
 * Asynchronously checks the signal strength of the connected access point.
 */
void LibNMInterface::updateConnectedSignalStrength()
{
    g_idle_add((GSourceFunc) &asyncSignalCheckConnected, this);
}

/**
 * Asynchronously checks the signal strength of all visible access points.
 */
void LibNMInterface::updateVisibleSignalStrengths()
{
    g_idle_add((GSourceFunc) &asyncSignalCheckAll, this);
}

/**
 * Asynchronously checks the wifi device list, connection state, and 
 * active and pending connections.
 */
void LibNMInterface::updateAllWifiData()
{
    g_idle_add((GSourceFunc) &asyncUpdateAll, this);
}

/**
 * Asynchronously close the currently active connection, if one exists.
 */
void LibNMInterface::disconnect()
{
    g_idle_add((GSourceFunc) &asyncDisconnect, this);
}

/**
 * Asynchronously turns on the wifi radio.
 */
void LibNMInterface::enableWifi()
{
    g_idle_add((GSourceFunc) &asyncEnable, this);
}

/**
 * Asynchronously turns off the wifi radio.
 */
void LibNMInterface::disableWifi()
{
    g_idle_add((GSourceFunc) &asyncDisable, this);
}

/**
 * Attaches LibNMInterface callback functions to the network manager client
 * and wifi device in order to receive updates when wifi actions occur.
 */
void LibNMInterface::nmSignalSubscribe()
{
    ScopedLock initLock(wifiLock);
    nmClient = nm_client_new();
    if (nmClient == nullptr || !NM_IS_CLIENT(nmClient))
    {
        DBG("LibNMInterface::" << __func__
                << ": failed to connect to nmclient over dbus");
        nmClient = nullptr;
        return;
    }
    nmDevice = nm_client_get_device_by_iface(nmClient, "wlan0");
    if (nmDevice == nullptr || !NM_IS_DEVICE(nmDevice))
    {
        DBG("LibNMInterface::" << __func__ <<
                ":  failed to connect to nmDevice wlan0 over DBus");
        nmClient = nullptr;
        nmDevice = nullptr;
        return;
    }
    nmWifiDevice = NM_DEVICE_WIFI(nmDevice);
    if (nmWifiDevice == nullptr || !NM_IS_DEVICE_WIFI(nmWifiDevice))
    {
        DBG("LibNMInterface::" << __func__ <<
                ": network manager cannot access wlan0 as a wifi device!");
        nmClient = nullptr;
        nmDevice = nullptr;
        nmWifiDevice = nullptr;
        return;
    }
    //Signal: notifies that wifi has turned on or off
    clientSignalHandlers.add(g_signal_connect(
            nmClient,
            "notify::" NM_CLIENT_WIRELESS_ENABLED,
            G_CALLBACK(handleWifiEnabledChange), this));

    //Signal: notifies that wifi state has changed
    deviceSignalHandlers.add(g_signal_connect(
            nmDevice,
            "notify::" NM_DEVICE_STATE,
            G_CALLBACK(handleStateChange), this));

    //Signal: notifies that the active access point has changed
    wifiSignalHandlers.add(g_signal_connect(
            nmWifiDevice,
            "notify::" NM_DEVICE_WIFI_ACTIVE_ACCESS_POINT,
            G_CALLBACK(handleConnectionChange), this));

    //Signal: notifies that a new wifi access point is visible.
    wifiSignalHandlers.add(g_signal_connect(
            nmWifiDevice,
            "access-point-added",
            G_CALLBACK(handleApChange), this));

    //Signal: notifies that a wifi access point is no longer visible.
    wifiSignalHandlers.add(g_signal_connect(
            nmWifiDevice,
            "access-point-removed",
            G_CALLBACK(handleApChange), this));
}

/**
 * Remove all subscribed LibNMInterface callback functions from the
 * network manager client and the wifi device.
 */
void LibNMInterface::nmSignalUnsubscribe()
{
    ScopedLock cleanupLock(wifiLock);
    int gIdleCount = 0;
    while (g_idle_remove_by_data(this))
    {
        gIdleCount++;
    }
    DBG("LibNMInterface::" << __func__ << ": removed " << gIdleCount
            << " async glib functions.");

    if (nmWifiDevice != nullptr && NM_IS_DEVICE_WIFI(nmWifiDevice))
    {
        for (gulong& signalHandlerId : wifiSignalHandlers)
        {
            g_signal_handler_disconnect(nmWifiDevice, signalHandlerId);
        }
    }
    wifiSignalHandlers.clear();
    nmWifiDevice = nullptr;

    if (nmDevice != nullptr && NM_IS_DEVICE(nmDevice))
    {
        for (gulong& signalHandlerId : deviceSignalHandlers)
        {
            g_signal_handler_disconnect(nmDevice, signalHandlerId);
        }
    }
    deviceSignalHandlers.clear();
    nmDevice = nullptr;

    if (nmClient != nullptr && NM_IS_CLIENT(nmClient))
    {
        for (gulong& signalHandlerId : clientSignalHandlers)
        {
            g_signal_handler_disconnect(nmClient, signalHandlerId);
        }
        clientSignalHandlers.clear();
    }
    clientSignalHandlers.clear();
    nmClient = nullptr;

}

/**
 * called whenever the application window gains focus.
 */
void LibNMInterface::windowFocusGained()
{
    nmSignalSubscribe();
}

/**
 * called whenever the application window loses focus.
 */
void LibNMInterface::windowFocusLost()
{
    nmSignalUnsubscribe();
}

/**
 * Generate a unique hash string identifying a wifi access point.
 * Adapted from network-manager-applet src/utils/utils.c 
 */
char* LibNMInterface::hashAP(NMAccessPoint* ap)
{
    const GByteArray* ssid = nm_access_point_get_ssid(ap);
    NM80211Mode mode = nm_access_point_get_mode(ap);
    guint32 flags = nm_access_point_get_flags(ap);
    guint32 wpa_flags = nm_access_point_get_wpa_flags(ap);
    guint32 rsn_flags = nm_access_point_get_rsn_flags(ap);
    unsigned char input[66];

    memset(&input[0], 0, sizeof (input));

    if (ssid)
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

    return g_compute_checksum_for_data(G_CHECKSUM_MD5, input, sizeof (input));
}

/**
 * @return true iff ap is a secure access point.
 */
bool LibNMInterface::resolveAPSecurity(NMAccessPoint *ap)
{
    //FIXME: Assumes all security types equal

    return (
            nm_access_point_get_flags(ap) == NM_802_11_AP_FLAGS_PRIVACY ||
            nm_access_point_get_wpa_flags(ap) != NM_802_11_AP_SEC_NONE ||
            nm_access_point_get_rsn_flags(ap) != NM_802_11_AP_SEC_NONE
            );
}

/**
 * @return WifiAccessPoint data read from a NMAccessPoint
 */
WifiAccessPoint LibNMInterface::createNMWifiAccessPoint(NMAccessPoint *ap)
{
    if (ap == nullptr)
    {
        return WifiAccessPoint();
    }
    const GByteArray *ssid = nm_access_point_get_ssid(ap);
    char* ssid_str = nullptr;
    char* ssid_hex_str = nullptr;
    bool security = resolveAPSecurity(ap);

    /* Convert to strings */
    if (ssid)
    {
        const guint8 *ssid_data = ssid->data;
        ssid_str = nm_utils_ssid_to_utf8(ssid);
    }

    if (ssid_str == nullptr || ssid == nullptr)
    {
        DBG("LibNMInterface::" << __func__ << ": libnm conversion of ssid to "
                << "utf8 failed, using empty string");
    }
    return WifiAccessPoint(!ssid_str ? "" : ssid_str,
            nm_access_point_get_strength(ap),
            security,
            hashAP(ap));
}

/**
 * Close a specific wifi connection on wifi device wlan0.  This should
 * only be called from the GLib main event loop.
 */
void LibNMInterface::removeNMConnection(NMActiveConnection *conn)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    if (nmDevice == nullptr)
    {
        DBG("LibNMInterface::" << __func__ << ": wlan0 is missing!");
        return;
    }
    if (conn == nullptr)
    {
        DBG("LibNMInterface::" << __func__ << ": connection is null!");
        return;
    }
    const char *ac_uuid = nm_active_connection_get_uuid(conn);
    const GPtrArray *avail_cons = nm_device_get_available_connections
            (nmDevice);
    for (int i = 0; avail_cons && (i < avail_cons->len); i++)
    {
        NMRemoteConnection* candidate = (NMRemoteConnection*)
                g_ptr_array_index(avail_cons, i);
        const char* test_uuid = nm_connection_get_uuid
                (NM_CONNECTION(candidate));

        if (g_strcmp0(ac_uuid, test_uuid) == 0)
        {
            GError *err = nullptr;
            nm_remote_connection_delete(candidate, nullptr, &err);
            if (err)
            {
                DBG("LibNMInterface::" << __func__
                        << ": failed to remove active connection!");
                DBG("LibNMInterface::" << __func__ << ": "
                        << err->message);
                g_error_free(err);
            }
            break;
        }
    }
}

/**
 * @param key
 * @return true iff key has the correct format for a WEP key.
 */
bool LibNMInterface::isValidWEPKeyFormat(String key)
{

    return (key.length() == 10) || (key.length() == 26);
}

/**
 * @param phrase
 * @return true iff phrase has the correct format for a WEP passphrase.
 */
bool LibNMInterface::isValidWEPPassphraseFormat(String phrase)
{

    return (phrase.length() == 5) || (phrase.length() == 13);
}

/**
 * Requests the current connecting access point from the network manager.
 * This should only be called from the GLib main event loop.
 */
NMAccessPoint* LibNMInterface::getConnectingNmAp()
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    NMActiveConnection* conn =
            nm_client_get_activating_connection
            (nmClient);
    if (conn != nullptr)
    {
        NMAccessPoint* connectingAP =
                nm_device_wifi_get_access_point_by_path(
                nmWifiDevice,
                nm_active_connection_get_specific_object(conn));
        return connectingAP;
    }
    return nullptr;
}

/**
 * Requests the current connected access point from the network manager.
 * This should only be called from the GLib main event loop.
 */
NMAccessPoint* LibNMInterface::getConnectedNmAp()
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    return nm_device_wifi_get_active_access_point(nmWifiDevice);
}

/**
 * Requests the current list of visible access points from the network 
 * manager.  This should only be called from the GLib main event loop.
 */
Array<NMAccessPoint*> LibNMInterface::getVisibleNmAps()
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    Array<NMAccessPoint*> accessPoints;
    const GPtrArray* apList = nm_device_wifi_get_access_points(nmWifiDevice);
    if (apList != nullptr)
    {
        StringArray hashlist;
        for (int i = 0; i < apList->len; i++)
        {
            accessPoints.add((NMAccessPoint *) g_ptr_array_index(apList, i));
        }
    }
    return accessPoints;
}

/**
 * This callback runs after libnm attempts to open a network connection. It
 * primarily serves to catch immediate connection errors.  If it encounters
 * an error, it will inform the Wifi state manager with 
 * signalConnectionFailed()
 */
void LibNMInterface::handleConnectionAttempt(
        NMClient *client,
        NMActiveConnection *active,
        const char* path,
        GError *err,
        LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    NMAccessPoint* ap =
            nm_device_wifi_get_access_point_by_path(
            nmInterface->nmWifiDevice,
            nm_active_connection_get_specific_object(active));
    if (err != nullptr || ap == nullptr)
    {
        nmInterface->connectingAP = WifiAccessPoint();
        DBG("LibNMInterface::" << __func__
                << ": failed to add/activate connection!");
        DBG("LibNMInterface::" << __func__ << ": " << err->message);
        nmInterface->signalConnectionFailed();
    }
    else
    {
        nmInterface->connectingAP = createNMWifiAccessPoint(ap);
        nmInterface->signalWifiConnecting();
    }
}

/**
 * This callback runs when the network manager client signals that wifi
 * was enabled or disabled. Depending on which event occurs, this will 
 * inform the Wifi state manager with either signalWifiEnabled() or 
 * signalWifiDisabled().
 */
void LibNMInterface::handleWifiEnabledChange(
        NMClient* client,
        LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    bool enabled = nm_client_wireless_get_enabled(client);
    DBG("LibNMInterface::" << __func__ << ":  changed to "
            << (enabled ? "enabled" : "disabled"));
    //FIXME: Force and wait for a scan after enable
    if (enabled)
    {
        nmInterface->signalWifiEnabled();
    }
    else
    {
        nmInterface->signalWifiDisabled();
    }
}

/**
 * This callback runs when device wlan0 signals that its state has changed.
 * Depending on what state it enters, the nmInterface may send an update to
 * the WifiStateManager.
 */
void LibNMInterface::handleStateChange(
        NMDevice* nmDevice,
        LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    ScopedLock updateLock(nmInterface->wifiLock);
    NMDeviceState oldState = nmInterface->lastNMState;
    nmInterface->lastNMState = nm_device_get_state(nmDevice);
    DBG("LibNMInterface::" << __func__ << ":  changed to "
            << deviceStateString(nmInterface->lastNMState));
    switch (nmInterface->lastNMState)
    {
        case NM_DEVICE_STATE_ACTIVATED:
        {
            ScopedUnlock unlockForUpdate(nmInterface->wifiLock);
            nmInterface->signalWifiConnected(nmInterface->getConnectedAP());
            break;
        }
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
            /* No state change for now, wait for connection to complete/fail */
            //ensure the pending connection is registered
            if (nmInterface->connectingAP.isNull())
            {
                NMAccessPoint* connecting = nmInterface->getConnectingNmAp();
                nmInterface->connectingAP
                        = createNMWifiAccessPoint(connecting);
                ScopedUnlock unlockForUpdate(nmInterface->wifiLock);
                nmInterface->signalWifiConnecting();
            }
            break;
        case NM_DEVICE_STATE_NEED_AUTH:
        {
            /* FIXME: let this drop into the general failed case for now
             *        eventually this should prompt the user
             */
            DBG("LibNMInterface::" << __func__
                    << " missing auth, canceling connection.");
            NMActiveConnection *conn =
                    nm_client_get_activating_connection
                    (nmInterface->nmClient);
            ScopedUnlock unlockForUpdate(nmInterface->wifiLock);
            nmInterface->removeNMConnection(conn);
        }
        case NM_DEVICE_STATE_DISCONNECTED:
        {
            ScopedUnlock unlockForUpdate(nmInterface->wifiLock);
            nmInterface->signalConnectionFailed();
            break;
        }
        case NM_DEVICE_STATE_DEACTIVATING:
        case NM_DEVICE_STATE_FAILED:
        {
            ScopedUnlock unlockForUpdate(nmInterface->wifiLock);
            nmInterface->signalWifiDisconnected();
            break;
        }
        case NM_DEVICE_STATE_UNKNOWN:
        case NM_DEVICE_STATE_UNMANAGED:
        case NM_DEVICE_STATE_UNAVAILABLE:
        default:
        {
            DBG("LibNMInterface::" << __func__
                    << ": wlan0 device entered unmanaged state: "
                    << deviceStateString(nmInterface->lastNMState));
            ScopedUnlock unlockForUpdate(nmInterface->wifiLock);
            nmInterface->signalWifiDisconnected();
        }
    }
}

/**
 * This callback runs when device wlan0 signals that its active connection 
 * has changed. This will check the status of the connection, and call 
 * signalWifiConnected() or signalWifiDisconnected()
 * 
 * @param wifiDevice    The wifi device wlan0.
 * 
 * @param nmInterface   A pointer to the LibNMInterface object.
 */
void LibNMInterface::handleConnectionChange(
        NMDeviceWifi* wifiDevice,
        LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    ScopedLock updateLock(nmInterface->wifiLock);
    NMAccessPoint* nmConnected = nmInterface->getConnectedNmAp();
    if (nmConnected == nullptr)
    {
        nmInterface->connectedAP = WifiAccessPoint();
        ScopedUnlock notifyUnlock(nmInterface->wifiLock);
        nmInterface->signalWifiDisconnected();
    }
    else
    {
        nmInterface->connectedAP
                = createNMWifiAccessPoint(nmConnected);
        ScopedUnlock notifyUnlock(nmInterface->wifiLock);
        nmInterface->signalWifiConnected(nmInterface->connectedAP);
    }
}

/**
 * This callback runs when device wlan0 signals that a new wifi access point
 * is visible or an old one has been lost.  This will update the nmInterface's 
 * record of visible access points.
 */
void LibNMInterface::handleApChange(
        NMDeviceWifi* wifiDevice,
        LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    ScopedLock updateLock(nmInterface->wifiLock);
    nmInterface->visibleAPs.clearQuick();
    StringArray processedAPHashes;

    Array<NMAccessPoint*> rawAPList = nmInterface->getVisibleNmAps();
    for (NMAccessPoint* nmAP : rawAPList)
    {
        WifiAccessPoint wifiAP = createNMWifiAccessPoint(nmAP);
        if (wifiAP.getSSID().isNotEmpty()
            && !processedAPHashes.contains(wifiAP.getHash()))
        {
            processedAPHashes.add(wifiAP.getHash());
            nmInterface->visibleAPs.add(wifiAP);
        }
    }
    DBG("LibNMInterface::" << __func__ << ": found " 
            << nmInterface->visibleAPs.size() << " AccessPoints");
}

/**
 * Runs on the GLib main event loop to turn on wifi.
 */
bool LibNMInterface::asyncEnable(LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    nm_client_wireless_set_enabled(nmInterface->nmClient, true);
    return true;
}

/**
 * Runs on the GLib main event loop to turn off wifi.
 */
bool LibNMInterface::asyncDisable(LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    nm_client_wireless_set_enabled(nmInterface->nmClient, false);
    return true;
}

/**
 * Runs on the GLib main event loop to initiate a wifi connection.
 */
bool LibNMInterface::asyncConnect(LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    const ScopedLock lock(nmInterface->wifiLock);
    WifiAccessPoint pendingAP = nmInterface->pendingConnect;
    String psk = nmInterface->pendingPsk;
    nmInterface->pendingConnect = WifiAccessPoint();
    nmInterface->pendingPsk = String();

    const char* apPath = nullptr;
    NMAccessPoint* matchingAP = nullptr;
    Array<NMAccessPoint*> visibleAPs = nmInterface->getVisibleNmAps();
    for (NMAccessPoint* candidateAP : visibleAPs)
    {
        const char* candidateHash = hashAP(candidateAP);
        if (pendingAP.getHash() == candidateHash)
        {
            apPath = nm_object_get_path(NM_OBJECT(candidateAP));
            matchingAP = candidateAP;
            break;
        }
    }
    if (apPath == nullptr)
    {
        DBG("LibNMInterface::" << __func__
                << ": no matching access point in list of length "
                << String(visibleAPs.size()));
        return true;
    }
    DBG("LibNMInterface::" << __func__ << ": matching NMAccessPoint found");
    NMConnection* connection = nm_connection_new();
    NMSettingWireless* settingWifi = (NMSettingWireless*)
            nm_setting_wireless_new();
    nm_connection_add_setting(connection, NM_SETTING(settingWifi));
    g_object_set(settingWifi,
            NM_SETTING_WIRELESS_SSID,
            nm_access_point_get_ssid(matchingAP),
            NM_SETTING_WIRELESS_HIDDEN,
            false,
            nullptr);

    if (!psk.isEmpty())
    {
        NMSettingWirelessSecurity* settingWifiSecurity =
                (NMSettingWirelessSecurity*)
                nm_setting_wireless_security_new();
        nm_connection_add_setting(connection, NM_SETTING(settingWifiSecurity));

        if (nm_access_point_get_wpa_flags(matchingAP)
            == NM_802_11_AP_SEC_NONE
            && nm_access_point_get_rsn_flags(matchingAP)
            == NM_802_11_AP_SEC_NONE)
        {
            DBG("LibNMInterface::" << __func__
                    << ": access point has WEP security");
            /* WEP */
            nm_setting_wireless_security_set_wep_key(settingWifiSecurity,
                    0, psk.toRawUTF8());
            if (isValidWEPKeyFormat(psk))
            {
                g_object_set(G_OBJECT(settingWifiSecurity),
                        NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE,
                        NM_WEP_KEY_TYPE_KEY, nullptr);
            }
            else if (isValidWEPPassphraseFormat(psk))
            {
                g_object_set(G_OBJECT(settingWifiSecurity),
                        NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE,
                        NM_WEP_KEY_TYPE_PASSPHRASE, nullptr);
            }
            else
            {
                DBG("LibNMInterface::" << __func__
                        << ": User input invalid WEP Key type, psk.length() = "
                        << psk.length() << ", not in [5,10,13,26]");
            }
        }
        else
        {
            DBG("LibNMInterface::" << __func__
                    << ": access point has WPA security");
            g_object_set(settingWifiSecurity, NM_SETTING_WIRELESS_SECURITY_PSK,
                    psk.toRawUTF8(), nullptr);
        }
    }
    nm_client_add_and_activate_connection(nmInterface->nmClient,
            connection,
            nmInterface->nmDevice,
            apPath,
            (NMClientAddActivateFn) handleConnectionAttempt,
            nmInterface);
    return true;
}

/**
 * Runs on the GLib main event loop to remove a wifi connection.
 */
bool LibNMInterface::asyncDisconnect(LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    nm_device_disconnect(nmInterface->nmDevice, nullptr, nullptr);
    return true;
}

/**
 * Runs on the GLib main event loop to update the connected access point's
 * signal strength.
 */
bool LibNMInterface::asyncSignalCheckConnected(LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    ScopedLock updateLock(nmInterface->wifiLock);
    NMAccessPoint* connectedAP = nmInterface->getConnectedNmAp();
    nmInterface->connectedAP.setSignalStrength
            (nm_access_point_get_strength(connectedAP));
    return true;
}

/**
 * Runs on the GLib main event loop to update all access point signal 
 * strengths.
 */
bool LibNMInterface::asyncSignalCheckAll(LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    handleApChange(nmInterface->nmWifiDevice, nmInterface);
    asyncSignalCheckConnected(nmInterface);
    return true;
}

/**
 * Runs on the GLib main event loop to update all wifi data.
 */
bool LibNMInterface::asyncUpdateAll(LibNMInterface* nmInterface)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    ScopedLock updateLock(nmInterface->wifiLock);
    nmInterface->connectedAP = createNMWifiAccessPoint
            (nmInterface->getConnectedNmAp());
    nmInterface->connectingAP = createNMWifiAccessPoint
            (nmInterface->getConnectingNmAp());
    nmInterface->lastNMState = nm_device_get_state(nmInterface->nmDevice);
    ScopedUnlock checkAPUnlock(nmInterface->wifiLock);
    handleApChange(nmInterface->nmWifiDevice, nmInterface);
    nmInterface->confirmWifiState();
    return true;
}

