#include <glib-2.0/glib/gslist.h>

#include "JuceHeader.h"
#include "Utils.h"
#include "WifiAccessPoint.h"

#ifdef LINUX
#include "GLibSignalHandler.h"
#endif

WifiAccessPoint::WifiAccessPoint
(String ssid, int signalStrength, bool requiresAuth, String hash) :
ssid(ssid),
signalStrength(median<int>(0, signalStrength, 100)),
requiresAuth(requiresAuth),
hash(hash) { }

#ifdef LINUX

/**
 * @param accessPoint  A LibNM access point object.
 */
WifiAccessPoint::WifiAccessPoint(NMAccessPoint* accessPoint) :
nmAP(accessPoint)
{
    GLibSignalHandler glibHandler;
    glibHandler.gLibCall([this]()
    {
        /**
         * Generate a unique hash string identifying a wifi access point.
         * Adapted from network-manager-applet src/utils/utils.c 
         */
        const GByteArray* rawSSID = nm_access_point_get_ssid(nmAP);
        NM80211Mode mode = nm_access_point_get_mode(nmAP);
        guint32 flags = nm_access_point_get_flags(nmAP);
        guint32 wpa_flags = nm_access_point_get_wpa_flags(nmAP);
        guint32 rsn_flags = nm_access_point_get_rsn_flags(nmAP);
        unsigned char input[66];

        memset(&input[0], 0, sizeof (input));

        if (rawSSID != nullptr)
        {
            memcpy(input, rawSSID->data, rawSSID->len);
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

        hash = g_compute_checksum_for_data
                (G_CHECKSUM_MD5, input, sizeof (input));

        //get ssid and bssid
        if (rawSSID != nullptr)
        {
            ssid = nm_utils_ssid_to_utf8(rawSSID);
        }
        bssid = nm_access_point_get_bssid(nmAP);

        //get security type
        if (rsn_flags != NM_802_11_AP_SEC_NONE)
        {
            security = securedRSN;
        }
        else if (nm_access_point_get_wpa_flags(nmAP)
                 != NM_802_11_AP_SEC_NONE)
        {
            security = securedWPA;
        }
        else
        {
            security = (flags == NM_802_11_AP_FLAGS_NONE) ?
                    none : securedWEP;
        }
        signalStrength = nm_access_point_get_strength(nmAP);

    });
}

#endif  

/**
 * Creates a void WifiAccessPoint, representing the absence of a wifi access 
 * point.
 */
WifiAccessPoint::WifiAccessPoint() :
signalStrength(-1),
security(none) { }

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
 * @return  the wifi access point frequency.
 */
unsigned long WifiAccessPoint::getFrequency()
{
    int frequency = 0;
#    ifdef LINUX
    if (nmAP != nullptr)
    {
        GLibSignalHandler glibHandler;
        glibHandler.gLibCall([this, &frequency]()
        {
            frequency = nm_access_point_get_frequency(nmAP);
        });
    }
#    endif  
    return frequency;
}

/**
 * @return  the maximum access point bitrate in kb/s
 */
unsigned long WifiAccessPoint::getMaxBitrate()
{
    int bitrate = 0;
#    ifdef LINUX
    if (nmAP != nullptr)
    {
        GLibSignalHandler glibHandler;
        glibHandler.gLibCall([this, &bitrate]()
        {
            bitrate = nm_access_point_get_max_bitrate(nmAP);
        });
    }
#    endif  
    return bitrate;
}


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
Array<NMConnection*> WifiAccessPoint::filterConnections
(const GSList* connections)
{
    Array<NMConnection*> compatible;
    if (nmAP != nullptr)
    {
        GLibSignalHandler glibHandler;
        glibHandler.gLibCall([this, connections, &compatible]()
        {
            GSList* filtered = nm_access_point_filter_connections
                    (nmAP, connections);
            for (GSList* connection = filtered;
                 connection != nullptr;
                 connection = connection->next)
            {
                if (connection->data != nullptr)
                {
                    compatible.add((NMConnection*) connection->data);
                }
            }
            g_slist_free(filtered);
        });
    }
    return compatible;
}
#endif

/**
 * @return true iff the connection could be activated with this access 
 *          point.
 */
bool WifiAccessPoint::isValidConnection(NMConnection* connection)
{
    bool isValid = false;
    if (nmAP != nullptr)
    {
        GLibSignalHandler glibHandler;
        glibHandler.gLibCall([this, connection, &isValid]()
        {
            isValid = nm_access_point_connection_valid(nmAP, connection);
        });
    }
    return isValid;
}
#endif

/**
 * Update the access point signal strength.
 */
void WifiAccessPoint::updateSignalStrength()
{
#    ifdef LINUX
    if (nmAP != nullptr)
    {
        GLibSignalHandler glibHandler;
        glibHandler.gLibCall([this]()
        {
            signalStrength = nm_access_point_get_strength(nmAP);
        });
    }
#    endif  
}
