#ifndef WIFI_IMPLEMENTATION
    #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_LibNM_ConnectionHandler.h
 *
 * @brief  Handles LibNM connection callbacks when opening a connection.
 */

#include <nm-client.h>

namespace Wifi { namespace LibNM
{
    class ConnectionHandler;
    class ActiveConnection;
} }

/**
 * @brief  Handles connection activation attempts.
 *
 *  When attempting to activate a new connection, a connection handler must be
 * provided to receive the resulting new connection object along with any errors
 * that occur.
 */
class Wifi::LibNM::ConnectionHandler
{
public:
    // Callback functions may only be called through the LibNM::Client.
    friend class Client;

    ConnectionHandler() { }

    virtual ~ConnectionHandler() { }

private:
    /**
     * @brief  This function will be called whenever starting to activate a
     *         connection succeeds.
     *
     * @param connection  A new active connection object representing the added
     *                    connection. This connection object might not be
     *                    completely connected yet.
     */
    virtual void openingConnection(ActiveConnection connection) = 0;

    /**
     * @brief  This function will be called whenever starting to activate a
     *         connection fails.
     *
     * @param connection  The connection that failed to activate. This may be a
     *                    null connection.
     *
     * @param error       A GError object describing the problem. This error
     *                    object must be freed by the ConnectionHandler.
     */
    virtual void openingConnectionFailed(ActiveConnection connection,
            GError* error) = 0;
};
