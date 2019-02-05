#pragma once
/**
 * @file  Wifi_Connection_Control_Module.h
 *
 * @brief  Updates, adds, and removes both current and saved Wifi connections.
 */

#include "SharedResource_Modular_Module.h"
#include <gio/gio.h>

namespace Wifi 
{ 
    namespace Connection { namespace Control { class Module; } } 
    class Resource;
    class AccessPoint;
    namespace LibNM
    {
        class ConnectionHandler;
        class ActiveConnection;
        class AccessPoint;
    }
}

class Wifi::Connection::Control::Module : 
        public SharedResource::Modular::Module<Resource>
{
public:
    Module(Resource& parentResource);

    virtual ~Module() { }

    /**
     * @brief  Attempts to open a Wifi network connection using a nearby access
     *         point.
     *
     * @param toConnect          The access point to use in the attempted 
     *                           connection.
     *
     * @param connectionHandler  The object that will handle LibNM connection
     *                           callbacks.
     *
     * @param securityKey        An optional security key to use when opening 
     *                           the connection. This will be ignored if it 
     *                           equals the empty string or if the access point
     *                           is unsecured.
     */
    void connectToAccessPoint(const AccessPoint toConnect,
            LibNM::ConnectionHandler* connectionHandler,
            juce::String securityKey = juce::String());

    /**
     * @brief  Disconnects the active Wifi connection. If there is no active
     *         wifi connection, no action is taken.
     */
    void disconnect();

    /**
     * @brief  This function will be called whenever starting to activate a 
     *         connection succeeds.
     * 
     * @param connection  A new active connection object representing the 
     *                    added connection. This connection object might not
     *                    be completely connected yet.
     */
    void openingConnection(LibNM::ActiveConnection connection);
    
    /**
     * @brief  This function will be called whenever starting to activate a
     *         connection fails.
     * 
     * @param connection  The connection that failed to activate. This may 
     *                    be a null connection.
     * 
     * @param error       A GError object describing the problem. This error
     *                    object must be freed by the ConnectionHandler.
     */
    void openingConnectionFailed(LibNM::ActiveConnection connection, 
            GError* error);

private:
    /**
     * @brief  Gets the LibNM::AccessPoint object used to establish an active 
     *         connection.
     *
     * @param connection  An active connection object.
     *
     * @return            The LibNM access point used to activate the 
     *                    connection, or a null access point if the connection 
     *                    is null.
     */
    LibNM::AccessPoint getActiveConnectionNMAP
    (const LibNM::ActiveConnection connection);

    /**
     * @brief  Gets the Wifi::AccessPoint object used to establish an active 
     *         connection.
     *
     * @param connection  An active connection object.
     *
     * @return            The access point used to activate the connection, or a 
     *                    null access point if the connection is null.
     */
    Wifi::AccessPoint getActiveConnectionAP
    (const Wifi::LibNM::ActiveConnection connection);

};
