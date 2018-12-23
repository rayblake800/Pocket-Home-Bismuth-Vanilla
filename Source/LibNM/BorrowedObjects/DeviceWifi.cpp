#include "LibNM/NMObjects/DeviceWifi.h"
#include "LibNM/AccessPoint.h"
#include "LibNM/NMObjects/Connection.h"
#include "LibNM/NMObjects/ActiveConnection.h"
#include "LibNM/ContextTest.h"
#include "GLib/SmartPointers/ObjectPtr.h"

/* Rename smart pointers for brevity: */
typedef GLib::ObjectPtr<NMObject*> NMObjectPtr;
typedef GLib::ObjectPtr<NMDevice*> NMDevicePtr;
typedef GLib::ObjectPtr<NMDeviceWifi*> NMDeviceWifiPtr;
typedef GLib::ObjectPtr<> ObjectPtr;

/* Object property keys: */
static const constexpr char * activeConnectionProperty = "active-connection";

/* Object signal keys: */
static const constexpr char * stateChangeSignal = "state-changed";
static const constexpr char * addedAPSignal     = "access-point-added";
static const constexpr char * removedAPSignal   = "access-point-removed";

/*
 * Creates a DeviceWifi object that shares a NMDeviceWifi* with another 
 * DeviceWifi object
 */
LibNM::DeviceWifi::DeviceWifi(const DeviceWifi& toCopy) :
LibNM::Object(toCopy, NM_TYPE_DEVICE_WIFI) { }

/*
 * Create a DeviceWifi to contain a NMDeviceWifi object.
 */
LibNM::DeviceWifi::DeviceWifi(NMDeviceWifi* toAssign) :
LibNM::Object(NM_OBJECT(toAssign), NM_TYPE_DEVICE_WIFI) 
{ 
    ASSERT_NM_CONTEXT;
}

/*
 * Creates a null DeviceWifi.
 */
LibNM::DeviceWifi::DeviceWifi() :
LibNM::Object(NM_TYPE_DEVICE_WIFI) { }

/*
 * Gets the current state of the wifi network device.
 */
NMDeviceState LibNM::DeviceWifi::getState() const
{ 
    ASSERT_NM_CONTEXT;
    NMDeviceState state = NM_DEVICE_STATE_UNKNOWN;
    NMDevicePtr device(NM_DEVICE(getGObject()));
    if(device != nullptr)
    {
        state = nm_device_get_state(device);
    }
    return state;
}

/*
 * Gets the reason for the current device state.
 */
NMDeviceStateReason LibNM::DeviceWifi::getStateReason() const
{ 
    ASSERT_NM_CONTEXT;
    NMDeviceStateReason reason = NM_DEVICE_STATE_REASON_UNKNOWN;
    NMDevicePtr device(NM_DEVICE(getGObject()));
    if(device != nullptr)
    {
        nm_device_get_state_reason(device, &reason);
    }
    return reason;
}

/*
 * Checks if NetworkManager manages this device.
 */
bool LibNM::DeviceWifi::isManaged() const
{ 
    ASSERT_NM_CONTEXT;
    NMDevicePtr device(NM_DEVICE(getGObject()));
    if(device != nullptr)
    {
        return nm_device_get_managed(device);
    }
    return false;
}

/*
 * Gets the interface name of this wifi device.
 */
const char* LibNM::DeviceWifi::getInterface() const
{ 
    ASSERT_NM_CONTEXT;
    const char* iface = "";
    NMDevicePtr device(NM_DEVICE(getGObject()));
    if(device != nullptr)
    {
        iface = nm_device_get_iface(device);
        if(iface == nullptr)
        {
            iface = "";
        }
    }
    return iface;
}

/*
 * Disconnects any connection that is active on this wifi device.
 */
void LibNM::DeviceWifi::disconnect() 
{ 
    ASSERT_NM_CONTEXT;
    NMDevicePtr device(NM_DEVICE(getGObject()));
    if(device != nullptr)
    {
        nm_device_disconnect(device, nullptr, nullptr);
    }
}

/*
 * Gets the current active connection running on this device.
 */
LibNM::ActiveConnection LibNM::DeviceWifi::getActiveConnection() const
{ 
    ASSERT_NM_CONTEXT;
    NMDevicePtr device(NM_DEVICE(getGObject()));
    if(device != nullptr)
    {
        NMActiveConnection* con = nm_device_get_active_connection(device);
        if(con != nullptr)
        {
            g_object_ref(G_OBJECT(con));
            return ActiveConnection(con);
        }
    }
    return ActiveConnection();
}
  
/*
 * Gets the list of connections available to activate on this device.
 */
juce::Array<LibNM::Connection> 
LibNM::DeviceWifi::getAvailableConnections() const
{ 
    ASSERT_NM_CONTEXT;
    juce::Array<Connection> availableConnections;
    NMDevicePtr device(NM_DEVICE(getGObject()));
    if(device != nullptr)
    {
        const GPtrArray* cons = nm_device_get_available_connections(device);
        for(int i = 0; cons && i < cons->len; i++)
        {
            NMConnection* connection = NM_CONNECTION(cons->pdata[i]);
            if(connection != nullptr)
            {
                g_object_ref(G_OBJECT(connection));
                availableConnections.add(Connection(connection));
            }
        }
    }
    return availableConnections;
}

/*
 * Finds the first available connection that is compatible with a specific wifi 
 * access point.
 */
LibNM::Connection LibNM::DeviceWifi::getAvailableConnection
(const AccessPoint& accessPoint) const
{
    ASSERT_NM_CONTEXT;
    if(!accessPoint.isNull())
    {
        juce::Array<Connection> available = getAvailableConnections();
        for(const Connection& con : available)
        {
            if(con.isCompatibleAccessPoint(accessPoint))
            {
                return con;
            }
        }
    }
    return Connection();
}

/*
 * Gets an access point object using the access point's path.
 */
LibNM::AccessPoint LibNM::DeviceWifi::getAccessPoint(const char* path) const
{ 
    ASSERT_NM_CONTEXT;
    NMDeviceWifiPtr device(NM_DEVICE_WIFI(getGObject()));
    if(device != nullptr && path != nullptr)
    {
        // Temporary change to help track a segfault here:
        //NMAccessPoint* nmAP = nm_device_wifi_get_access_point_by_path
        //        (device, path);
        //if(nmAP != nullptr && NM_IS_ACCESS_POINT(nmAP))
        //{
        //    g_object_ref(G_OBJECT(nmAP));
        //    return AccessPoint(nmAP);
        //}

        juce::String pathString(path);
        juce::Array<AccessPoint> accessPoints = getAccessPoints();
        for(const AccessPoint& ap : accessPoints)
        {
            if(pathString == ap.getPath())
            {
                return ap;
            }
        }
    }
    return AccessPoint();
}
  
/*
 * Gets the active connection's access point.
 */
LibNM::AccessPoint LibNM::DeviceWifi::getActiveAccessPoint() const
{ 
    ASSERT_NM_CONTEXT;
    NMDeviceWifiPtr device(NM_DEVICE_WIFI(getGObject()));
    if(device != nullptr)
    {
        NMAccessPoint* nmAP = nm_device_wifi_get_active_access_point(device);
        if(nmAP != nullptr && NM_IS_ACCESS_POINT(nmAP))
        {
            // Are the access points getting unreffed to death?
            g_object_ref(G_OBJECT(nmAP)); //REMOVE AFTER TEST
            g_object_ref(G_OBJECT(nmAP));
            return AccessPoint(nmAP);
        }
    }
    return AccessPoint();
}

/*
 * Gets all access points visible to this device.
 */
juce::Array<LibNM::AccessPoint> LibNM::DeviceWifi::getAccessPoints() const
{ 
    ASSERT_NM_CONTEXT;
    juce::Array<AccessPoint> accessPoints;
    NMDeviceWifiPtr device(NM_DEVICE_WIFI(getGObject()));
    if(device != nullptr)
    {
        const GPtrArray* aps = nm_device_wifi_get_access_points(device);
        for(int i = 0; aps && i < aps->len; i++)
        {
            jassert(NM_IS_ACCESS_POINT(aps->pdata[i]));
            NMAccessPoint* nmAP = NM_ACCESS_POINT(aps->pdata[i]);
            if(nmAP != nullptr)
            {
                g_object_ref(G_OBJECT(nmAP));
                accessPoints.add(AccessPoint(nmAP));
            }
        }
    }
    return accessPoints;
}
    
/*
 * Checks if a specific connection is present in the list of available device 
 * connections.
 */
bool LibNM::DeviceWifi::hasConnectionAvailable(const Connection& toFind) const
{
    ASSERT_NM_CONTEXT;
    if(toFind.isNull())
    {
        return false;
    }
    return getAvailableConnections().contains(toFind);
}

/*
 * Sends a request to the wifi device asking it to scan visible access points.
 */
void LibNM::DeviceWifi::requestScan() 
{ 
    ASSERT_NM_CONTEXT;
    NMDeviceWifiPtr device(NM_DEVICE_WIFI(getGObject()));
    if(device != nullptr)
    {
        nm_device_wifi_request_scan_simple(device, nullptr, nullptr);
    }
}

/*
 * Subscribes to all relevant signals from a single GObject signal source.
 */
void LibNM::DeviceWifi::Listener::connectAllSignals(GObject* source) 
{
    ASSERT_NM_CONTEXT;
    if(source != nullptr && NM_IS_DEVICE_WIFI(source))
    {
        connectSignal(source, stateChangeSignal, 
                G_CALLBACK(stateChangeCallback));
        connectSignal(source, addedAPSignal, 
                G_CALLBACK(apAddedCallback));
        connectSignal(source, removedAPSignal,
                G_CALLBACK(apRemovedCallback));
        connectNotifySignal(source, activeConnectionProperty);
    }
}
 
/*
 * Converts generic property change notifications into activeConnectionChanged 
 * calls.
 */
void LibNM::DeviceWifi::Listener::propertyChanged(GObject* source,
        juce::String property)
{
    ASSERT_NM_CONTEXT;
    jassert(property == activeConnectionProperty);
    if(source != nullptr && NM_IS_DEVICE_WIFI(source)) 
    {
        DeviceWifi device(NM_DEVICE_WIFI(source));
        activeConnectionChanged(device.getActiveConnection());
    }
}

/*
 * Adds a listener to this wifi device.
 */
void LibNM::DeviceWifi::addListener(DeviceWifi::Listener& listener)
{
    ObjectPtr object(getGObject());
    if(object != nullptr)
    {
        listener.connectAllSignals(object);
    }
}

/*
 * The GCallback method called directly by LibNM code when sending state-changed 
 * signals.
 */
void LibNM::DeviceWifi::stateChangeCallback(
        NMDevice* device,
        NMDeviceState newState,
        NMDeviceState oldState,
        NMDeviceStateReason reason,
        Listener* listener) 
{ 
    ASSERT_NM_CONTEXT;
    listener->stateChanged(newState, oldState, reason);
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-added signals.
 */
void LibNM::DeviceWifi::apAddedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
        Listener* listener) 
{ 
    ASSERT_NM_CONTEXT;
    listener->accessPointAdded(AccessPoint(ap));
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-removed signals.
 */
void LibNM::DeviceWifi::apRemovedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
        Listener* listener) 
{ 
    ASSERT_NM_CONTEXT;
    listener->accessPointRemoved(AccessPoint(ap));
}
