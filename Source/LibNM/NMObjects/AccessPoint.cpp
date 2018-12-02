#include "LibNM/NMObjects/AccessPoint.h"
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
LibNM::AccessPoint::AccessPoint(const AccessPoint& toCopy) :
LibNM::Object(toCopy, NM_TYPE_ACCESS_POINT) 
{
    ASSERT_CORRECT_CONTEXT;
}

/*
 * Create a AccessPoint to contain a NMAccessPoint object.
 */
LibNM::AccessPoint::AccessPoint(NMAccessPoint* toAssign) :
LibNM::Object(NM_OBJECT(toAssign), NM_TYPE_ACCESS_POINT) 
{
    ASSERT_CORRECT_CONTEXT;
}
    
/*
 * Creates a null AccessPoint.
 */
LibNM::AccessPoint::AccessPoint() : LibNM::Object(NM_TYPE_ACCESS_POINT) { }

/*
 * Gets the access point SSID as a byte array from the access point.  This 
 * may contain unprintable characters, and might not be null-terminated.
 */
const GByteArray* LibNM::AccessPoint::getSSID() const
{
    ASSERT_CORRECT_CONTEXT;
    const GByteArray* ssid = nullptr;
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        ssid = nm_access_point_get_ssid(accessPoint);
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
    ASSERT_CORRECT_CONTEXT;
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
    ASSERT_CORRECT_CONTEXT;
    const char* bssid = "";
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        bssid = nm_access_point_get_bssid(accessPoint);
    }
    return bssid;
}

/*
 * Gets the signal strength of the wifi access point.
 */
unsigned int LibNM::AccessPoint::getSignalStrength() const
{
    ASSERT_CORRECT_CONTEXT;
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_strength(accessPoint);
    }
    return 0;
}

/*
 * Checks if a connection could potentially be activated with this access point.
 */
bool LibNM::AccessPoint::isValidConnection(const Connection& connection) const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr nmConnection(NM_CONNECTION(getOtherGObject(connection)));
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(nmConnection == nullptr || accessPoint == nullptr)
    {
        return false;
    }
    return nm_access_point_connection_valid(accessPoint, nmConnection);
}

/*
 * Gets the device mode of this access point.
 */
NM80211Mode LibNM::AccessPoint::getMode() const
{
    ASSERT_CORRECT_CONTEXT;
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_mode(accessPoint);
    }
    return NM_802_11_MODE_UNKNOWN;
}

/*
 * Gets access point flags for this access point.
 */
NM80211ApFlags LibNM::AccessPoint::getFlags() const
{
    ASSERT_CORRECT_CONTEXT;
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_flags(accessPoint);
    }
    return NM_802_11_AP_FLAGS_NONE;
}

/*
 * Gets WPA security flags for this access point.
 */
NM80211ApSecurityFlags LibNM::AccessPoint::getWPAFlags() const
{
    ASSERT_CORRECT_CONTEXT;
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_wpa_flags(accessPoint);
    }
    return NM_802_11_AP_SEC_NONE;
}

/*
 * Gets RSN security flags for this access point.
 */
NM80211ApSecurityFlags LibNM::AccessPoint::getRSNFlags() const
{
    ASSERT_CORRECT_CONTEXT;
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_rsn_flags(accessPoint);
    }
    return NM_802_11_AP_SEC_NONE;
}

/*
 * Subscribes to signal strength signals from a single NMAccessPoint.
 */
void LibNM::AccessPoint::Listener::connectAllSignals(GObject* source)
{
    ASSERT_CORRECT_CONTEXT;
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
    ASSERT_CORRECT_CONTEXT;
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
    ASSERT_CORRECT_CONTEXT;
    ObjectPtr apObject(getGObject());
    if(apObject != nullptr)
    {
        listener.connectAllSignals(apObject);
    }
}
