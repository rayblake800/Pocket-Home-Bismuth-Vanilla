#pragma once
#include "SharedResource_Modular_Handler.h"
#include "Wifi_LibNM_ConnectionHandler.h"


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

class Wifi::Connection::Control::Handler : 
        public SharedResource::Modular::Handler<Resource, Module>,
        public LibNM::ConnectionHandler
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

private:
    /**
     * @brief  This function will be called whenever starting to activate a 
     *         connection succeeds.
     * 
     * @param connection  A new active connection object representing the 
     *                    added connection. This connection object might not
     *                    be completely connected yet.
     */
    virtual void openingConnection(LibNM::ActiveConnection connection) override;
    
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
    virtual void openingConnectionFailed(LibNM::ActiveConnection connection, 
            GError* error) override;
};
