#include "LibNM/AccessPoint.h"
#include "LibNM/Data/APHash.h"
#include "LibNM/Data/APMode.h"
#include "LibNM/Data/SecurityType.h"
#include "LibNM/ContextTest.h"
#include "GLib/SmartPointers/ObjectPtr.h"
#include <nm-utils.h>

/* Rename smart pointers for brevity: */
typedef GLib::ObjectPtr<NMAccessPoint*> NMAccessPointPtr;
typedef GLib::ObjectPtr<NMObject*> NMObjectPtr;
typedef GLib::ObjectPtr<NMConnection*> NMConnectionPtr;
typedef GLib::ObjectPtr<> ObjectPtr;

/*
 * Create a AccessPoint sharing a GObject with an existing
 * AccessPoint.
 */
LibNM::AccessPoint::AccessPoint(const AccessPoint& toCopy) 
{
    ASSERT_NM_CONTEXT;
    getDataReference() = toCopy.getData();
}

/*
 * Create a AccessPoint to contain a NMAccessPoint object.
 */
LibNM::AccessPoint::AccessPoint(NMAccessPoint* toAssign) 
{ 
    ASSERT_NM_CONTEXT;
    getDataReference() = toAssign;
}
    
/*
 * Creates a null AccessPoint.
 */
LibNM::AccessPoint::AccessPoint() { }

/*
 * Gets the access point's basic security type.
 */
LibNM::SecurityType LibNM::AccessPoint::getSecurityType() const
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

/*
 * Gets a hash value that may be used to identify and compare access points, 
 * treating access points with shared connections as equivalent.
 */
LibNM::APHash LibNM::AccessPoint::generateHash() const
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

/*
 * Gets the access point SSID as a byte array from the access point.  This 
 * may contain unprintable characters, and might not be null-terminated.
 */
const GByteArray* LibNM::AccessPoint::getSSID() const
{
    ASSERT_NM_CONTEXT;
    const GByteArray* ssid = nullptr;
    if(!isNull())
    {
        ssid = nm_access_point_get_ssid(getData());
    }
    return ssid;
}

/*
 * Gets the access point SSID, converted into a printable string object.
 * This value should only be used for displaying the access point name to
 * the user, or for debug output.
 */
juce::String LibNM::AccessPoint::getSSIDText() const
{
    ASSERT_NM_CONTEXT;
    juce::String ssidText;
    const GByteArray* ssid = getSSID();
    if(ssid != nullptr)
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

/*
 * Gets the MAC address of the wifi access point.
 */
const char* LibNM::AccessPoint::getBSSID() const
{
    ASSERT_NM_CONTEXT;
    const char* bssid = "";
    if(!isNull())
    {
        bssid = nm_access_point_get_bssid(getData());
    }
    return bssid;
}

/*
 * Gets the signal strength of the wifi access point.
 */
unsigned int LibNM::AccessPoint::getSignalStrength() const
{
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        return nm_access_point_get_strength(getData());
    }
    return 0;
}

/*
 * Gets the device mode of this access point.
 */
NM80211Mode LibNM::AccessPoint::getMode() const
{
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        return nm_access_point_get_mode(getData());
    }
    return NM_802_11_MODE_UNKNOWN;
}

/*
 * Gets access point flags for this access point.
 */
NM80211ApFlags LibNM::AccessPoint::getFlags() const
{
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        return nm_access_point_get_flags(getData());
    }
    return NM_802_11_AP_FLAGS_NONE;
}

/*
 * Gets WPA security flags for this access point.
 */
NM80211ApSecurityFlags LibNM::AccessPoint::getWPAFlags() const
{
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        return nm_access_point_get_wpa_flags(getData());
    }
    return NM_802_11_AP_SEC_NONE;
}

/*
 * Gets RSN security flags for this access point.
 */
NM80211ApSecurityFlags LibNM::AccessPoint::getRSNFlags() const
{
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        return nm_access_point_get_rsn_flags(getData());
    }
    return NM_802_11_AP_SEC_NONE;
}

/*
 * Directly accesses the AccessPoint object's stored data object.
 */
NMAccessPoint* LibNM::AccessPoint::getNMData() const
{
    return getData();
}

/*
 * Gets the Object's DBus path.
 */
const char* LibNM::AccessPoint::getPath() const
{ 
    ASSERT_NM_CONTEXT;
    const char* path = "";
    if(!isNull())
    {
        path = nm_object_get_path(NM_OBJECT(getData()));
        if(path == nullptr)
        {
            path = "";
        }
    }
    return path;
}

/*
 * Subscribes to signal strength signals from a single NMAccessPoint.
 */
void LibNM::AccessPoint::Listener::connectAllSignals(GObject* source)
{
    ASSERT_NM_CONTEXT;
    if(source != nullptr && NM_IS_ACCESS_POINT(source))
    {
        connectNotifySignal(source, NM_ACCESS_POINT_STRENGTH);
    }
}

/*
 * Builds signalStrengthChanged() calls from generic property change 
 * notifications.
 */
void LibNM::AccessPoint::Listener::propertyChanged
(GObject* source, juce::String property) 
{ 
    ASSERT_NM_CONTEXT;
    if(property == NM_ACCESS_POINT_STRENGTH && NM_IS_ACCESS_POINT(source))
    {
        g_object_ref(source);
        AccessPoint tempAP(NM_ACCESS_POINT(source));
        unsigned int strength = tempAP.getSignalStrength();
        signalStrengthChanged(tempAP, strength);
    }
}
 
/*
 * Adds a new listener to receive updates from this access point.
 */
void LibNM::AccessPoint::addListener(AccessPoint::Listener& listener)
{
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        listener.connectAllSignals(G_OBJECT(getData()));
    }
}
