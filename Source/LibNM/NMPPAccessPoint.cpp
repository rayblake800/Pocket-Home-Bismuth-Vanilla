#include <nm-utils.h>
#include "Utils.h"
#include "GLib/SmartPointers/ObjectPtr.h"
#include "NMPPAccessPoint.h"

/* Rename smart pointers for brevity: */
typedef GLib::ObjectPtr<NMAccessPoint*> NMAccessPointPtr;
typedef GLib::ObjectPtr<NMObject*> NMObjectPtr;
typedef GLib::ObjectPtr<NMConnection*> NMConnectionPtr;
typedef GLib::ObjectPtr<> ObjectPtr;

/*
 * Create a NMPPAccessPoint sharing a GObject with an existing
 * NMPPAccessPoint.
 */
NMPPAccessPoint::NMPPAccessPoint(const NMPPAccessPoint& toCopy) :
GLib::Object(toCopy, NM_TYPE_ACCESS_POINT) { }

/*
 * Create a NMPPAccessPoint to contain a NMAccessPoint object.
 */
NMPPAccessPoint::NMPPAccessPoint(NMAccessPoint* toAssign) :
GLib::Object(G_OBJECT(toAssign), NM_TYPE_ACCESS_POINT) { }
    
/**
 * Creates a null NMPPAccessPoint.
 */
NMPPAccessPoint::NMPPAccessPoint() : GLib::Object(NM_TYPE_ACCESS_POINT) { }

/*
 * Gets the access point SSID as a byte array from the access point.  This 
 * may contain unprintable characters, and might not be null-terminated.
 */
const GByteArray* NMPPAccessPoint::getSSID() const
{
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
juce::String NMPPAccessPoint::getSSIDText() const
{
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
const char* NMPPAccessPoint::getBSSID() const
{
    const char* bssid = "";
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        bssid = nm_access_point_get_bssid(accessPoint);
    }
    return bssid;
}

/*
 * Gets the DBus path of the wifi access point.
 */
const char* NMPPAccessPoint::getPath() const
{
    const char* path = "";
    NMObjectPtr accessPoint(NM_OBJECT(getGObject()));
    if(accessPoint != nullptr)
    {
        path = nm_object_get_path(accessPoint);
        if(path == nullptr)
        {
            path = "";
        }
    }
    return path;
}

/*
 * Gets the wifi access point frequency in (TODO: what format? MHz? 
 * documentation is unclear, do some tests and figure it out.)
 */
unsigned int NMPPAccessPoint::getFrequency() const
{
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_frequency(accessPoint);
    }
    return 0;
}

/*
 * Gets the access point's maximum data transfer bit rate.
 */
unsigned int NMPPAccessPoint::getMaxBitrate() const
{
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_max_bitrate(accessPoint);
    }
    return 0;
}

/*
 * Gets the signal strength of the wifi access point.
 */
unsigned int NMPPAccessPoint::getSignalStrength() const
{
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_strength(accessPoint);
    }
    return 0;
}

/*
 * Check the settings of a connection against the properties of this access
 * point to see if the connection could be activated with this access point.
 */
bool NMPPAccessPoint::isValidConnection(const NMPPConnection& connection) const
{
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
NM80211Mode NMPPAccessPoint::getMode() const
{
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_mode(accessPoint);
    }
    return NM_802_11_MODE_UNKNOWN;
}

/*
 * Get access point flags for this access point.
 */
NM80211ApFlags NMPPAccessPoint::getFlags() const
{
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_flags(accessPoint);
    }
    return NM_802_11_AP_FLAGS_NONE;
}

/*
 * Get WPA security flags for this access point.
 */
NM80211ApSecurityFlags NMPPAccessPoint::getWPAFlags() const
{
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_wpa_flags(accessPoint);
    }
    return NM_802_11_AP_SEC_NONE;
}

/*
 * Get RSN security flags for this access point.
 */
NM80211ApSecurityFlags NMPPAccessPoint::getRSNFlags() const
{
    NMAccessPointPtr accessPoint(NM_ACCESS_POINT(getGObject()));
    if(accessPoint != nullptr)
    {
        return nm_access_point_get_rsn_flags(accessPoint);
    }
    return NM_802_11_AP_SEC_NONE;
}

/*
 * Subscribe to signal strength signals from a single NMAccessPoint.
 */
void NMPPAccessPoint::Listener::connectAllSignals(GObject* source)
{
    if(source != nullptr && NM_IS_ACCESS_POINT(source))
    {
        connectNotifySignal(source, NM_ACCESS_POINT_STRENGTH);
    }
}

/*
 * Build NMPPAccessPoint::Listener::signalStrengthChanged() calls from generic 
 * property change notifications.
 */
void NMPPAccessPoint::Listener::propertyChanged
(GObject* source, juce::String property) 
{ 
    if(property == NM_ACCESS_POINT_STRENGTH && NM_IS_ACCESS_POINT(source))
    {
        g_object_ref(source);
        NMPPAccessPoint tempAP(NM_ACCESS_POINT(source));
        unsigned int strength = tempAP.getSignalStrength();
        signalStrengthChanged(tempAP, strength);
    }
}
 
/**
 * Add a new listener to receive updates from this access point.
 */
void NMPPAccessPoint::addListener(NMPPAccessPoint::Listener& listener)
{
    ObjectPtr apObject(getGObject());
    if(apObject != nullptr)
    {
        listener.connectAllSignals(apObject);
    }
}
