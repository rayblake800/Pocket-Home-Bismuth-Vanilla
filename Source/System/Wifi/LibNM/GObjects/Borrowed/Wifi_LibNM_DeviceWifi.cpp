#define WIFI_IMPLEMENTATION

#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_Connection.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_Thread_Handler.h"
#include "Wifi_LibNM_ContextTest.h"
#include "Wifi_Resource.h"
#include "GLib_ThreadHandler.h"
#include "GLib_ErrorPtr.h"
#include "SharedResource_Modular_Handler.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Wifi::LibNM::DeviceWifi::";
#endif

// Gets a pointer to the container holding ActiveConnection objects managed
// through the wifi device.
Wifi::LibNM::ObjectLenderInterface::ConnectionLender*
Wifi::LibNM::ObjectLenderInterface::getConnectionLender() const
{
    ASSERT_NM_CONTEXT;
    return connectionLender;
}


// Gets a pointer to the container holding AccessPoint objects managed through
// the wifi device.
Wifi::LibNM::ObjectLenderInterface::APLender*
Wifi::LibNM::ObjectLenderInterface::getAPLender() const
{
    ASSERT_NM_CONTEXT;
    return apLender;
}


// Allows the thread module to connect object lenders to the active wifi device
// object.
void Wifi::LibNM::ObjectLenderInterface::setLenders
(ConnectionLender* connectionLender, APLender* apLender)
{
    this->connectionLender = connectionLender;
    this->apLender = apLender;
}


// Creates a null DeviceWifi.
Wifi::LibNM::DeviceWifi::DeviceWifi() :
GLib::Borrowed::Object(NM_TYPE_ACCESS_POINT) { }


// Gets the current state of the wifi network device.
NMDeviceState Wifi::LibNM::DeviceWifi::getState() const
{
    ASSERT_NM_CONTEXT;
    NMDeviceState state = NM_DEVICE_STATE_UNKNOWN;
    if (!isNull())
    {
        state = nm_device_get_state(getDevicePtr());
    }
    return state;
}


// Gets the reason for the current device state.
NMDeviceStateReason Wifi::LibNM::DeviceWifi::getStateReason() const
{
    ASSERT_NM_CONTEXT;
    NMDeviceStateReason reason = NM_DEVICE_STATE_REASON_UNKNOWN;
    if (!isNull())
    {
        nm_device_get_state_reason(getDevicePtr(), &reason);
    }
    return reason;
}


// Checks if NetworkManager manages this device.
bool Wifi::LibNM::DeviceWifi::isManaged() const
{
    ASSERT_NM_CONTEXT;
    if (!isNull())
    {
        return nm_device_get_managed(getDevicePtr());
    }
    return false;
}


// Gets the interface name of this wifi device.
const char* Wifi::LibNM::DeviceWifi::getInterface() const
{
    ASSERT_NM_CONTEXT;
    const char* iface = "";
    if (!isNull())
    {
        iface = nm_device_get_iface(getDevicePtr());
        if (iface == nullptr)
        {
            iface = "";
        }
    }
    return iface;
}


// Gets the DBus path to this wifi device's remote object.
const char* Wifi::LibNM::DeviceWifi::getPath() const
{
    return nm_object_get_path(NM_OBJECT(getGObject()));
}


// Disconnects any connection that is active on this wifi device.
void Wifi::LibNM::DeviceWifi::disconnect() const
{
    ASSERT_NM_CONTEXT;
    if (!isNull())
    {
        nm_device_disconnect(getDevicePtr(), nullptr, nullptr);
    }
}


// Gets the current active connection running on this device.
Wifi::LibNM::ActiveConnection
Wifi::LibNM::DeviceWifi::getActiveConnection() const
{
    ASSERT_NM_CONTEXT;
    if (!isNull())
    {
        if (NMActiveConnection* nmConnection
                = nm_device_get_active_connection(getDevicePtr()))
        {
            if (ConnectionLender* lender = getConnectionLender())
            {
                return lender->borrowObject(G_OBJECT(nmConnection));
            }
        }
    }
    return ActiveConnection();
}


// Gets an access point object using the access point's path.
Wifi::LibNM::AccessPoint Wifi::LibNM::DeviceWifi::getAccessPoint
(const char* path) const
{
    ASSERT_NM_CONTEXT;
    if (!isNull() && path != nullptr)
    {
        if (NMAccessPoint* nmAP = nm_device_wifi_get_access_point_by_path(
                    NM_DEVICE_WIFI(getGObject()), path))
        {
            if (NM_IS_ACCESS_POINT(nmAP))
            {
                if (APLender* lender = getAPLender())
                {
                    return lender->borrowObject(G_OBJECT(nmAP));
                }
            }
        }
    }
    return AccessPoint();
}


// Gets the active connection's access point.
Wifi::LibNM::AccessPoint Wifi::LibNM::DeviceWifi::getActiveAccessPoint() const
{
    ASSERT_NM_CONTEXT;
    if (!isNull())
    {
        if (NMAccessPoint* nmAP = nm_device_wifi_get_active_access_point(
                NM_DEVICE_WIFI(getGObject())))
        {
            if (NM_IS_ACCESS_POINT(nmAP))
            {
                if (APLender* lender = getAPLender())
                {
                    return lender->borrowObject(G_OBJECT(nmAP));
                }
            }
        }
    }
    return AccessPoint();
}


// Gets all access points visible to this device.
juce::Array<Wifi::LibNM::AccessPoint>
Wifi::LibNM::DeviceWifi::getAccessPoints() const
{
    ASSERT_NM_CONTEXT;
    juce::Array<AccessPoint> currentAccessPoints;
    if (isNull())
    {
        DBG(dbgPrefix << __func__ 
                << ": Can't get access points, wifi device is null.");
        return currentAccessPoints;
    }
    APLender* apLender = getAPLender();
    if (apLender == nullptr)
    {
        DBG(dbgPrefix << __func__ << ": Error, failed to get APLender.");
        jassertfalse;
        return currentAccessPoints;
    }

    juce::Array<AccessPoint> removedAccessPoints;
    removedAccessPoints.addArray(apLender->getAllBorrowed());
    if (const GPtrArray* nmAccessPoints
            = nm_device_wifi_get_access_points(getWifiDevicePtr()))
    {
        for (int i = 0; i < nmAccessPoints->len; i++)
        {
            GObject* nmAccessPoint = G_OBJECT(nmAccessPoints->pdata[i]);
            jassert(nmAccessPoint != nullptr);
            AccessPoint foundAccessPoint 
                    = apLender->borrowObject(nmAccessPoint);
            if (!foundAccessPoint.isNull()
                    && foundAccessPoint.getSSID() != nullptr)
            {
                currentAccessPoints.add(foundAccessPoint);
                removedAccessPoints.removeAllInstancesOf(foundAccessPoint);
            }
        }
    }

    // Invalidate any objects in the AccessPoint list not found in the updated
    // list.
    #ifdef JUCE_DEBUG
    if (!removedAccessPoints.isEmpty())
    {
        DBG(dbgPrefix << __func__ << ": Found " << currentAccessPoints.size()
                << " APs, and removing " << removedAccessPoints.size()
                << " APs that are no longer visible");
    }
    #endif
    for (AccessPoint& invalidAP : removedAccessPoints)
    {
        apLender->invalidateObject(invalidAP.getGObject());
    }
    return currentAccessPoints;
}

static void postScanCallback(NMDeviceWifi* device, GError* error,
        gpointer user_data)
{
        DBG(dbgPrefix << __func__ << ": scanned for visible access points.");
        if (error != nullptr)
        {
            DBG(dbgPrefix << __func__ << ": Scanning error: "
                    << (char *) error->message);
        }
}


// Sends a request to the wifi device asking it to scan visible access points.
void Wifi::LibNM::DeviceWifi::requestScan() const
{
    ASSERT_NM_CONTEXT;
    if (!isNull())
    {
        DBG(dbgPrefix << __func__
                << ": requesting scan for visible access points.");
        nm_device_wifi_request_scan_simple(getWifiDevicePtr(),
                postScanCallback, nullptr);
    }
}


// Gets this object's GLib object pointer cast as NMDeviceWifi* data.
NMDeviceWifi* Wifi::LibNM::DeviceWifi::getWifiDevicePtr() const
{
    return NM_DEVICE_WIFI(getGObject());
}


// Gets this object's GLib object pointer cast as NMDevice* data.
NMDevice* Wifi::LibNM::DeviceWifi::getDevicePtr() const
{
    return NM_DEVICE(getGObject());
}
