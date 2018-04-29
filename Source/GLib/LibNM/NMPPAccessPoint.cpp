#include "nm-utils.h"
#include "NMPPAccessPoint.h"

/*
 * Create a NMPPAccessPoint sharing a GObject with an existing
 * NMPPAccessPoint.
 */
NMPPAccessPoint::NMPPAccessPoint(const NMPPAccessPoint& toCopy)
{ 
    setGObject(toCopy);
}

/*
 * Create a NMPPAccessPoint to contain a NMAccessPoint object.
 */
NMPPAccessPoint::NMPPAccessPoint(NMAccessPoint* toAssign)
{
    setGObject(G_OBJECT(toAssign));
}

/*
 * Gets the access point SSID as a byte array from the access point.  This 
 * may contain unprintable characters, and might not be null-terminated.
 */
const GByteArray* NMPPAccessPoint::getSSID() const
{
    const GByteArray* ssid = nullptr;
    callInMainContext([&ssid](GObject* apObject)
    {
        NMAccessPoint* accessPoint = NM_ACCESS_POINT(apObject);
        if(accessPoint != nullptr)
        {
            ssid = nm_access_point_get_ssid(accessPoint);
        }
    });
    return ssid;
}

/*
 * Gets the access point SSID, converted into a printable string object.
 * This value should only be used for displaying the access point name to
 * the user, or for debug output.
 */
String NMPPAccessPoint::getSSIDText() const
{
    String ssidText;
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
    callInMainContext([&bssid](GObject* apObject)
    {
        NMAccessPoint* accessPoint = NM_ACCESS_POINT(apObject);
        if(accessPoint != nullptr)
        {
            bssid = nm_access_point_get_bssid(accessPoint);
        }
    });
    return bssid;
}

/*
 * Gets the DBus path of the wifi access point.
 */
const char* NMPPAccessPoint::getPath() const
{
    const char* path = "";
    callInMainContext([&path](GObject* apObject)
    {
        NMObject* accessPoint = NM_OBJECT(apObject);
        if(accessPoint != nullptr)
        {
            path = nm_object_get_path(accessPoint);
        }
    });
    return path;
}


/*
 * Gets the wifi access point frequency in (TODO: what format? MHz? 
 * documentation is unclear, do some tests and figure it out.)
 */
unsigned int NMPPAccessPoint::getFrequency() const
{
    unsigned int frequency = 0;
    callInMainContext([&frequency](GObject* apObject)
    {
        NMAccessPoint* accessPoint = NM_ACCESS_POINT(apObject);
        if(accessPoint != nullptr)
        {
            frequency = nm_access_point_get_frequency(accessPoint);
        }
    });
    return frequency;
}

/*
 * Gets the access point's maximum data transfer bit rate.
 */
unsigned int NMPPAccessPoint::getMaxBitrate() const
{
    unsigned int maxBitrate = 0;
    callInMainContext([&maxBitrate](GObject* apObject)
    {
        NMAccessPoint* accessPoint = NM_ACCESS_POINT(apObject);
        if(accessPoint != nullptr)
        {
            maxBitrate = nm_access_point_get_max_bitrate(accessPoint);
        }
    });
    return maxBitrate;
}

/*
 * Gets the signal strength of the wifi access point.
 */
unsigned int NMPPAccessPoint::getSignalStrength() const
{
    unsigned int signalStrength = 0;
    callInMainContext([&signalStrength](GObject* apObject)
    {
        NMAccessPoint* accessPoint = NM_ACCESS_POINT(apObject);
        if(accessPoint != nullptr)
        {
            signalStrength = nm_access_point_get_strength(accessPoint);
        }
    });
    return signalStrength;
}

/*
 * Check the settings of a connection against the properties of this access
 * point to see if the connection could be activated with this access point.
 */
bool NMPPAccessPoint::isValidConnection(const NMPPConnection& connection) const
{
    bool isValid = false;
    NMConnection* nmConn = NM_CONNECTION(getOtherGObject(connection));
    callInMainContext([&isValid, &connection, nmConn](GObject* apObject)
    {
        NMAccessPoint* accessPoint = NM_ACCESS_POINT(apObject);
        if(accessPoint != nullptr && nmConn != nullptr)
        {
            isValid = nm_access_point_connection_valid(accessPoint, nmConn);
        }
    });
    if(nmConn != nullptr)
    {
        g_object_unref(G_OBJECT(nmConn));
    }
    return isValid;
}

/*
 * Get access point flags for this access point.
 */
NM80211ApFlags NMPPAccessPoint::getFlags() const
{
    NM80211ApFlags flags = NM_802_11_AP_FLAGS_NONE;
    callInMainContext([&flags](GObject* apObject)
    {
        NMAccessPoint* accessPoint = NM_ACCESS_POINT(apObject);
        if(accessPoint != nullptr)
        {
            flags = nm_access_point_get_flags(accessPoint);
        }
    });
    return flags;
}

/*
 * Get WPA security flags for this access point.
 */
NM80211ApSecurityFlags NMPPAccessPoint::getWPAFlags() const
{
    NM80211ApSecurityFlags flags = NM_802_11_AP_SEC_NONE;
    callInMainContext([&flags](GObject* apObject)
    {
        NMAccessPoint* accessPoint = NM_ACCESS_POINT(apObject);
        if(accessPoint != nullptr)
        {
            flags = nm_access_point_get_wpa_flags(accessPoint);
        }
    });
    return flags;
}

/*
 * Get RSN security flags for this access point.
 */
NM80211ApSecurityFlags NMPPAccessPoint::getRSNFlags() const
{
    NM80211ApSecurityFlags flags = NM_802_11_AP_SEC_NONE;
    callInMainContext([&flags](GObject* apObject)
    {
        NMAccessPoint* accessPoint = NM_ACCESS_POINT(apObject);
        if(accessPoint != nullptr)
        {
            flags = nm_access_point_get_rsn_flags(accessPoint);
        }
    });
    return flags;
}

/*
 * Build NMPPAccessPoint::Listener::signalStrengthChanged() calls from generic 
 * property change notifications.
 */
void NMPPAccessPoint::Listener::propertyChanged
(GPPObject* source, String property) 
{ 
    NMPPAccessPoint* ap = dynamic_cast<NMPPAccessPoint*>(source);
    if(ap != nullptr && property == NM_ACCESS_POINT_STRENGTH)
    {
        unsigned int strength = ap->getSignalStrength();
        signalStrengthChanged(ap, strength);
    }
}

/*
 * Add a new listener object to receive signals from this access point.
 */
void NMPPAccessPoint::addListener(Listener* listener) 
{ 
    addNotifySignalHandler(listener, NM_ACCESS_POINT_STRENGTH);
}

/*
 * Remove a listener object from this access point.
 */
void NMPPAccessPoint::removeListener(Listener* listener) 
{ 
    removeSignalHandler(listener);
}

/*
 * Get the NMAccessPoint class GType
 */
GType NMPPAccessPoint::getType() const 
{ 
    return NM_TYPE_ACCESS_POINT;
}

/*
 * Check if a GObject's type allows it to be held by this object. 
 */
bool NMPPAccessPoint::isValidType(GObject* toCheck) const 
{ 
    return NM_IS_ACCESS_POINT(toCheck);
}

/*
 * Used to re-add a list of signal handlers to new GObject data.
 */
void NMPPAccessPoint::transferSignalHandlers
(Array<SignalHandler*>& toTransfer) 
{
    for(SignalHandler* handler : toTransfer)
    {
        addListener(static_cast<Listener*>(handler));
    }
}