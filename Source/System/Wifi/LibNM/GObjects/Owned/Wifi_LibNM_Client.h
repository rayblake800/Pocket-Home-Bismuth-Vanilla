#pragma once
/**
 * @file  Wifi_LibNM_Client.h
 * 
 * @brief  A RAII container and C++ interface for the LibNM NMClient class.
 */

#include "GLib_Owned_Object.h"
#include "GLib_Signal_Handler.h"
#include "GLib_Borrowed_ObjectLender.h"
#include <nm-client.h>

namespace Wifi 
{ 
    namespace LibNM 
    {
        class Client;
        class LenderInterface;
        class DeviceWifi;
        class AccessPoint;
        class ActiveConnection;
        class Connection;
        class ConnectionHandler;
        namespace Signal { class ClientHandler; }
    } 
}

/**
 * @brief  The interface used to control access to the Client's 
 *         ObjectLenders.
 */
class Wifi::LibNM::LenderInterface
{
public: 
    virtual ~LenderInterface() { }

private:
    friend class Client;
    friend class Signal::ClientHandler;

    /* More convenient names for ObjectLenders managed by the client: */
    typedef GLib::Borrowed::ObjectLender<DeviceWifi> DeviceLender;
    typedef GLib::Borrowed::ObjectLender<ActiveConnection> ConnectionLender;

    /**
     * @brief  Gets the Client's Wifi device object lender.
     *
     * @return   The WifiDevice lender.
     */
    virtual DeviceLender* getDeviceLender() const = 0;

    /**
     * @brief  Gets the Client's connection object lender.
     *
     * @return   The ActiveConnection lender.
     */
    virtual ConnectionLender* getConnectionLender() const = 0;
};

/**
 *  The LibNM Client provides the main interface used to access and control
 * the NetworkManager. It is responsible for creating network device objects,
 * opening new connections, and enabling or disabling wireless networks. 
 *
 *  Client provides a Listener interface, which may be used to receive
 * notifications when wireless networking is enabled or disabled.
 */
class Wifi::LibNM::Client : public GLib::Owned::Object, public LenderInterface
{
public:
    /**
     * @brief  Creates a null Client object.
     */
    Client();
   
    /**
     * @brief  Creates a Client holding an existing NMClient object.
     *
     * @param toAssign          The NMClient* represented by the new Client.
     *
     * @param deviceLender      The container held by the LibNM::Thread::Module
     *                          containing DeviceWifi objects managed by the
     *                          Client.
     *
     * @param connectionLender  The container held by the LibNM::Thread::Module
     *                          containing ActiveConnection objects managed by
     *                          the client.
     */
    Client(NMClient* toAssign, DeviceLender* deviceLender,
            ConnectionLender* connectionLender);

    virtual ~Client() { }

    /**
     * @brief  Gets all wifi devices from the network manager.
     * 
     * @return  All NMDevices found that are wifi devices, packaged into an
     *          array of DeviceWifi objects.
     */
    juce::Array<DeviceWifi> getWifiDevices() const;
    
    /**
     * @brief  Gets a specific wifi device using its interface name.
     * 
     * @param interface  The interface name of the desired wifi device.
     * 
     * @return           The requested wifi device, or a null object if no valid
     *                   device is found.
     */
    DeviceWifi getWifiDeviceByIface(const char* interface) const;
        
    /**
     * @brief  Gets a specific wifi device using its DBus path.
     * 
     * @param path  The DBus path of the desired wifi device.
     * 
     * @return      The requested wifi device, or a null object if no valid 
     *              device is found.
     */
    DeviceWifi getWifiDeviceByPath(const char* path) const;
    
    /**
     * @brief  Gets the list of all active connections known to the network 
     *         manager.
     * 
     * @return  An array of all active network connections.
     */
    juce::Array<ActiveConnection> getActiveConnections() const;
    
    /**
     * @brief  Gets the primary active network connection.
     * 
     * @return  An active Connection object, or a null object if there is no
     *          primary connection.
     */
    ActiveConnection getPrimaryConnection() const;
    
    /**
     * @brief  Gets the connection being activated by the network manager.
     * 
     * @return  The activating connection as an ActiveConnection object, or a 
     *          null object if there is no activating connection.
     */
    ActiveConnection getActivatingConnection() const;
    
    /**
     * @brief  Deactivates an active network connection.
     * 
     * @param activeCon  The network connection to deactivate.  If this 
     *                   connection is not active, nothing will happen.
     */
    void deactivateConnection(ActiveConnection& activeCon) const;
    
    /**
     * @brief  Checks if wireless connections are currently enabled.
     * 
     * @return  Whether wifi is enabled.
     */
    bool wirelessEnabled() const;
    
    /**
     * @brief  Sets whether wireless connections are enabled.
     * 
     * @param enabled  If true, wifi will be enabled, if false, wifi will be
     *                 disabled.
     */
    void setWirelessEnabled(bool enabled) const;
    
    
    /**
     * @brief  Activates a wifi network connection, and attempts to set it as 
     *         the primary network connection.
     * 
     * @param connection   A network connection to activate. If this is a new 
     *                     connection, it will be saved by the network manager.
     * 
     * @param wifiDevice   The system network device used to activate the
     *                     connection. 
     * 
     * @param accessPoint  The wifi access point used to activate the 
     *                     connection.
     * 
     * @param handler      The connectionHandler object that will receive the 
     *                     new connection object.
     * 
     * @param useSaved     If true, the client will attempt to re-activate an
     *                     existing connection. Otherwise, a new connection
     *                     will be created even if an existing one is found.  
     */
    void activateConnection(
            const Connection& connection,
            const DeviceWifi& wifiDevice,
            const AccessPoint& accessPoint,
            ConnectionHandler* handler,
            bool useSaved = true);

private:
    /**
     * @brief  Gets the Client's Wifi device object lender.
     *
     * @return   The WifiDevice lender.
     */
    virtual DeviceLender* getDeviceLender() const override;

    /**
     * @brief  Gets the Client's connection object lender.
     *
     * @return   The ActiveConnection lender.
     */
    virtual ConnectionLender* getConnectionLender() const override;

    /* A private data structure providing access to objects needed to handle 
       connection callbacks: */
    struct CallbackData;

    /**
     * @brief  The NMClientActivateFn called by LibNM when activating an 
     *         existing connection.
     *
     *  This is used to create a call to ConnectionHandler::openingConnection or
     * to ConnectionHandler::openingConnectionFailed.
     * 
     * @param client        The LibNM Client object handling the connection.
     *
     * @param connection    The new activating connection's LibNM object.
     *
     * @param error         A connection error value to handle, or nullptr.
     * 
     * @param callbackData  The callback data structure created in the
     *                      activateConnection call that scheduled this 
     *                      callback function.
     */
    static void activateCallback(
            NMClient* client,
            NMActiveConnection* connection,
            GError* error,
            CallbackData* callbackData);

    /**
     * @brief  The NMClientAddActivateFn called by LibNM when adding and 
     *         activating an existing connection.
     *
     *  This is used to create a call to ConnectionHandler::openingConnection or
     * to ConnectionHandler::openingConnectionFailed.
     *
     * @param client        The LibNM Client object handling the connection.
     *
     * @param connection    The new activating connection's LibNM object.
     *
     * @param path          The new connection's DBus path.
     *
     * @param error         A connection error value to handle, or nullptr.
     * 
     * @param callbackData  The callback data structure created in the
     *                      activateConnection call that scheduled this 
     *                      callback function.
     */
    static void addActivateCallback(
            NMClient* client,
            NMActiveConnection* connection,
            const char* path,
            GError* error,
            CallbackData* callbackData);

    /* ObjectLenders are managed by the Client, but held by the 
       LibNM::Thread::Module. */

    /* Holds DeviceWifi objects for all valid wifi devices. */
    DeviceLender* deviceLender = nullptr;

    /* Holds ActiveConnection objects for all valid NMActiveConnection* values
       taken from the Client. */
    ConnectionLender* connectionLender = nullptr;

    JUCE_LEAK_DETECTOR(Wifi::LibNM::Client);
};
