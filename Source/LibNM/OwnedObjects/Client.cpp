// Required to implement the client's private ThreadHandler class. */
#define LIB_NM_THREAD_IMPLEMENTATION

#include "LibNM/OwnedObjects/Client.h"
#include "LibNM/OwnedObjects/Connection.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/BorrowedObjects/ActiveConnection.h"
#include "LibNM/ThreadResource.h"
#include "LibNM/ContextTest.h"
#include "GLib/SmartPointers/ObjectPtr.h"
#include "GLib/Thread/ThreadHandler.h"

/**
 * @brief  The private ThreadHandler used by the active wifi device to access
 *         the LibNM thread's Client data.
 */
class ClientThreadHandler : public GLib::ThreadHandler
{ 
public:
    ClientThreadHandler() : 
    GLib::ThreadHandler(LibNM::ThreadResource::resourceKey, 
        []()->GLib::ThreadResource* { return new LibNM::ThreadResource(); } ) 
    { }

    virtual ~ClientThreadHandler() { }
    
    /**
     * @brief  Gets the shared NetworkManager client object if called within
     *         the LibNM event loop.
     *
     * @return  The client object if called within the event loop, or a null
     *          Client if called outside of the event loop.
     */
    LibNM::Client getClient() const
    {
        SharedResource::LockedPtr<LibNM::ThreadResource> nmThread
            = getWriteLockedResource<LibNM::ThreadResource>();
        return nmThread->getClient();
    }
    
    /**
     * @brief  Gets the set of Wifi device objects managed by the networkClient.
     *
     * @return  The ObjectLender used to manage all NMDeviceWifi* data.
     */
    GLib::Borrowed::ObjectLender<LibNM::DeviceWifi>& getWifiDeviceLender() const
    {
        SharedResource::LockedPtr<LibNM::ThreadResource> nmThread
            = getWriteLockedResource<LibNM::ThreadResource>();
        return nmThread->getWifiDeviceLender();
    }

    /**
     * @brief  Gets the set of ActiveConnection objects managed by the 
     *         networkClient.
     *
     * @return  The ObjectLender used to manage the networkClient's
     *          NMActiveConnection* data.
     */
    GLib::Borrowed::ObjectLender<LibNM::ActiveConnection>& getConnectionLender()
        const
    {
        SharedResource::LockedPtr<LibNM::ThreadResource> nmThread
            = getWriteLockedResource<LibNM::ThreadResource>();
        return nmThread->getConnectionLender();
    }
};

/*
 * Creates a null Client object.
 */
LibNM::Client::Client() : GLib::Owned::Object(NM_TYPE_CLIENT) { }

/*
 * Creates a Client that shares a NMClient with another Client.
 */
LibNM::Client::Client(const Client& toCopy) : 
GLib::Owned::Object(toCopy, NM_TYPE_CLIENT)
{
    ASSERT_NM_CONTEXT;
    setGObject(toCopy);
}
 
/*
 * Creates a Client holding an existing NMClient object.
 */
LibNM::Client::Client(NMClient* toAssign) :
GLib::Owned::Object(G_OBJECT(toAssign), NM_TYPE_CLIENT) 
{ 
    ASSERT_NM_CONTEXT;
}

/*
 * Initializes a null Client object with new NMClient data.
 */
void LibNM::Client::initClient()
{
    ASSERT_NM_CONTEXT;
    GObject* newClient = G_OBJECT(nm_client_new());
    g_object_ref(newClient); // TEST: this shouldn't be necessary
    setGObject(newClient);
}

/**
 * @brief  Casts a GLib::ObjectPtr holding the Client data to the NMClient*
 *         type.
 *
 * @param clientPtr  A RAII pointer object storing the client's GObject* value.
 *
 * @return  The value stored in clientPtr as a NMClient*, or nullptr if 
 *          clientPtr does not hold a NMClient*
 */
static NMClient* toClientPtr(GLib::ObjectPtr& clientPtr)
{
    return NM_CLIENT((GObject*) clientPtr);
}

/*
 * Gets all wifi devices from the network manager.
 */
juce::Array<LibNM::DeviceWifi> LibNM::Client::getWifiDevices() const
{ 
    ASSERT_NM_CONTEXT;
    juce::Array<DeviceWifi> devices;
    GLib::ObjectPtr dataPtr(*this);
    if(dataPtr != nullptr)
    {
        const ClientThreadHandler threadHandler;
        GLib::Borrowed::ObjectLender<DeviceWifi>& deviceLender
            = threadHandler.getWifiDeviceLender();
        const GPtrArray* deviceArray 
            = nm_client_get_devices(toClientPtr(dataPtr));
        for(int i = 0; deviceArray && i < deviceArray->len; i++)
        {
            NMDevice* device = NM_DEVICE(deviceArray->pdata[i]);
            if(device != nullptr && NM_IS_DEVICE_WIFI(device))
            {
                devices.add(deviceLender.borrowObject(G_OBJECT(device)));
            }
        }  
    }
    return devices;
}

/**
 * @brief  Gets a DeviceWifi object holding a specific NMDeviceWifi*, adding
 *         the device to the tracked list of Wifi devices if necessary.
 *
 * @param devicePtr  The LibNM Wifi device object pointer the DeviceWifi should
 *                   hold.
 *
 * @return  A DeviceWifi that is holding devicePtr.
 */
static LibNM::DeviceWifi borrowDevice(NMDeviceWifi* devicePtr)
{
    using namespace LibNM;
    const ClientThreadHandler threadHandler;
    GLib::Borrowed::ObjectLender<DeviceWifi>& deviceLender
        = threadHandler.getWifiDeviceLender();
    return deviceLender.borrowObject(G_OBJECT(devicePtr));
}

/*
 * Gets a specific wifi device using its interface name.
 */
LibNM::DeviceWifi 
LibNM::Client::getWifiDeviceByIface(const char* interface) const
{ 
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if(dataPtr != nullptr)
    {
        NMDevice* device = nm_client_get_device_by_iface(toClientPtr(dataPtr),
                interface);
        if(device != nullptr && NM_IS_DEVICE_WIFI(device))
        {
            return borrowDevice(NM_DEVICE_WIFI(device));
        }
    }
    return DeviceWifi();
}

/*
 * Gets a specific wifi device using its DBus path.
 */
LibNM::DeviceWifi LibNM::Client::getWifiDeviceByPath(const char* path) const
{ 
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if(dataPtr != nullptr)
    {
        NMDevice* device = nm_client_get_device_by_path(toClientPtr(dataPtr),
                path);
        if(device != nullptr && NM_IS_DEVICE_WIFI(device))
        {
            return borrowDevice(NM_DEVICE_WIFI(device));
        }
    }
    return DeviceWifi();
}

/*
 * Gets the list of all active connections known to the network manager.
 */
juce::Array<LibNM::ActiveConnection> LibNM::Client::getActiveConnections() const
{ 
    ASSERT_NM_CONTEXT;
    juce::Array<ActiveConnection> connections;
    GLib::ObjectPtr dataPtr(*this);
    if(dataPtr != nullptr)
    {
        const ClientThreadHandler threadHandler;
        GLib::Borrowed::ObjectLender<ActiveConnection>& connectionLender
            = threadHandler.getConnectionLender();
        const GPtrArray* nmConnections 
                = nm_client_get_active_connections(toClientPtr(dataPtr));
        for(int i = 0; nmConnections && i < nmConnections->len; i++)
        {
            NMActiveConnection* connection 
                = NM_ACTIVE_CONNECTION(nmConnections->pdata[i]);
            if(connection != nullptr && NM_IS_ACTIVE_CONNECTION(connection))
            {
                connections.add(connectionLender.borrowObject
                        (G_OBJECT(connection)));
            }
        }
    }
    return connections;
}


/**
 * @brief  Gets a ActiveConnection object holding a specific 
 *         NMActiveConnection*, adding the connection to the tracked list of 
 *         active connections if necessary.
 *
 * @param connectionPtr  The LibNM active connection object pointer the 
 *                       ActiveConnection should hold.
 *
 * @return  An ActiveConnection that is holding connectionPtr.
 */
static LibNM::ActiveConnection borrowConnection
(NMActiveConnection* connectionPtr) 
{
    using namespace LibNM;
    const ClientThreadHandler threadHandler;
    GLib::Borrowed::ObjectLender<ActiveConnection>& connectionLender
        = threadHandler.getConnectionLender();
    return connectionLender.borrowObject(G_OBJECT(connectionPtr));
}

/*
 * Gets the primary active network connection.
 */
LibNM::ActiveConnection LibNM::Client::getPrimaryConnection() const
{ 
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if(dataPtr != nullptr)
    {
        NMActiveConnection* connection 
                = nm_client_get_primary_connection(toClientPtr(dataPtr));
        if(connection != nullptr && NM_IS_ACTIVE_CONNECTION(connection))
        {
            return borrowConnection(connection);
        }
    }
    return ActiveConnection();
}

/*
 * Gets the connection being activated by the network manager.
 */
LibNM::ActiveConnection LibNM::Client::getActivatingConnection() const
{ 
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if(dataPtr != nullptr)
    {
        NMActiveConnection* connection 
                = nm_client_get_activating_connection(toClientPtr(dataPtr));
        if(connection != nullptr && NM_IS_ACTIVE_CONNECTION(connection))
        {
            return borrowConnection(connection);
        }
    }
    return ActiveConnection();
}

/*
 * Deactivates an active network connection.
 */
void LibNM::Client::deactivateConnection(ActiveConnection& activeCon) const
{ 
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if(dataPtr != nullptr)
    {
        if(!activeCon.isNull())
        {
                nm_client_deactivate_connection(toClientPtr(dataPtr),
                        NM_ACTIVE_CONNECTION(activeCon.getGObject()));
                ClientThreadHandler threadHandler;
                threadHandler.getConnectionLender().invalidateObject
                        (activeCon.getGObject());
        }         
    }
}

/*
 * Checks if wireless connections are currently enabled.
 */
bool LibNM::Client::wirelessEnabled() const
{ 
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if(dataPtr != nullptr)
    {
        return nm_client_wireless_get_enabled(toClientPtr(dataPtr));
    }
    return false;
}

/*
 * Sets whether wireless connections are enabled.
 */
void LibNM::Client::setWirelessEnabled(bool enabled) const
{ 
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if(dataPtr != nullptr)
    {
        nm_client_wireless_set_enabled(toClientPtr(dataPtr), enabled);
    }
}

/*
 * The NMClientActivateFn called by LibNM when activating an existing
 * connection, used to create a call to openingConnection or to 
 * openingConnectionFailed.
 */
void LibNM::Client::ConnectionHandler::activateCallback(NMClient* client,
        NMActiveConnection* connection,
        GError* error,
        Client::ConnectionHandler* handler) 
{
    ASSERT_NM_CONTEXT;
    ActiveConnection activeConnection = borrowConnection(connection);
    if(error != nullptr)
    {
        handler->openingConnectionFailed(activeConnection, error);
    }
    else
    {
        handler->openingConnection(activeConnection);
    } 
}

/*
 * The NMClientAddActivateFn called by LibNM when adding and activating
 * a new connection, used to create a call to openingConnection or 
 * openingConnectionFailed.
 */
void LibNM::Client::ConnectionHandler::addActivateCallback(NMClient* client,
        NMConnection* connection,
        const char* path,
        GError* error,
        Client::ConnectionHandler* handler) 
{
    ActiveConnection activeConnection 
        = borrowConnection(NM_ACTIVE_CONNECTION(connection));
    jassert((activeConnection.isNull()) == (connection == nullptr));
    if(error != nullptr)
    {
        handler->openingConnectionFailed(activeConnection, error);
    }
    else
    {
        handler->openingConnection(activeConnection);
    } 
}

/*
 * Activates a wifi network connection, attempting to set it as the primary
 * network connection.
 */ 
void LibNM::Client::activateConnection(
        const Connection& connection,
        const DeviceWifi& wifiDevice,
        const AccessPoint& accessPoint, 
        Client::ConnectionHandler* handler,
        bool usedSavedConnection)
{ 
    ASSERT_NM_CONTEXT;
    //determine if this is a new connection attempt
    bool isNew = !usedSavedConnection;
    GLib::ObjectPtr clientDataPtr(*this);
    if(clientDataPtr != nullptr)
    {
        const char* apPath = accessPoint.getPath();
        if(apPath != nullptr && !wifiDevice.isNull())
        {
            GLib::ObjectPtr connectionDataPtr(connection);
            if(isNew)
            {
                DBG("Client::activateConnection"
                        << ": adding new connection.");
                // using a null connection object is allowed and may work.
                nm_client_add_and_activate_connection(
                        toClientPtr(clientDataPtr),
                        NM_CONNECTION((GObject*) connectionDataPtr),
                        NM_DEVICE(wifiDevice.getGObject()),
                        apPath,
                        (NMClientAddActivateFn) 
                        ConnectionHandler::addActivateCallback,
                        handler);   
            }
            else
            {
                DBG("Client::activateConnection"
                        << ": activating saved connection.");
                nm_client_activate_connection(
                        toClientPtr(clientDataPtr),
                        NM_CONNECTION((GObject*) connectionDataPtr),
                        NM_DEVICE(wifiDevice.getGObject()),
                        apPath,
                        (NMClientActivateFn) 
                        ConnectionHandler::activateCallback,
                        handler);  
            }
        }
    }
}

/*
 * Subscribes to all relevant signals from a single GObject signal source.
 */
void LibNM::Client::Listener::connectAllSignals(GObject* source)
{
    ASSERT_NM_CONTEXT;
    if(source != nullptr && NM_IS_CLIENT(source))
    {
        connectNotifySignal(source, NM_CLIENT_WIRELESS_ENABLED, true);
    }
}

/*
 * Converts generic propertyChanged calls to class-specific 
 * wirelessStateChange calls.
 */
void LibNM::Client::Listener::propertyChanged
(GObject* source, juce::String property)
{ 
    ASSERT_NM_CONTEXT;
    if(source != nullptr && NM_IS_CLIENT(source) 
            && property == NM_CLIENT_WIRELESS_ENABLED)
    {
        g_object_ref(source);
        Client client(NM_CLIENT(source));
        bool enabled = client.wirelessEnabled();
        wirelessStateChange(enabled);
    }
}
    
/*
 * Adds a listener to this network manager client.
 */
void LibNM::Client::addListener(Listener& listener)
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr source(*this);
    if(source != nullptr)
    {
        listener.connectAllSignals(source);
    }
}
