#pragma once
/**
 * @file  Wifi_Connection_Record_Listener.h
 *
 * @brief  A basis for classes that should receive updates whenever the Wifi
 *         network connection state changes.
 */

#ifndef WIFI_IMPLEMENTATION
    #define CONNECTION_LISTENER_IMPLEMENTATION
    #define WIFI_IMPLEMENTATION
#endif
#include "Wifi_Connection_Record_UpdateInterface.h"
#ifdef CONNECTION_LISTENER_IMPLEMENTATION
    #undef CONNECTION_LISTENER_IMPLEMENTATION
    #undef WIFI_IMPLEMENTATION
#endif
#include "SharedResource_Modular_Handler.h"

namespace Wifi
{
    class Resource;
    namespace Connection
    {
        namespace Record
        {
            class Listener;
            class Module;
        }
    }
}

/**
 * @brief  Receives updates whenever new Wifi connection events are recorded.
 *
 *  Listener object functions will be called by the Connection::Record::Module
 * to notify the Listener of new connection events. All of its notification
 * functions do nothing by default. Listener subclasses should override these
 * functions to handle the connection events as necessary.
 */
class Wifi::Connection::Record::Listener : public UpdateInterface,
    public SharedResource::Modular::Handler<Resource, Module>
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
     * @param disconnectedAP  The access point that was being used by the
     *                        closed connection.
     */
    virtual void disconnected(const AccessPoint disconnectedAP) override;
};
