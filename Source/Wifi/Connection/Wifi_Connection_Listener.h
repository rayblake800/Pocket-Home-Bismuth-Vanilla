#pragma once
/**
 * @file  Wifi_Connection_Listener.h
 *
 * @brief  Receives notifications whenever the Wifi network connection's state
 *         changes.
 */

#include "Wifi_Connection_UpdateInterface.h"
#include "SharedResource_Handler.h"

namespace Wifi { namespace Connection { class Listener; } }
namespace Wifi { namespace Connection { class RecordResource; } }

/**
 *  Connection::Listener object methods will be called by the 
 * Connection::RecordResource to notify the Listener of new connection events.  
 * All of these methods do nothing by default; Listener subclasses should 
 * override these methods to handle the connection events as necessary.
 */
class Wifi::Connection::Listener : public UpdateInterface,
    public SharedResource::Handler<RecordResource>
{
public:
    Listener();

    virtual ~Listener() { }

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
