#include "Utils.h"
#include "NMPPDeviceWifi.h"

/*
 * Creates a NMPPDeviceWifi object that shares a NMDeviceWifi*
 * with another NMPPDeviceWifi object
 */
NMPPDeviceWifi::NMPPDeviceWifi(const NMPPDeviceWifi& toCopy)
{ 
    setGObject(toCopy);
}

/*
 * Create a NMPPDeviceWifi to contain a NMDeviceWifi object.
 */
NMPPDeviceWifi::NMPPDeviceWifi(NMDeviceWifi* toAssign)
{
    setGObject(G_OBJECT(toAssign));
}

/*
 * Gets the current state of the wifi network device.
 */
NMDeviceState NMPPDeviceWifi::getState() const
{ 
    NMDeviceState state = NM_DEVICE_STATE_UNKNOWN;
    callInMainContext([&state](GObject* devObject)
    {
        NMDevice* device = NM_DEVICE(devObject);
        if(device != nullptr)
        {
            state = nm_device_get_state(device);
        }
    });
    return state;
}

/*
 * Gets the reason for the current device state.
 */
NMDeviceStateReason NMPPDeviceWifi::getStateReason() const
{ 
    NMDeviceStateReason reason = NM_DEVICE_STATE_REASON_UNKNOWN;
    callInMainContext([&reason](GObject* devObject)
    {
        NMDevice* device = NM_DEVICE(devObject);
        if(device != nullptr)
        {
            nm_device_get_state_reason(device, &reason);
        }
    });
    return reason;
}

/*
 * Checks if NetworkManager manages this device.
 */
bool NMPPDeviceWifi::isManaged() const
{ 
    bool managed = false;
    callInMainContext([&managed](GObject* devObject)
    {
        NMDevice* device = NM_DEVICE(devObject);
        if(device != nullptr)
        {
            managed = nm_device_get_managed(device);
        }
    });
    return managed;
}

/*
 * Gets the interface name of this wifi device.
 */
const char* NMPPDeviceWifi::getInterface() const
{ 
    const char* iface = "";
    callInMainContext([&iface](GObject* devObject)
    {
        NMDevice* device = NM_DEVICE(devObject);
        if(device != nullptr)
        {
            iface = nm_device_get_iface(device);
        }
    });
    return iface;
}

/*
 * Gets the DBus path of this wifi device.
 */
const char* NMPPDeviceWifi::getPath() const
{ 
    const char* path = "";
    callInMainContext([&path](GObject* devObject)
    {
        NMObject* device = NM_OBJECT(devObject);
        if(device != nullptr)
        {
            path = nm_object_get_path(device);
        }
    });
    return path;
}

/*
 * Disconnects any connection that is active on this wifi device.  If there
 * is no active connection, or the object is null, nothing will happen.
 */
void NMPPDeviceWifi::disconnect() 
{ 
    callInMainContext([](GObject* devObject)
    {
        NMDevice* device = NM_DEVICE(devObject);
        if(device != nullptr)
        {
            nm_device_disconnect(device, nullptr, nullptr);
        }
    });
}

/*
 * Gets the current active connection running on this device.
 */
NMPPActiveConnection NMPPDeviceWifi::getActiveConnection() const
{ 
    NMPPActiveConnection active;
    callInMainContext([&active](GObject* devObject)
    {
        NMDevice* device = NM_DEVICE(devObject);
        if(device != nullptr)
        {
            NMActiveConnection* con = nm_device_get_active_connection(device);
            if(con != nullptr)
            {
                active = con;
            }
        }
    });
    return active;
}
  
/*
 * Gets the list of connections available to activate on this device.
 * This might not load all saved connections.
 */
Array<NMPPConnection> NMPPDeviceWifi::getAvailableConnections() const
{ 
    Array<NMPPConnection> available;
    callInMainContext([&available](GObject* devObject)
    {
        NMDevice* device = NM_DEVICE(devObject);
        if(device != nullptr)
        {
            const GPtrArray* cons = nm_device_get_available_connections(device);
            for(int i = 0; cons && i < cons->len; i++)
            {
                NMConnection* connection = NM_CONNECTION(cons->pdata[i]);
                if(connection != nullptr)
                {
                    available.add(NMPPConnection(connection));
                }
            }
        }
    });
    return available;
}


    
/*
 * Finds the first available connection that is compatible with a specific
 * wifi access point.
 */
NMPPConnection NMPPDeviceWifi::getAvailableConnection
(const NMPPAccessPoint& accessPoint) const
{
    if(!accessPoint.isNull())
    {
        Array<NMPPConnection> available = getAvailableConnections();
        for(const NMPPConnection& con : available)
        {
            if(accessPoint.isValidConnection(con))
            {
                return con;
            }
        }
    }
    return NMPPConnection();
}

/*
 * Gets an access point object using the access point's path.
 */
NMPPAccessPoint NMPPDeviceWifi::getAccessPoint(const char* path) const
{ 
    NMPPAccessPoint ap;
    callInMainContext([&ap, path](GObject* devObject)
    {
        NMDeviceWifi* device = NM_DEVICE_WIFI(devObject);
        if(device != nullptr)
        {
            ap = nm_device_wifi_get_access_point_by_path(device, path);
        }
    });
    return ap;
}
  
/*
 * Gets the active connection's access point.
 */
NMPPAccessPoint NMPPDeviceWifi::getActiveAccessPoint() const
{ 
    NMPPAccessPoint ap;
    callInMainContext([&ap](GObject* devObject)
    {
        NMDeviceWifi* device = NM_DEVICE_WIFI(devObject);
        if(device != nullptr)
        {
            ap = nm_device_wifi_get_active_access_point(device);
        }
    });
    return ap;
}

/*
 * Gets all access points visible to this device.
 */
Array<NMPPAccessPoint> NMPPDeviceWifi::getAccessPoints() const
{ 
    Array<NMPPAccessPoint> accessPoints;
    callInMainContext([&accessPoints](GObject* devObject)
    {
        NMDeviceWifi* device = NM_DEVICE_WIFI(devObject);
        if(device != nullptr)
        {
            const GPtrArray* aps = nm_device_wifi_get_access_points(device);
            for(int i = 0; aps && i < aps->len; i++)
            {
                NMAccessPoint* nmAP = NM_ACCESS_POINT(aps->pdata[i]);
                if(nmAP != nullptr)
                {
                    //ADDR_LOG(nmAP,"Created as NMAccessPoint");
                    accessPoints.add(NMPPAccessPoint(nmAP));
                }
            }
        }
    });
    return accessPoints;
}
    
/*
 * Checks if a specific connection is present in the list of available
 * device connections.
 */
bool NMPPDeviceWifi::hasConnectionAvailable(const NMPPConnection& toFind) const
{
    if(toFind.isNull())
    {
        return false;
    }
    return getAvailableConnections().contains(toFind);
}

/*
 * Sends a request to the wifi device asking it to scan visible access 
 * points.
 */
void NMPPDeviceWifi::requestScan() 
{ 
    callInMainContext([](GObject* devObject)
    {
        NMDeviceWifi* device = NM_DEVICE_WIFI(devObject);
        if(device != nullptr)
        {
            nm_device_wifi_request_scan_simple(device, nullptr, nullptr);
        }
    });
}

/*
 * Convert generic property change notifications into 
 * activeConnectionChanged calls.
 * 
 * @param source    Holds the GObject that emitted the signal. This
 *                  will be a NMPPDeviceWifi object.
 * 
 * @param property  This should be the active connection property, 
 *                  "active-connection"
 */
void NMPPDeviceWifi::Listener::propertyChanged(GPPObject* source, 
        String property)
{
    jassert(property == "active-connection");
    NMPPDeviceWifi* dev = static_cast<NMPPDeviceWifi*>(source);
    activeConnectionChanged(dev->getActiveConnection());
}

/*
 * Adds a signal handler object to this wifi device.
 */
void NMPPDeviceWifi::addSignalHandler(SignalHandler* handler)
{ 
    if(isClass<SignalHandler,Listener>(handler))
    {
        connectSignalHandler(handler, "state-changed",
                G_CALLBACK(stateChangeCallback));
        connectSignalHandler(handler, "access-point-added", 
                G_CALLBACK(apAddedCallback));
        connectSignalHandler(handler, "access-point-removed",
                G_CALLBACK(apRemovedCallback));
    }
    else
    {
        DBG("NMPPDeviceWifi::" << __func__ << ": Invalid signal handler!");
    }
}


/*
 * The GCallback method called directly by LibNM code when sending 
 * state-changed signals.  This will use the signal data to call the 
 * listener object's stateChanged method.
 */
void NMPPDeviceWifi::stateChangeCallback(
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
 * listener object's accessPointAdded method
 */
void NMPPDeviceWifi::apAddedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
        Listener* listener) 
{ 
    listener->accessPointAdded(NMPPAccessPoint(ap));
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-removed signals.  This will use the signal data to call the
 * listener object's accessPointRemoved method
 */
void NMPPDeviceWifi::apRemovedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
        Listener* listener) 
{ 
    listener->accessPointRemoved(NMPPAccessPoint(ap));
}

/*
 * Gets the GType of this object's stored GObject class.
 */
GType NMPPDeviceWifi::getType() const 
{ 
    return NM_TYPE_DEVICE_WIFI;
}

/*
 * Checks if a GObject's type allows it to be held by this object. 
 */
bool NMPPDeviceWifi::isValidType(GObject* toCheck) const 
{ 
    return NM_IS_DEVICE_WIFI(toCheck);
}