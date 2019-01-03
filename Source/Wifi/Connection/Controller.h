#pragma once
/**
 * @file  Wifi/Connection/Controller.h
 *
 * @brief  Updates, adds, and removes both current and saved Wifi connections.
 */

#include "LibNM/OwnedObjects/Client.h"
#include "SharedResource_Resource.h"
#include "JuceHeader.h"

namespace Wifi { class AccessPoint; }
namespace Wifi { namespace Connection { class Controller; } }

class Wifi::Connection::Controller : public LibNM::Client::ConnectionHandler
{
public:
    Controller() { }

    virtual ~Controller() { }

    /**
     * @brief  Attempts to open a Wifi network connection using a nearby access
     *         point.
     *
     * @param toConnect    The access point to use in the attempted connection.
     *
     * @param securityKey  An optional security key to use when opening the
     *                     connection. This will be ignored if it equals the
     *                     empty string or if the access point is unsecured.
     */
    void connectToAccessPoint(const AccessPoint toConnect,
            juce::String securityKey = juce::String()) const;

    /**
     * @brief  Disconnects the active Wifi connection. If there is no active
     *         wifi connection, no action is taken.
     */
    void disconnect() const;

    /**
     * @brief  Discards all saved connection data linked to an access point.
     *
     * @param toRemove  The Wifi access point to remove from saved connections. 
     */
    void removeSavedConnection(const AccessPoint toRemove) const;

private:
    /**
     * @brief  Signals that a connection is being opened.
     *
     * @param connection  The activating connection object.
     */
    virtual void openingConnection(LibNM::ActiveConnection connection) override;

    /**
     * @brief  Signals that an attempt to open a connection failed.
     *
     * @param connection  The failed connection object.
     *
     * @param error       A GLib error object explaining the problem.
     */
    virtual void openingConnectionFailed(LibNM::ActiveConnection connection,
            GError* error) override;
};
