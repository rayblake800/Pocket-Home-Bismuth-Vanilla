#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_APHash.h"
#include "Wifi_LibNM_APMode.h"
#include "Wifi_LibNM_SecurityType.h"
#include "Wifi_LibNM_ContextTest.h"
#include "Wifi_LibNM_Thread_Handler.h"
#include "GLib_ObjectPtr.h"
#include <nm-utils.h>

// Creates a null AccessPoint.
Wifi::LibNM::AccessPoint::AccessPoint() :
GLib::Borrowed::Object(NM_TYPE_ACCESS_POINT) { }


// Gets the access point's basic security type.
Wifi::LibNM::SecurityType Wifi::LibNM::AccessPoint::getSecurityType() const
{
    ASSERT_NM_CONTEXT;
    if (getRSNFlags() != NM_802_11_AP_SEC_NONE)
    {
        return SecurityType::securedRSN;
    }
    if (getWPAFlags() != NM_802_11_AP_SEC_NONE)
    {
        return SecurityType::securedWPA;
    }
    return (getFlags() == NM_802_11_AP_FLAGS_NONE) ?
            SecurityType::unsecured : SecurityType::securedWEP;
}


// Gets a hash value that may be used to identify and compare access points,
// treating access points with shared connections as equivalent.
Wifi::LibNM::APHash Wifi::LibNM::AccessPoint::generateHash() const
{
    ASSERT_NM_CONTEXT;
    APMode mode;
    switch(getMode())
    {
        case NM_802_11_MODE_UNKNOWN:
            mode = APMode::unknown;
            break;
        case NM_802_11_MODE_ADHOC:
            mode = APMode::adhoc;
            break;
        case NM_802_11_MODE_INFRA:
            mode = APMode::infrastructure;
            break;
        case NM_802_11_MODE_AP:
            mode = APMode::hotspot;
    }
    return APHash(getSSID(), mode, getSecurityType());
}


// Gets the access point SSID as a byte array from the access point. This may
// contain unprintable characters, and might not be null-terminated.
const GByteArray* Wifi::LibNM::AccessPoint::getSSID() const
{
    ASSERT_NM_CONTEXT;
    const GByteArray* ssid = nullptr;
    if (!isNull())
    {
        NMAccessPoint* nmPtr = getNMObjectPtr();
        ssid = nm_access_point_get_ssid(nmPtr);
    }
    return ssid;
}


// Gets the access point SSID, converted into a printable string object. This
// value should only be used for displaying the access point name to the user,
// or for debug output.
juce::String Wifi::LibNM::AccessPoint::getSSIDText() const
{
    ASSERT_NM_CONTEXT;
    juce::String ssidText;
    const GByteArray* ssid = getSSID();
    if (ssid != nullptr)
    {
        char* utfSSID = nm_utils_ssid_to_utf8(ssid);
        if (utfSSID != nullptr)
        {
            ssidText = utfSSID;
            g_free(utfSSID);
            utfSSID = nullptr;
        }
    }
    return ssidText;
}


// Gets the MAC address of the wifi access point.
const char* Wifi::LibNM::AccessPoint::getBSSID() const
{
    ASSERT_NM_CONTEXT;
    const char* bssid = "";
    if (!isNull())
    {
        bssid = nm_access_point_get_bssid(getNMObjectPtr());
    }
    return bssid;
}


// Gets the signal strength of the wifi access point.
unsigned int Wifi::LibNM::AccessPoint::getSignalStrength() const
{
    ASSERT_NM_CONTEXT;
    if (!isNull())
    {
        return nm_access_point_get_strength(getNMObjectPtr());
    }
    return 0;
}


// Gets the device mode of this access point.
NM80211Mode Wifi::LibNM::AccessPoint::getMode() const
{
    ASSERT_NM_CONTEXT;
    if (!isNull())
    {
        return nm_access_point_get_mode(getNMObjectPtr());
    }
    return NM_802_11_MODE_UNKNOWN;
}


// Gets access point flags for this access point.
NM80211ApFlags Wifi::LibNM::AccessPoint::getFlags() const
{
    ASSERT_NM_CONTEXT;
    if (!isNull())
    {
        return nm_access_point_get_flags(getNMObjectPtr());
    }
    return NM_802_11_AP_FLAGS_NONE;
}


// Gets WPA security flags for this access point.
NM80211ApSecurityFlags Wifi::LibNM::AccessPoint::getWPAFlags() const
{
    ASSERT_NM_CONTEXT;
    if (!isNull())
    {
        return nm_access_point_get_wpa_flags(getNMObjectPtr());
    }
    return NM_802_11_AP_SEC_NONE;
}


// Gets RSN security flags for this access point.
NM80211ApSecurityFlags Wifi::LibNM::AccessPoint::getRSNFlags() const
{
    ASSERT_NM_CONTEXT;
    if (!isNull())
    {
        return nm_access_point_get_rsn_flags(getNMObjectPtr());
    }
    return NM_802_11_AP_SEC_NONE;
}


// Gets the Object's DBus path.
const char* Wifi::LibNM::AccessPoint::getPath() const
{
    ASSERT_NM_CONTEXT;
    const char* path = "";
    if (!isNull())
    {
        path = nm_object_get_path(NM_OBJECT(getGObject()));
        if (path == nullptr)
        {
            path = "";
        }
    }
    return path;
}


static Wifi::LibNM::AccessPoint findAccessPoint(GObject* nmAccessPoint)
{
    using namespace Wifi::LibNM;
    Thread::Handler nmThreadHandler;
    DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
    juce::Array<AccessPoint> allAPs = wifiDevice.getAccessPoints();
    for (const AccessPoint& accessPoint : allAPs)
    {
        if (accessPoint == nmAccessPoint)
        {
            return accessPoint;
        }
    }
    return AccessPoint();
}


// Gets the AccessPoint object's stored LibNM access point data.
NMAccessPoint* Wifi::LibNM::AccessPoint::getNMObjectPtr() const
{
    return NM_ACCESS_POINT(getGObject());
}
