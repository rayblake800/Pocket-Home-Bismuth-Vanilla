// Required to implement the Wifi device's private ThreadHandler class. */
#define LIB_NM_THREAD_IMPLEMENTATION

#include "LibNM/BorrowedObjects/DeviceWifi.h"
#include "LibNM/OwnedObjects/Connection.h"
#include "LibNM/ThreadResource.h"
#include "LibNM/ContextTest.h"
#include "GLib/SignalHandler.h"
#include "GLib/Thread/ThreadHandler.h"

/* Object property keys: */
static const constexpr char * activeConnectionProperty = "active-connection";

/* Object signal keys: */
static const constexpr char * stateChangeSignal = "state-changed";
static const constexpr char * addedAPSignal     = "access-point-added";
static const constexpr char * removedAPSignal   = "access-point-removed";

/**
 * @brief  The private ThreadHandler used by the active wifi device to access
 *         the LibNM thread's Wifi device data.
 */
class DeviceThreadHandler : public GLib::ThreadHandler
{ 
public:
    DeviceThreadHandler() : 
    GLib::ThreadHandler(LibNM::ThreadResource::resourceKey, 
        []()->GLib::ThreadResource* { return new LibNM::ThreadResource(); } ) 
    { }

    virtual ~DeviceThreadHandler() { }
    
    /**
     * @brief  Gets the shared DeviceWifi object used to control the
     *         LibNM-managed Wifi device if called within the LibNM event loop.
     *
     * @return  The wifi device object if called within the event loop, or a
     *          null DeviceWifi if called outside of the event loop, or if no
     *          managed wifi device was found.
     */
    LibNM::DeviceWifi getWifiDevice() const
    {
        ASSERT_NM_CONTEXT;
        SharedResource::LockedPtr<LibNM::ThreadResource> nmThread
            = getWriteLockedResource<LibNM::ThreadResource>();
        return nmThread->getWifiDevice();
    }

    /**
     * @brief  Gets the set of AccessPoint objects managed by the active Wifi
     *         device.
     *
     * @return  The ObjectLender used to manage all NMAccessPoint* data.
     */
    GLib::Borrowed::ObjectLender<LibNM::AccessPoint>& getAccessPointLender() 
            const
    {
        ASSERT_NM_CONTEXT;
        SharedResource::LockedPtr<LibNM::ThreadResource> nmThread
            = getWriteLockedResource<LibNM::ThreadResource>();
        return nmThread->getAccessPointLender();
    }

    /**
     * @brief  Gets the set of ActiveConnection objects managed by the active 
     *         Wifi device.
     *
     * @return  The ObjectLender used to manage the Wifi device's 
     *          NMActiveConnection* data.
     */
    GLib::Borrowed::ObjectLender<LibNM::ActiveConnection>& 
            getConnectionLender() const
    {
        ASSERT_NM_CONTEXT;
        SharedResource::LockedPtr<LibNM::ThreadResource> nmThread
            = getWriteLockedResource<LibNM::ThreadResource>();
        return nmThread->getConnectionLender();
    }
};

/*
 * Creates a null DeviceWifi.
 */
LibNM::DeviceWifi::DeviceWifi() : 
GLib::Borrowed::Object(NM_TYPE_ACCESS_POINT) { }

/*
 * Gets the current state of the wifi network device.
 */
NMDeviceState LibNM::DeviceWifi::getState() const
{ 
    ASSERT_NM_CONTEXT;
    NMDeviceState state = NM_DEVICE_STATE_UNKNOWN;
    if(!isNull())
    {
        state = nm_device_get_state(getDevicePtr());
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
    if(!isNull())
    {
        nm_device_get_state_reason(getDevicePtr(), &reason);
    }
    return reason;
}

/*
 * Checks if NetworkManager manages this device.
 */
bool LibNM::DeviceWifi::isManaged() const
{ 
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        return nm_device_get_managed(getDevicePtr());
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
    if(!isNull())
    {
        iface = nm_device_get_iface(getDevicePtr());
        if(iface == nullptr)
        {
            iface = "";
        }
    }
    return iface;
}

/*
 * Gets the DBus path to this wifi device's remote object.
 */
const char* LibNM::DeviceWifi::getPath() const
{
    return nm_object_get_path(NM_OBJECT(getGObject()));
}

/*
 * Disconnects any connection that is active on this wifi device.
 */
void LibNM::DeviceWifi::disconnect() const
{ 
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        nm_device_disconnect(getDevicePtr(), nullptr, nullptr);
    }
}

/*
 * Gets the current active connection running on this device.
 */
LibNM::ActiveConnection LibNM::DeviceWifi::getActiveConnection() const
{ 
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        NMActiveConnection* nmConnection 
            = nm_device_get_active_connection(getDevicePtr());
        if(nmConnection != nullptr)
        {
            const DeviceThreadHandler nmThreadHandler;
            return nmThreadHandler.getConnectionLender().borrowObject
                (G_OBJECT(nmConnection)); 
        }
    }
    return ActiveConnection();
}

/*
 * Gets an access point object using the access point's path.
 */
LibNM::AccessPoint LibNM::DeviceWifi::getAccessPoint(const char* path) const
{ 
    ASSERT_NM_CONTEXT;
    if(!isNull() && path != nullptr)
    {
        NMAccessPoint* nmAP = nm_device_wifi_get_access_point_by_path
                (NM_DEVICE_WIFI(getGObject()), path);
        if(nmAP != nullptr && NM_IS_ACCESS_POINT(nmAP))
        {
            const DeviceThreadHandler nmThreadHandler;
            return nmThreadHandler.getAccessPointLender().borrowObject
                (G_OBJECT(nmAP));
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
    if(!isNull())
    {
        NMAccessPoint* nmAP = nm_device_wifi_get_active_access_point(
                NM_DEVICE_WIFI(getGObject()));
        if(nmAP != nullptr && NM_IS_ACCESS_POINT(nmAP))
        {
            const DeviceThreadHandler nmThreadHandler;
            return nmThreadHandler.getAccessPointLender().borrowObject
                (G_OBJECT(nmAP));
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
    const DeviceThreadHandler nmThreadHandler;
    juce::Array<AccessPoint> currentAccessPoints;
    juce::Array<AccessPoint> removedAccessPoints 
        = nmThreadHandler.getAccessPointLender().getAllBorrowed();

    if(!isNull())
    {
        const GPtrArray* aps = nm_device_wifi_get_access_points(
                getWifiDevicePtr());
        for(int i = 0; aps && i < aps->len; i++)
        {
            GObject* nmAccessPoint = G_OBJECT(aps->pdata[i]);
            jassert(nmAccessPoint != nullptr);
            AccessPoint foundAccessPoint = nmThreadHandler
                .getAccessPointLender().borrowObject(nmAccessPoint);
            if(!foundAccessPoint.isNull())
            {
                currentAccessPoints.add(foundAccessPoint);
                removedAccessPoints.removeAllInstancesOf(foundAccessPoint);
            }
        }
    }
    
    // Invalidate any objects in the AccessPointSet not found in the updated
    // list.
    for(AccessPoint& invalidAP : removedAccessPoints)
    {
        nmThreadHandler.getAccessPointLender().invalidateObject
            (invalidAP.getGObject());
    }
    return currentAccessPoints;
}

/*
 * Sends a request to the wifi device asking it to scan visible access points.
 */
void LibNM::DeviceWifi::requestScan() const 
{ 
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        nm_device_wifi_request_scan_simple(getWifiDevicePtr(), nullptr,
                nullptr);
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
                G_CALLBACK(stateChangeCallback), false);
        connectSignal(source, addedAPSignal, 
                G_CALLBACK(apAddedCallback), false);
        connectSignal(source, removedAPSignal,
                G_CALLBACK(apRemovedCallback), false);
        connectNotifySignal(source, activeConnectionProperty, false);
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
        const DeviceThreadHandler nmThreadHandler;
        DeviceWifi device = nmThreadHandler.getWifiDevice();
        activeConnectionChanged(device.getActiveConnection());
    }
}

/*
 * Adds a listener to this wifi device.
 */
void LibNM::DeviceWifi::addListener(DeviceWifi::Listener& listener)
{
    if(!isNull())
    {
        listener.connectAllSignals(getGObject());
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
    const DeviceThreadHandler nmThreadHandler;
    listener->accessPointAdded(nmThreadHandler.getAccessPointLender()
            .borrowObject(G_OBJECT(ap)));
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-removed signals.
 */
void LibNM::DeviceWifi::apRemovedCallback
(NMDeviceWifi* device, NMAccessPoint* ap, Listener* listener) 
{ 
    ASSERT_NM_CONTEXT;
    const DeviceThreadHandler nmThreadHandler;
    nmThreadHandler.getAccessPointLender().invalidateObject(G_OBJECT(ap));
    listener->accessPointRemoved();
}

/*
 * Gets this object's GLib object pointer cast as NMDeviceWifi* data.
 */
NMDeviceWifi* LibNM::DeviceWifi::getWifiDevicePtr() const
{
    return NM_DEVICE_WIFI(getGObject());
}

/*
 * Gets this object's GLib object pointer cast as NMDevice* data.
 */
NMDevice* LibNM::DeviceWifi::getDevicePtr() const
{
    return NM_DEVICE(getGObject());
}
