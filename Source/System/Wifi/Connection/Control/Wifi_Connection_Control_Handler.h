#pragma once
/**
 * @file  Wifi_Connection_Control_Handler.h
 *
 * @brief  Provides methods for starting new Wifi connections or closing the
 *         existing Wifi connection.
 */

#include "SharedResource_Modular_Handler.h"

namespace Wifi 
{ 
    namespace Connection { namespace Control 
    { 
        class Handler; 
        class Module;
    } } 
    class Resource;
    class AccessPoint;
    namespace LibNM { class ActiveConnection; }
}

/**
 * @brief  Connects to the Connection::Control::Module to open or close Wifi
 *         connections.
 */
class Wifi::Connection::Control::Handler : 
        public SharedResource::Modular::Handler<Resource, Module>
{
public:
    Handler();

    virtual ~Handler() { }

    /**
     * @brief  Attempts to open a Wifi network connection using a nearby access
     *         point.
     *
     * @param toConnect          The access point to use in the attempted 
     *                           connection.
     *
     * @param securityKey        An optional security key to use when opening 
     *                           the connection. This will be ignored if it 
     *                           equals the empty string or if the access point
     *                           is unsecured.
     */
    void connectToAccessPoint(const AccessPoint toConnect,
            juce::String securityKey = juce::String());

    /**
     * @brief  Disconnects the active Wifi connection. If there is no active
     *         wifi connection, no action is taken.
     */
    void disconnect();
};
