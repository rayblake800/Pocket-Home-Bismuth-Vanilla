// Required to implement the client's private ThreadHandler class. */
#define LIB_NM_THREAD_IMPLEMENTATION
#define WIFI_IMPLEMENTATION

#include "Wifi_LibNM_Client.h"
#include "Wifi_LibNM_Connection.h"
#include "Wifi_LibNM_ConnectionHandler.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_ActiveConnection.h"
#include "Wifi_LibNM_ContextTest.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "GLib_ObjectPtr.h"
#include "SharedResource_Modular_Handler.h"

#ifdef JUCE_DEBUG
    // Print the full class name before all debug output:
    static const constexpr char* dbgPrefix = "Wifi::LibNM::Client::";
#endif

// Creates a null Client object.
Wifi::LibNM::Client::Client() :
GLib::Owned::Object(NM_TYPE_CLIENT) { }


// Creates a Client holding an existing NMClient object.
Wifi::LibNM::Client::Client(NMClient* toAssign, DeviceLender* deviceLender,
            ConnectionLender* connectionLender) :
GLib::Owned::Object(G_OBJECT(toAssign), NM_TYPE_CLIENT),
deviceLender(deviceLender),
connectionLender(connectionLender)
{
    ASSERT_NM_CONTEXT;
    jassert(deviceLender != nullptr);
    jassert(connectionLender != nullptr);
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
    return NM_CLIENT( (GObject*) clientPtr);
}


// Gets all wifi devices from the network manager.
juce::Array<Wifi::LibNM::DeviceWifi>
Wifi::LibNM::Client::getWifiDevices() const
{
    ASSERT_NM_CONTEXT;
    juce::Array<DeviceWifi> devices;
    GLib::ObjectPtr dataPtr(*this);
    if (dataPtr != nullptr)
    {
        const GPtrArray* deviceArray
            = nm_client_get_devices(toClientPtr(dataPtr));
        for (int i = 0; deviceArray && i < deviceArray->len; i++)
        {
            NMDevice* device = NM_DEVICE(deviceArray->pdata[i]);
            if (device != nullptr && NM_IS_DEVICE_WIFI(device))
            {
                devices.add(deviceLender->borrowObject(G_OBJECT(device)));
            }
        }
    }
    return devices;
}


// Gets a specific wifi device using its interface name.
Wifi::LibNM::DeviceWifi Wifi::LibNM::Client::getWifiDeviceByIface
(const char* interface) const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if (dataPtr != nullptr)
    {
        NMDevice* device = nm_client_get_device_by_iface(toClientPtr(dataPtr),
                interface);
        if (device != nullptr && NM_IS_DEVICE_WIFI(device))
        {
            return deviceLender->borrowObject(G_OBJECT(device));
        }
    }
    return DeviceWifi();
}


// Gets a specific wifi device using its DBus path.
Wifi::LibNM::DeviceWifi Wifi::LibNM::Client::getWifiDeviceByPath
(const char* path) const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if (dataPtr != nullptr)
    {
        NMDevice* device = nm_client_get_device_by_path(toClientPtr(dataPtr),
                path);
        if (device != nullptr && NM_IS_DEVICE_WIFI(device))
        {
            return deviceLender->borrowObject(G_OBJECT(device));
        }
    }
    return DeviceWifi();
}


// Gets the list of all active connections known to the network manager.
juce::Array<Wifi::LibNM::ActiveConnection>
Wifi::LibNM::Client::getActiveConnections() const
{
    ASSERT_NM_CONTEXT;
    juce::Array<ActiveConnection> connections;
    GLib::ObjectPtr dataPtr(*this);
    if (dataPtr != nullptr)
    {
        const GPtrArray* nmConnections
                = nm_client_get_active_connections(toClientPtr(dataPtr));
        for (int i = 0; nmConnections && i < nmConnections->len; i++)
        {
            NMActiveConnection* connection
                = NM_ACTIVE_CONNECTION(nmConnections->pdata[i]);
            if (connection != nullptr && NM_IS_ACTIVE_CONNECTION(connection))
            {
                connections.add(connectionLender->borrowObject
                        (G_OBJECT(connection)));
            }
        }
    }
    return connections;
}


// Gets the primary active network connection.
Wifi::LibNM::ActiveConnection Wifi::LibNM::Client::getPrimaryConnection() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if (dataPtr != nullptr)
    {
        NMActiveConnection* connection
                = nm_client_get_primary_connection(toClientPtr(dataPtr));
        if (connection != nullptr && NM_IS_ACTIVE_CONNECTION(connection))
        {
            return connectionLender->borrowObject(G_OBJECT(connection));
        }
    }
    return ActiveConnection();
}


// Gets the connection being activated by the network manager.
Wifi::LibNM::ActiveConnection
Wifi::LibNM::Client::getActivatingConnection() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if (dataPtr != nullptr)
    {
        NMActiveConnection* connection
                = nm_client_get_activating_connection(toClientPtr(dataPtr));
        if (connection != nullptr && NM_IS_ACTIVE_CONNECTION(connection))
        {
            return connectionLender->borrowObject(G_OBJECT(connection));
        }
    }
    return ActiveConnection();
}


// Deactivates an active network connection.
void Wifi::LibNM::Client::deactivateConnection
(ActiveConnection& activeCon) const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if (dataPtr != nullptr)
    {
        if (!activeCon.isNull())
        {
            nm_client_deactivate_connection(toClientPtr(dataPtr),
                    NM_ACTIVE_CONNECTION(activeCon.getGObject()));
            connectionLender->invalidateObject(activeCon.getGObject());
        }
    }
}


// Checks if wireless connections are currently enabled.
bool Wifi::LibNM::Client::wirelessEnabled() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if (dataPtr != nullptr)
    {
        return nm_client_wireless_get_enabled(toClientPtr(dataPtr));
    }
    return false;
}


// Sets whether wireless connections are enabled.
void Wifi::LibNM::Client::setWirelessEnabled(bool enabled) const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr dataPtr(*this);
    if (dataPtr != nullptr)
    {
        nm_client_wireless_set_enabled(toClientPtr(dataPtr), enabled);
    }
}


// Gets the Client's Wifi device object lender.
Wifi::LibNM::LenderInterface::DeviceLender*
Wifi::LibNM::Client::getDeviceLender() const
{
    return deviceLender;
}


// Gets the Client's connection object lender.
Wifi::LibNM::LenderInterface::ConnectionLender*
Wifi::LibNM::Client::getConnectionLender() const
{
    return connectionLender;
}


// Stores access to the Client object and a ConnectionHandler pointer so that
// they can be passed to LibNM connection callback methods.
struct Wifi::LibNM::Client::CallbackData
{
    Client client;
    ConnectionHandler* const handler;

    CallbackData(Client& client, ConnectionHandler* const handler) :
        client(client), handler(handler) { }

private:
    JUCE_LEAK_DETECTOR(Wifi::LibNM::Client::CallbackData);
};


// Activates a wifi network connection, attempting to set it as the primary
// network connection.
void Wifi::LibNM::Client::activateConnection(
        const Connection& connection,
        const DeviceWifi& wifiDevice,
        const AccessPoint& accessPoint,
        ConnectionHandler* handler,
        bool usedSavedConnection)
{
    ASSERT_NM_CONTEXT;
    //determine if this is a new connection attempt
    bool isNew = !usedSavedConnection;
    GLib::ObjectPtr clientDataPtr(*this);
    if (clientDataPtr != nullptr)
    {
        const char* apPath = accessPoint.getPath();
        // Never try to connect with a null wifi device or access point.
        // Using a null connection object is allowed and may work.
        if (apPath != nullptr && !wifiDevice.isNull())
        {
            GLib::ObjectPtr connectionDataPtr(connection);
            if (isNew)
            {
                DBG(dbgPrefix << __func__ << ": adding new connection.");
                nm_client_add_and_activate_connection(
                        toClientPtr(clientDataPtr),
                        NM_CONNECTION( (GObject*) connectionDataPtr),
                        NM_DEVICE(wifiDevice.getGObject()),
                        apPath,
                        (NMClientAddActivateFn)
                        addActivateCallback,
                        new CallbackData(*this, handler));
            }
            else
            {
                DBG(dbgPrefix << __func__ << ": activating saved connection.");
                nm_client_activate_connection(
                        toClientPtr(clientDataPtr),
                        NM_CONNECTION( (GObject*) connectionDataPtr),
                        NM_DEVICE(wifiDevice.getGObject()),
                        apPath,
                        (NMClientActivateFn)
                        activateCallback,
                        new CallbackData(*this, handler));
            }
        }
    }
}


// The NMClientActivateFn called by LibNM when activating an existing
// connection.
void Wifi::LibNM::Client::activateCallback(
        NMClient* client,
        NMActiveConnection* connection,
        GError* error,
        CallbackData* callbackData)
{
    ASSERT_NM_CONTEXT;
    std::unique_ptr<CallbackData> dataPtr(callbackData);
    ActiveConnection activeConnection = dataPtr->client.connectionLender
            ->borrowObject(G_OBJECT(connection));
    if (error != nullptr)
    {
        dataPtr->handler->openingConnectionFailed(activeConnection, error);
    }
    else
    {
        dataPtr->handler->openingConnection(activeConnection);
    }
}


// The NMClientAddActivateFn called by LibNM when adding and activating a new
// connection.
void Wifi::LibNM::Client::addActivateCallback(
        NMClient* client,
        NMActiveConnection* connection,
        const char* path,
        GError* error,
        CallbackData* callbackData)
{
    activateCallback(client, connection, error, callbackData);
}

