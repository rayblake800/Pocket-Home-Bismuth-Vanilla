#pragma once
/**
 * @file  Wifi/Connection/ConnectionListener.h
 *
 * @brief  Receives notifications whenever the Wifi network connection's state
 *         changes.
 */

#include "Wifi/Connection/ConnectionUpdateInterface.h"
#include "SharedResource/Handler.h"

namespace Wifi { class ConnectionListener; }
namespace Wifi { class ConnectionTracker; }

/**
 *  ConnectionListener's methods will be called by the ConnectionTracker to 
 * notify the Listener of new connection events.  All of these methods do
 * nothing by default. ConnectionListener subclasses should override these
 * methods to handle the connection events as necessary.
 */
class Wifi::ConnectionListener : public ConnectionUpdateInterface,
    public SharedResource::Handler<ConnectionTracker>
{
public:
    ConnectionListener() { };

    virtual ~ConnectionListener();

private:
    /**
     * @brief  Signals that a new active Wifi connection is being opened.
     *
     * @param connectingAP  The Wifi access point the system is trying to 
     *                      connect to.
     */
    virtual void startedConnecting(const AccessPoint connectingAP) override;

    /**
     * @brief  Signals that an attempt to open a Wifi connection failed due to
     *         improper authentication, usually because of an incorrect Wifi
     *         password.
     *
     * @param connectingAP  The access point the system was attempting to use
     *                      to open the new connection.
     */
    virtual void connectionAuthFailed(const AccessPoint connectingAP) override;

    /**
     * @brief  Signals that a new Wifi connection was opened successfully.
     *
     * @param connectedAP  The access point used to open the connection.
     */
    virtual void connected(const AccessPoint connectedAP) override; 

    /**
     * @brief  Signals that an active Wifi connection was closed.
     *
     * @param disconnectedAP  The access point that was being used by the closed
     *                        connection.
     */
    virtual void disconnected(const AccessPoint disconnectedAP) override; 
};
