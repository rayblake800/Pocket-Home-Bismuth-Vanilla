#include "LibNM/DeviceWifi.h"
#include "Utils.h"
#include "GLib/SmartPointers/ObjectPtr.h"

/* Rename smart pointers for brevity: */
typedef GLib::ObjectPtr<NMObject*> NMObjectPtr;
typedef GLib::ObjectPtr<NMDevice*> NMDevicePtr;
typedef GLib::ObjectPtr<NMDeviceWifi*> NMDeviceWifiPtr;
typedef GLib::ObjectPtr<> ObjectPtr;


/*
 * Creates a DeviceWifi object that shares a NMDeviceWifi* with another 
 * DeviceWifi object
 */
LibNM::DeviceWifi::DeviceWifi(const DeviceWifi& toCopy) :
GLib::Object(toCopy, NM_TYPE_DEVICE_WIFI) { }

/*
 * Create a DeviceWifi to contain a NMDeviceWifi object.
 */
LibNM::DeviceWifi::DeviceWifi(NMDeviceWifi* toAssign) :
GLib::Object(G_OBJECT(toAssign), NM_TYPE_DEVICE_WIFI) { }

    
/*
 * Creates a null DeviceWifi.
 */
LibNM::DeviceWifi::DeviceWifi() :
GLib::Object(NM_TYPE_DEVICE_WIFI) { }

/*
 * Gets the current state of the wifi network device.
 */
NMDeviceState DeviceWifi::getState() const
{ 
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
NMDeviceStateReason DeviceWifi::getStateReason() const
{ 
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
 * Gets the DBus path of this wifi device.
 */
const char* LibNM::DeviceWifi::getPath() const
{ 
    const char* path = "";
    NMObjectPtr device(NM_OBJECT(getGObject()));
    if(device != nullptr)
    {
        path = nm_object_get_path(device);
        if(path == nullptr)
        {
            path = "";
        }
    }
    return path;
}

/*
 * Disconnects any connection that is active on this wifi device.  If there
 * is no active connection, or the object is null, nothing will happen.
 */
void LibNM::DeviceWifi::disconnect() 
{ 
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
 * This might not load all saved connections.
 */
juce::Array<LibNM::Connection> 
LibNM::DeviceWifi::getAvailableConnections() const
{ 
    juce::Array<Connection> available;
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
                available.add(Connection(connection));
            }
        }
    }
    return available;
}


    
/*
 * Finds the first available connection that is compatible with a specific wifi 
 * access point.
 */
LibNM::Connection LibNM::DeviceWifi::getAvailableConnection
(const AccessPoint& accessPoint) const
{
    if(!accessPoint.isNull())
    {
        juce::Array<Connection> available = getAvailableConnections();
        for(const Connection& con : available)
        {
            if(accessPoint.isValidConnection(con))
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
    NMDeviceWifiPtr device(NM_DEVICE_WIFI(getGObject()));
    if(device != nullptr)
    {
        NMAccessPoint* nmAP = nm_device_wifi_get_access_point_by_path
                (device, path);
        if(nmAP != nullptr && NM_IS_ACCESS_POINT(nmAP))
        {
            g_object_ref(G_OBJECT(nmAP));
            return AccessPoint(nmAP);
        }
    }
    return AccessPoint();
}
  
/*
 * Gets the active connection's access point.
 */
LibNM::AccessPoint LibNM::DeviceWifi::getActiveAccessPoint() const
{ 
    NMDeviceWifiPtr device(NM_DEVICE_WIFI(getGObject()));
    if(device != nullptr)
    {
        NMAccessPoint* nmAP = nm_device_wifi_get_active_access_point
                (device);
        if(nmAP != nullptr && NM_IS_ACCESS_POINT(nmAP))
        {
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
    juce::Array<AccessPoint> accessPoints;
    NMDeviceWifiPtr device(NM_DEVICE_WIFI(getGObject()));
    if(device != nullptr)
    {
        const GPtrArray* aps = nm_device_wifi_get_access_points(device);
        for(int i = 0; aps && i < aps->len; i++)
        {
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
    NMDeviceWifiPtr device(NM_DEVICE_WIFI(getGObject()));
    if(device != nullptr)
    {
        nm_device_wifi_request_scan_simple(device, nullptr, nullptr);
    }
}

/*
 * Subscribe to all relevant signals from a single GObject signal source.
 * 
 */
void LibNM::DeviceWifi::Listener::connectAllSignals(GObject* source) 
{
    if(source != nullptr && NM_IS_DEVICE_WIFI(source))
    {
        connectSignal(source, "state-changed",
                G_CALLBACK(stateChangeCallback));
        connectSignal(source, "access-point-added", 
                G_CALLBACK(apAddedCallback));
        connectSignal(source, "access-point-removed",
                G_CALLBACK(apRemovedCallback));
        connectNotifySignal(source, "active-connection");
    }
}
 
/*
 * Convert generic property change notifications into 
 * activeConnectionChanged calls.
 */
void LibNM::DeviceWifi::Listener::propertyChanged(GObject* source,
        juce::String property)
{
    jassert(property == "active-connection");
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
 * signals.  This will use the signal data to call the listener object's 
 * stateChanged method.
 */
void LibNM::DeviceWifi::stateChangeCallback(
        NMDevice* device,
        NMDeviceState newState,
        NMDeviceState oldState,
        NMDeviceStateReason reason,
        Listener* listener) 
{ 
    listener->stateChanged(newState, oldState, reason);
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-added signals.  This will use the signal data to call the
 * listener object's accessPointAdded method.
 */
void LibNM::DeviceWifi::apAddedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
        Listener* listener) 
{ 
    listener->accessPointAdded(AccessPoint(ap));
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-removed signals.  This will use the signal data to call the
 * listener object's accessPointRemoved method.
 */
void LibNM::DeviceWifi::apRemovedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
        Listener* listener) 
{ 
    listener->accessPointRemoved(AccessPoint(ap));
}
