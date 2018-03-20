#ifdef LINUX
#include <map>
#include <nm-remote-connection.h>
#include <nm-active-connection.h>
#include <nm-utils.h>
#include "JuceHeader.h"
#include "LibNMInterface.h"

#define LIBNM_ITERATION_PERIOD 100 // milliseconds

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

LibNMInterface::LibNMInterface() : Thread("libnmWifiMonitor")
{
    if (!connectToNetworkManager())
    {
        DBG("LibNMInterface::" << __func__
                << ": failed to connect to nmclient over dbus");
        return;
    }
    context = g_main_context_default();

    g_signal_connect_swapped(nmClient,
            "notify::" NM_CLIENT_WIRELESS_ENABLED,
            G_CALLBACK(handleWifiEnabledChange), this);

    g_signal_connect_swapped(nmDevice, "notify::" NM_DEVICE_STATE,
            G_CALLBACK(handleConnectionEvent), this);

    g_signal_connect_swapped(NM_DEVICE_WIFI(nmDevice),
            "notify::" NM_DEVICE_WIFI_ACTIVE_ACCESS_POINT,
            G_CALLBACK(handleConnectionEvent), this);

    g_signal_connect_swapped(NM_DEVICE_WIFI(nmDevice), "access-point-added",
            G_CALLBACK(handleConnectionEvent), this);

    g_signal_connect_swapped(NM_DEVICE_WIFI(nmDevice), "access-point-removed",
            G_CALLBACK(handleConnectionEvent), this);

    if (nm_client_wireless_get_enabled(nmClient))
    {
        startThread();
    }
}

/**
 * Check the NMDevice state to see if wifi is enabled.
 */
bool LibNMInterface::isWifiEnabled()
{
    const ScopedLock lock(wifiLock);
    if (nmDevice != nullptr)
    {
        switch (nm_device_get_state(nmDevice))
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
    }
    return false;
}

/**
 * @return true iff the wifi device is currently connecting to an access point.
 */
bool LibNMInterface::isWifiConnecting()
{
    const ScopedLock lock(wifiLock);
    if (nmDevice != nullptr)
    {
        switch (nm_device_get_state(nmDevice))
        {
            case NM_DEVICE_STATE_PREPARE:
            case NM_DEVICE_STATE_CONFIG:
            case NM_DEVICE_STATE_NEED_AUTH:
            case NM_DEVICE_STATE_IP_CONFIG:
            case NM_DEVICE_STATE_IP_CHECK:
            case NM_DEVICE_STATE_SECONDARIES:
                return true;
        }
    }
    return false;
}

/**
 * Check the NMDevice state to see if wifi is connected.
 */
bool LibNMInterface::isWifiConnected()
{
    const ScopedLock lock(wifiLock);
    if (nmDevice != nullptr)
    {
        DBG("LibNMInterface::" << __func__ << ": state is "
                << deviceStateString(nm_device_get_state(nmDevice)));
        if (nm_device_get_state(nmDevice) == NM_DEVICE_STATE_ACTIVATED)
        {
            return true;
        }
    }
    return false;
}

/**
 * Request information on the connected access point from the NMDevice.
 */
WifiAccessPoint LibNMInterface::getConnectedAP()
{
    const ScopedLock lock(wifiLock);
    NMDeviceWifi* wifiDevice = NM_DEVICE_WIFI(nmDevice);
    NMAccessPoint *ap = nm_device_wifi_get_active_access_point(wifiDevice);
    if (!wifiDevice || !ap)
    {
        DBG("LibNMInterface::" << __func__ << ": no NMAccessPoint found!");
        return WifiAccessPoint();
    }
    return createNMWifiAccessPoint(ap);
}

/**
 * Request information on the connecting access point from the NMDevice.
 */
WifiAccessPoint LibNMInterface::getConnectingAP()
{
    const ScopedLock lock(wifiLock);
    NMActiveConnection* conn =
            nm_client_get_activating_connection(nmClient);
    if (conn == nullptr)
    {
        DBG("LibNMInterface::" << __func__ << ": no connecting AP found, "
                << "returning the connected AP, if any");
        return getConnectedAP();
    }
    NMDeviceWifi* wifiDevice = NM_DEVICE_WIFI(nmDevice);
    NMAccessPoint* connectingAP = nm_device_wifi_get_access_point_by_path
            (NM_DEVICE_WIFI(nmDevice),
            nm_active_connection_get_specific_object(conn));
    return createNMWifiAccessPoint(connectingAP);
}

/**
 * Request information on all wifi access points detected by the NMDevice.
 */
Array<WifiAccessPoint> LibNMInterface::getVisibleAPs()
{
    const ScopedLock lock(wifiLock);
    NMDeviceWifi* wifiDevice = NM_DEVICE_WIFI(nmDevice);
    const GPtrArray* apList = nm_device_wifi_get_access_points(wifiDevice);
    Array<WifiAccessPoint> accessPoints;

    if (apList != nullptr)
    {
        std::map<String, WifiAccessPoint> uniqueAPs;
        for (int i = 0; i < apList->len; i++)
        {
            NMAccessPoint *ap = (NMAccessPoint *) g_ptr_array_index(apList, i);
            WifiAccessPoint createdAP = createNMWifiAccessPoint(ap);

            /*FIXME: dropping hidden (no ssid) networks until gui supports it*/
            if (createdAP.getSSID().length() == 0)
            {
                continue;
            }

            if (uniqueAPs.count(createdAP.getHash()) == 0)
            {
                uniqueAPs[createdAP.getHash()] = createdAP;
            }
            else
            {
                if (createdAP.getSignalStrength() <
                    uniqueAPs[createdAP.getHash()].getSignalStrength())
                {
                    uniqueAPs[createdAP.getHash()] = createdAP;
                }
            }
        }
        for (const auto ap : uniqueAPs)
        {
            accessPoints.add(ap.second);
        }
    }
    DBG("LibNMInterface::" << __func__ << ": found " << accessPoints.size()
            << " AccessPoints");
    return accessPoints;
}

/**
 * Attempt to connect to a wifi access point.
 */
void LibNMInterface::connectToAccessPoint(const WifiAccessPoint& toConnect,
        String psk)
{
    const ScopedLock lock(wifiLock);
    DBG("LibNMInterface::" << __func__ << ": trying to connect to "
            << toConnect.getSSID());
    const char* apPath = nullptr;
    const GPtrArray* apList = nm_device_wifi_get_access_points
            (NM_DEVICE_WIFI(nmDevice));
    if (apList == nullptr)
    {
        DBG("LibNMInterface::" << __func__
                << ": can't connect, no access points found.");
        return;
    }
    NMAccessPoint* candidateAP = nullptr;
    for (int i = 0; i < apList->len; i++)
    {
        candidateAP = (NMAccessPoint*) g_ptr_array_index(apList, i);
        const char* candidateHash = hashAP(candidateAP);
        if (toConnect.getHash() == candidateHash)
        {
            apPath = nm_object_get_path(NM_OBJECT(candidateAP));
            break;
        }
    }
    if (!apPath)
    {
        DBG("LibNMInterface::" << __func__
                << ": no matching access point in list of length "
                << String(apList->len));
        return;
    }
    DBG("LibNMInterface::" << __func__ << ": matching NMAccessPoint found");
    NMConnection* connection = nm_connection_new();
    NMSettingWireless* settingWifi = (NMSettingWireless*)
            nm_setting_wireless_new();
    nm_connection_add_setting(connection, NM_SETTING(settingWifi));
    g_object_set(settingWifi,
            NM_SETTING_WIRELESS_SSID,
            nm_access_point_get_ssid(candidateAP),
            NM_SETTING_WIRELESS_HIDDEN,
            false,
            nullptr);

    if (!psk.isEmpty())
    {
        NMSettingWirelessSecurity* settingWifiSecurity =
                (NMSettingWirelessSecurity*)
                nm_setting_wireless_security_new();
        nm_connection_add_setting(connection, NM_SETTING(settingWifiSecurity));

        if (nm_access_point_get_wpa_flags(candidateAP)
            == NM_802_11_AP_SEC_NONE
            && nm_access_point_get_rsn_flags(candidateAP)
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
    nm_client_add_and_activate_connection(nmClient,
            connection,
            nmDevice,
            apPath,
            handleConnectionAttempt,
            this);
}

/**
 * Close the currently active connection, if one exists.
 */
void LibNMInterface::disconnect()
{
    const ScopedLock lock(wifiLock);
    if (nmDevice != nullptr)
    {
        DBG("LibNMInterface::" << __func__ << ": attempting to disconnect.");
        nm_device_disconnect(nmDevice, nullptr, nullptr);
    }
}

/**
 * Turns on the wifi radio.
 */
void LibNMInterface::enableWifi()
{
    const ScopedLock lock(wifiLock);
    if (!isWifiEnabled())
    {
        DBG("LibNMInterface::" << __func__ << ": enabling wifi connections");
        nm_client_wireless_set_enabled(nmClient, true);
        if (!isThreadRunning())
        {
            startThread();
        }
    }
    else
    {
        DBG("LibNMInterface::" << __func__ << ": wifi is already enabled!");
    }
}

/**
 * Turns off the wifi radio.
 */
void LibNMInterface::disableWifi()
{
    const ScopedLock lock(wifiLock);
    if (isWifiEnabled())
    {
        DBG("LibNMInterface::" << __func__ << ": disabling wifi connections");
        nm_client_wireless_set_enabled(nmClient, false);
    }
    else
    {
        DBG("LibNMInterface::" << __func__ << ": wifi is already disabled!");
    }
}

/**
 * This callback runs after libnm attempts to open a network connection. It
 * primarily serves to catch immediate connection errors.  If it encounters
 * an error, it will inform the Wifi state manager with 
 * signalConnectionFailed()
 */
void LibNMInterface::handleConnectionAttempt(NMClient *client,
        NMActiveConnection *active,
        const char* path,
        GError *err,
        gpointer user_data)
{
    LibNMInterface* wifiStatus = (LibNMInterface *) user_data;
    if (err)
    {
        DBG("LibNMInterface::" << __func__
                << ": failed to add/activate connection!");
        DBG("LibNMInterface::" << __func__ << ": " << err->message);
        wifiStatus->signalConnectionFailed();
    }
}

/**
 * This callback runs when libnm detects that wifi was enabled or disabled.
 * Depending on which event occurs, this will inform the Wifi state manager
 * with either signalWifiEnabled() or signalWifiDisabled().
 */
void LibNMInterface::handleWifiEnabledChange(LibNMInterface* wifiStatus)
{
    bool enabled = nm_client_wireless_get_enabled(wifiStatus->nmClient);
    DBG("LibNMInterface::" << __func__ << ":  changed to "
            << (enabled ? "enabled" : "disabled"));
    //FIXME: Force and wait for a scan after enable
    if (enabled)
    {
        wifiStatus->signalWifiEnabled();
    }
    else
    {
        wifiStatus->signalWifiDisabled();
        wifiStatus->signalThreadShouldExit();
    }
}

/**
 * This callback runs when libnm signals that a connection attempt is 
 * finished.  This will check the status of the connection, and call
 * signalWifiConnected(), signalWifiDisconnected(), or 
 * signalConnectionFailed() depending on the results.
 */
void LibNMInterface::handleConnectionEvent(LibNMInterface* wifiStatus)
{
    NMDeviceState state = nm_device_get_state(wifiStatus->nmDevice);
    DBG("LibNMInterface::" << __func__ << ":  changed to "
            << deviceStateString(state));
    switch (state)
    {
        case NM_DEVICE_STATE_ACTIVATED:
            wifiStatus->signalWifiConnected(wifiStatus->getConnectedAP());
            break;
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
            /* No state change for now, wait for connection to complete/fail */
            break;
        case NM_DEVICE_STATE_NEED_AUTH:
        {
            DBG("LibNMInterface::" << __func__
                    << " missing auth, canceling connection.");
            NMActiveConnection *conn =
                    nm_client_get_activating_connection
                    (wifiStatus->nmClient);
            wifiStatus->removeNMConnection(conn);
        }
            /* FIXME: let this drop into the general failed case for now
             *        eventually this should prompt the user
             */
        case NM_DEVICE_STATE_DISCONNECTED:
            wifiStatus->signalConnectionFailed();
            break;
        case NM_DEVICE_STATE_DEACTIVATING:
        case NM_DEVICE_STATE_FAILED:
            wifiStatus->signalWifiDisconnected();
            break;
        case NM_DEVICE_STATE_UNKNOWN:
        case NM_DEVICE_STATE_UNMANAGED:
        case NM_DEVICE_STATE_UNAVAILABLE:
        default:
            DBG("LibNMInterface::" << __func__
                    << ": wlan0 device entered unmanaged state: "
                    << deviceStateString(state));
            wifiStatus->signalWifiDisconnected();
    }
}

/**
 * Attempt to open a connection to the network manager, and find wifi network 
 * device wlan0.
 */
bool LibNMInterface::connectToNetworkManager()
{
    if (nmClient == nullptr || !NM_IS_CLIENT(nmClient))
    {
        nmClient = nm_client_new();
    }
    if (nmClient == nullptr || !NM_IS_CLIENT(nmClient))
    {
        DBG("LibNMInterface::" << __func__
                << ": failed to connect to nmclient over dbus");
        return false;
    }
    if (nmDevice == nullptr || !NM_IS_DEVICE(nmDevice))
    {
        nmDevice = nm_client_get_device_by_iface(nmClient, "wlan0");
    }
    if (nmDevice == nullptr || !NM_IS_DEVICE(nmDevice))
    {

        DBG("LibNMInterface::" << __func__ <<
                ":  failed to connect to nmdevice wlan0 over dbus");
        return false;
    }
    return true;
}

/**
 * While the wifi status thread is running, regularly check the network
 * manager for wifi network changes.
 */
void LibNMInterface::run()
{
    if (nmClient == nullptr || nmDevice == nullptr)
    {
        DBG("LibNMInterface::" << __func__
                << ": Can't access device wlan0, exiting wifi thread");
        signalWifiDisabled();
        return;
    }

    while (!threadShouldExit())
    {
        {
            const MessageManagerLock mmLock(this);
            if (mmLock.lockWasGained())
            {
                if (isWifiConnecting())
                {
                    signalWifiConnecting();
                }
                bool dispatched = g_main_context_iteration(context, false);
            }
        }
        wait(LIBNM_ITERATION_PERIOD);
    }
}

/**
 * called whenever the application window gains focus.
 */
void LibNMInterface::windowFocusGained()
{
    if (nmClient == nullptr && !connectToNetworkManager())
    {

        DBG("LibNMInterface::" << __func__ << ": Can't connect to wifi device "
                << "wlan0, wifi monitor thread will not start.");
    }
    else if (!isThreadRunning())
    {
        startThread();
    }
}

/**
 * called whenever the application window loses focus.
 */
void LibNMInterface::windowFocusLost()
{
    if (isThreadRunning())
    {
        DBG("LibNMInterface::" << __func__ << ": Suspending wifi thread.");
        signalThreadShouldExit();
    }
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
 * Close a specific wifi connection on wifi device wlan0.
 */
void LibNMInterface::removeNMConnection(NMActiveConnection *conn)
{
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

#endif // LINUX
