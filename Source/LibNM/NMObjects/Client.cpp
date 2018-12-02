#include "LibNM/NMObjects/Client.h"
#include "GLib/SmartPointers/ObjectPtr.h"

/* Rename smart pointers for brevity: */
typedef GLib::ObjectPtr<NMClient*> NMClientPtr;
typedef GLib::ObjectPtr<NMConnection*> NMConnectionPtr;
typedef GLib::ObjectPtr<NMDevice*> NMDevicePtr;
typedef GLib::ObjectPtr<> ObjectPtr;

/*
 * Creates a Client holding a new NMClient object.
 */
LibNM::Client::Client() : LibNM::Object(NM_TYPE_CLIENT)
{ 
    ASSERT_CORRECT_CONTEXT;
    setGObject(G_OBJECT(nm_client_new()));
}

/*
 * Creates a Client that shares a NMClient with another Client.
 */
LibNM::Client::Client(const Client& toCopy) : 
LibNM::Object(toCopy, NM_TYPE_CLIENT)
{
    ASSERT_CORRECT_CONTEXT;
}
 
/*
 * Creates a Client holding an existing NMClient object.
 */
LibNM::Client::Client(NMClient* toAssign) :
LibNM::Object(NM_OBJECT(toAssign), NM_TYPE_CLIENT) 
{ 
    ASSERT_CORRECT_CONTEXT;
}

/*
 * Gets all wifi devices from the network manager.
 */
juce::Array<LibNM::DeviceWifi> LibNM::Client::getWifiDevices() const
{ 
    ASSERT_CORRECT_CONTEXT;
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
                g_object_ref(G_OBJECT(dev));
                devices.add(DeviceWifi(NM_DEVICE_WIFI(dev)));
            }
        }  
    }
    return devices;
}

/*
 * Gets a specific wifi device using its interface name.
 */
LibNM::DeviceWifi 
LibNM::Client::getWifiDeviceByIface(const char* interface) const
{ 
    ASSERT_CORRECT_CONTEXT;
    DeviceWifi wifiDevice;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        NMDevice* dev = nm_client_get_device_by_iface(client, interface);
        if(dev != nullptr && NM_IS_DEVICE_WIFI(dev))
        {
            g_object_ref(G_OBJECT(dev));
            wifiDevice = NM_DEVICE_WIFI(dev);
        }
    }
    return wifiDevice;
}

/*
 * Gets a specific wifi device using its DBus path.
 */
LibNM::DeviceWifi LibNM::Client::getWifiDeviceByPath(const char* path) const
{ 
    ASSERT_CORRECT_CONTEXT;
    DeviceWifi wifiDevice;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        NMDevice* dev = nm_client_get_device_by_path(client, path);
        if(dev != nullptr && NM_IS_DEVICE_WIFI(dev))
        {
            g_object_ref(G_OBJECT(dev));
            wifiDevice = NM_DEVICE_WIFI(dev);
        }
    }
    return wifiDevice;
}

/*
 * Gets the list of all active connections known to the network manager.
 */
juce::Array<LibNM::ActiveConnection> LibNM::Client::getActiveConnections() const
{ 
    ASSERT_CORRECT_CONTEXT;
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
                g_object_ref(G_OBJECT(con));
                connections.add(ActiveConnection(con));
            }
        }
    }
    return connections;
}

/*
 * Gets the primary active network connection.
 */
LibNM::ActiveConnection LibNM::Client::getPrimaryConnection() const
{ 
    ASSERT_CORRECT_CONTEXT;
    ActiveConnection primary;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        NMActiveConnection* con = nm_client_get_primary_connection(client);
        if(con != nullptr && NM_IS_ACTIVE_CONNECTION(con))
        {
            g_object_ref(G_OBJECT(con));
            primary = con;
        }
    }
    return primary;
}

/*
 * Gets the connection being activated by the network manager.
 */
LibNM::ActiveConnection LibNM::Client::getActivatingConnection() const
{ 
    ASSERT_CORRECT_CONTEXT;
    ActiveConnection activating;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        NMActiveConnection* con = nm_client_get_activating_connection(
                client);
        if(con != nullptr)
        {
            g_object_ref(G_OBJECT(con));
            activating = con;
        }
    }
    return activating;
}

/*
 * Deactivates an active network connection.
 */
void LibNM::Client::deactivateConnection(ActiveConnection& activeCon) 
{ 
    ASSERT_CORRECT_CONTEXT;
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        GLib::ObjectPtr<GObject*> connection(getOtherGObject(activeCon));
        if(connection != nullptr)
        {
            if(NM_IS_ACTIVE_CONNECTION((GObject*) connection))
            {
                nm_client_deactivate_connection(client,
                        NM_ACTIVE_CONNECTION((GObject*) connection));
            }
        }         
    }
}

/*
 * Checks if wireless connections are currently enabled.
 */
bool LibNM::Client::wirelessEnabled() const
{ 
    ASSERT_CORRECT_CONTEXT;
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
    ASSERT_CORRECT_CONTEXT;
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
    ASSERT_CORRECT_CONTEXT;
    if(error != nullptr)
    {
        handler->openingConnectionFailed(
                ActiveConnection(NM_ACTIVE_CONNECTION(connection)), error,
                false);
    }
    else
    {
        handler->openingConnection(
                ActiveConnection(NM_ACTIVE_CONNECTION(connection)), false);
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
    ASSERT_CORRECT_CONTEXT;
    if(error != nullptr)
    {
        handler->openingConnectionFailed
                (ActiveConnection(NM_ACTIVE_CONNECTION(connection)), error,
                true);
    }
    else
    {
        handler->openingConnection    
                (ActiveConnection(NM_ACTIVE_CONNECTION(connection)), true);
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
        bool usedSaved)
{ 
    ASSERT_CORRECT_CONTEXT;
    //determine if this is a new connection attempt
    bool isNew = !usedSaved || !wifiDevice.hasConnectionAvailable(connection);
    NMClientPtr client(NM_CLIENT(getGObject()));
    if(client != nullptr)
    {
        const char* apPath = accessPoint.getPath();
        NMConnectionPtr connectionObject 
            (NM_CONNECTION(getOtherGObject(connection)));
        NMDevicePtr deviceObject
            (NM_DEVICE(getOtherGObject(wifiDevice)));
       
        //connecting with a null connection object is allowed and may work.
        if(apPath != nullptr && deviceObject != nullptr)
        {
            if(isNew)
            {
                DBG("Client::activateConnection"
                        << ": adding new connection.");
                nm_client_add_and_activate_connection(client,
                    connectionObject,
                    deviceObject,
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
                        deviceObject,
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
    ASSERT_CORRECT_CONTEXT;
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
    ASSERT_CORRECT_CONTEXT;
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
    ASSERT_CORRECT_CONTEXT;
    ObjectPtr source(getGObject());
    if(source != nullptr)
    {
        listener.connectAllSignals(source);
    }
}
