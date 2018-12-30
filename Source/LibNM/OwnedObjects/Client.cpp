#include "LibNM/OwnedObjects/Client.h"
#include "LibNM/OwnedObjects/Connection.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/BorrowedObjects/ActiveConnection.h"
#include "LibNM/ThreadHandler.h"
#include "LibNM/ContextTest.h"
#include "GLib/SmartPointers/ObjectPtr.h"

/* Rename smart pointers for brevity: */
typedef GLib::ObjectPtr<NMClient*> NMClientPtr;
typedef GLib::ObjectPtr<NMConnection*> NMConnectionPtr;
typedef GLib::ObjectPtr<> ObjectPtr;

/*
 * Creates a null Client object.
 */
LibNM::Client::Client() : LibNM::OwnedObject(NM_TYPE_CLIENT) { }

/*
 * Creates a Client that shares a NMClient with another Client.
 */
LibNM::Client::Client(const Client& toCopy) : 
LibNM::OwnedObject(toCopy, NM_TYPE_CLIENT)
{
    ASSERT_NM_CONTEXT;
}
 
/*
 * Creates a Client holding an existing NMClient object.
 */
LibNM::Client::Client(NMClient* toAssign) :
LibNM::OwnedObject(NM_OBJECT(toAssign), NM_TYPE_CLIENT) 
{ 
    ASSERT_NM_CONTEXT;
}

/*
 * Initializes a null Client object with new NMClient data.
 */
void LibNM::Client::initClient()
{
    ASSERT_NM_CONTEXT;
    setGObject(G_OBJECT(nm_client_new()));
}

/*
 * Gets all wifi devices from the network manager.
 */
juce::Array<LibNM::DeviceWifi> LibNM::Client::getWifiDevices()
{ 
    ASSERT_NM_CONTEXT;
    juce::Array<DeviceWifi> devices;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        const GPtrArray* devArray = nm_client_get_devices(client);
        for(int i = 0; devArray && i < devArray->len; i++)
        {
            NMDevice* dev = NM_DEVICE(devArray->pdata[i]);
            if(dev != nullptr && NM_IS_DEVICE_WIFI(dev))
            {
                BorrowedObject<NMDeviceWifi> deviceHolder
                    = wifiDevices.getBorrowedObject(NM_DEVICE_WIFI(dev));
                devices.add(DeviceWifi(deviceHolder));
            }
        }  
    }
    return devices;
}

/*
 * Gets a specific wifi device using its interface name.
 */
LibNM::DeviceWifi 
LibNM::Client::getWifiDeviceByIface(const char* interface)
{ 
    ASSERT_NM_CONTEXT;
    DeviceWifi wifiDevice;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        NMDevice* dev = nm_client_get_device_by_iface(client, interface);
        if(dev != nullptr && NM_IS_DEVICE_WIFI(dev))
        {
            BorrowedObject<NMDeviceWifi> deviceHolder
                = wifiDevices.getBorrowedObject(NM_DEVICE_WIFI(dev));
            wifiDevice = DeviceWifi(deviceHolder);
        }
    }
    return wifiDevice;
}

/*
 * Gets a specific wifi device using its DBus path.
 */
LibNM::DeviceWifi LibNM::Client::getWifiDeviceByPath(const char* path)
{ 
    ASSERT_NM_CONTEXT;
    DeviceWifi wifiDevice;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        NMDevice* dev = nm_client_get_device_by_path(client, path);
        if(dev != nullptr && NM_IS_DEVICE_WIFI(dev))
        {
            BorrowedObject<NMDeviceWifi> deviceHolder
                = wifiDevices.getBorrowedObject(NM_DEVICE_WIFI(dev));
            wifiDevice = DeviceWifi(deviceHolder);
        }
    }
    return wifiDevice;
}

/*
 * Gets the list of all active connections known to the network manager.
 */
juce::Array<LibNM::ActiveConnection> LibNM::Client::getActiveConnections()
{ 
    ASSERT_NM_CONTEXT;
    juce::Array<ActiveConnection> connections;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        const GPtrArray* cons = nm_client_get_active_connections(client);
        for(int i = 0; cons && i < cons->len; i++)
        {
            NMActiveConnection* con = NM_ACTIVE_CONNECTION(cons->pdata[i]);
            if(con != nullptr && NM_IS_ACTIVE_CONNECTION(con))
            {
                BorrowedObject<NMActiveConnection> connectionHolder
                    = activeConnections.getBorrowedObject(con);
                connections.add(ActiveConnection(connectionHolder));
            }
        }
    }
    return connections;
}

/*
 * Gets the primary active network connection.
 */
LibNM::ActiveConnection LibNM::Client::getPrimaryConnection()
{ 
    ASSERT_NM_CONTEXT;
    ActiveConnection primary;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        NMActiveConnection* con = nm_client_get_primary_connection(client);
        if(con != nullptr && NM_IS_ACTIVE_CONNECTION(con))
        {
            BorrowedObject<NMActiveConnection> connectionHolder
                = activeConnections.getBorrowedObject(con);
            primary = ActiveConnection(connectionHolder);
        }
    }
    return primary;
}

/*
 * Gets the connection being activated by the network manager.
 */
LibNM::ActiveConnection LibNM::Client::getActivatingConnection()
{ 
    ASSERT_NM_CONTEXT;
    ActiveConnection activating;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        NMActiveConnection* con = nm_client_get_activating_connection(
                client);
        if(con != nullptr)
        {
            BorrowedObject<NMActiveConnection> connectionHolder
                = activeConnections.getBorrowedObject(con);
            activating = ActiveConnection(connectionHolder);
        }
    }
    return activating;
}

/*
 * Deactivates an active network connection.
 */
void LibNM::Client::deactivateConnection(ActiveConnection& activeCon) 
{ 
    ASSERT_NM_CONTEXT;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        if(!activeCon.isNull())
        {
                nm_client_deactivate_connection(client, activeCon.getNMData());
        }         
    }
}

/*
 * Checks if wireless connections are currently enabled.
 */
bool LibNM::Client::wirelessEnabled() const
{ 
    ASSERT_NM_CONTEXT;
    bool enabled = false;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        return nm_client_wireless_get_enabled(client);
    }
    return false;
}

/*
 * Sets whether wireless connections are enabled.
 */
void LibNM::Client::setWirelessEnabled(bool enabled) 
{ 
    ASSERT_NM_CONTEXT;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        nm_client_wireless_set_enabled(client, enabled);
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
    ThreadHandler nmThreadHandler;
    Client networkClient = nmThreadHandler.getClient();
    ActiveConnection activeConnnection(networkClient.activeConnections
            .getBorrowedObject(connection));
    if(error != nullptr)
    {
        handler->openingConnectionFailed(activeConnnection, error);
    }
    else
    {
        handler->openingConnection(activeConnnection);
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
    ASSERT_NM_CONTEXT;
    ThreadHandler nmThreadHandler;
    Client networkClient = nmThreadHandler.getClient();
    ActiveConnection activeConnection = networkClient.getPrimaryConnection();
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
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        const char* apPath = accessPoint.getPath();
        NMConnectionPtr connectionObject(NM_CONNECTION(
                    getOtherGObject(connection)));
       
        //connecting with a null connection object is allowed and may work.
        if(apPath != nullptr && !wifiDevice.isNull())
        {
            if(isNew)
            {
                DBG("Client::activateConnection"
                        << ": adding new connection.");
                nm_client_add_and_activate_connection(client,
                    connectionObject,
                    NM_DEVICE(wifiDevice.getNMData()),
                    apPath,
                    (NMClientAddActivateFn) 
                    ConnectionHandler::addActivateCallback,
                    handler);   
            }
            else
            {
                DBG("Client::activateConnection"
                        << ": activating saved connection.");
                nm_client_activate_connection(client,
                        connectionObject,
                        NM_DEVICE(wifiDevice.getNMData()),
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
        connectNotifySignal(source, NM_CLIENT_WIRELESS_ENABLED);
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
    ObjectPtr source(getGObject());
    if(source != nullptr)
    {
        listener.connectAllSignals(source);
    }
}
