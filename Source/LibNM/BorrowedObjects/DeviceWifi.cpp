#include "LibNM/BorrowedObjects/DeviceWifi.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/BorrowedObjects/ActiveConnection.h"
#include "LibNM/OwnedObjects/Connection.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/ContextTest.h"

/* Object property keys: */
static const constexpr char * activeConnectionProperty = "active-connection";

/* Object signal keys: */
static const constexpr char * stateChangeSignal = "state-changed";
static const constexpr char * addedAPSignal     = "access-point-added";
static const constexpr char * removedAPSignal   = "access-point-removed";

/*
 * Create a DeviceWifi to contain a NMDeviceWifi object.
 */
LibNM::DeviceWifi::DeviceWifi(BorrowedObject<NMDeviceWifi> toAssign) :
BorrowedObjectInterface<NMDeviceWifi>(toAssign)
{ 
    ASSERT_NM_CONTEXT;
}

/*
 * Creates a null DeviceWifi.
 */
LibNM::DeviceWifi::DeviceWifi() { }

/*
 * Gets the current state of the wifi network device.
 */
NMDeviceState LibNM::DeviceWifi::getState() const
{ 
    ASSERT_NM_CONTEXT;
    NMDeviceState state = NM_DEVICE_STATE_UNKNOWN;
    if(!isNull())
    {
        state = nm_device_get_state(NM_DEVICE(getNMData()));
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
        nm_device_get_state_reason(NM_DEVICE(getNMData()), &reason);
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
        return nm_device_get_managed(NM_DEVICE(getNMData()));
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
        iface = nm_device_get_iface(NM_DEVICE(getNMData()));
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
    if(!isNull())
    {
        nm_device_disconnect(NM_DEVICE(getNMData()), nullptr, nullptr);
    }
}

/*
 * Gets the current active connection running on this device.
 */
LibNM::ActiveConnection LibNM::DeviceWifi::getActiveConnection()
{ 
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        NMActiveConnection* con 
            = nm_device_get_active_connection(NM_DEVICE(getNMData()));
        if(con != nullptr)
        {
            return ActiveConnection(activeConnections.getBorrowedObject(con));
        }
    }
    return ActiveConnection();
}

/*
 * Gets an access point object using the access point's path.
 */
LibNM::AccessPoint LibNM::DeviceWifi::getAccessPoint(const char* path)
{ 
    ASSERT_NM_CONTEXT;
    if(!isNull() && path != nullptr)
    {
        NMAccessPoint* nmAP = nm_device_wifi_get_access_point_by_path
                (getNMData(), path);
        if(nmAP != nullptr && NM_IS_ACCESS_POINT(nmAP))
        {
            return AccessPoint(accessPointSet.getBorrowedObject(nmAP));
        }
    }
    return AccessPoint();
}
  
/*
 * Gets the active connection's access point.
 */
LibNM::AccessPoint LibNM::DeviceWifi::getActiveAccessPoint()
{ 
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        NMAccessPoint* nmAP 
            = nm_device_wifi_get_active_access_point(getNMData());
        if(nmAP != nullptr && NM_IS_ACCESS_POINT(nmAP))
        {
            g_object_ref(G_OBJECT(nmAP));
            return AccessPoint(accessPointSet.getBorrowedObject(nmAP));
        }
    }
    return AccessPoint();
}

/*
 * Gets all access points visible to this device.
 */
juce::Array<LibNM::AccessPoint> LibNM::DeviceWifi::getAccessPoints()
{ 
    ASSERT_NM_CONTEXT;
    juce::Array<AccessPoint> accessPoints;
    if(!isNull())
    {
        const GPtrArray* aps = nm_device_wifi_get_access_points(getNMData());
        for(int i = 0; aps && i < aps->len; i++)
        {
            jassert(NM_IS_ACCESS_POINT(aps->pdata[i]));
            NMAccessPoint* nmAP = NM_ACCESS_POINT(aps->pdata[i]);
            if(nmAP != nullptr)
            {
                g_object_ref(G_OBJECT(nmAP));
                accessPoints.add(AccessPoint(accessPointSet
                            .getBorrowedObject(nmAP)));
            }
        }
    }
    return accessPoints;
}

/*
 * Sends a request to the wifi device asking it to scan visible access points.
 */
void LibNM::DeviceWifi::requestScan() 
{ 
    ASSERT_NM_CONTEXT;
    if(!isNull())
    {
        nm_device_wifi_request_scan_simple(getNMData(), nullptr, nullptr);
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
        ThreadHandler nmThreadHandler;
        DeviceWifi device(nmThreadHandler.getWifiDevice());
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
        listener.connectAllSignals(G_OBJECT(getNMData()));
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
    ThreadHandler nmThreadHandler;
    DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
    AccessPoint addedAP(wifiDevice.accessPointSet.getBorrowedObject(ap));
    listener->accessPointAdded(addedAP);
}

/*
 * The GCallback method called directly by LibNM code when sending 
 * access-point-removed signals.
 */
void LibNM::DeviceWifi::apRemovedCallback
(NMDeviceWifi* device, NMAccessPoint* ap, Listener* listener) 
{ 
    ASSERT_NM_CONTEXT;
    ThreadHandler nmThreadHandler;
    DeviceWifi wifiDevice = nmThreadHandler.getWifiDevice();
    wifiDevice.accessPointSet.removeBorrowedObject(ap);
    listener->accessPointRemoved();
}
