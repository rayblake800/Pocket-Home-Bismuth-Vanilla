#pragma once
/**
 * @file  Wifi/Control/ConnectionHandler.h
 *
 * @brief  Connects to and disconnects from wireless networks, and clears saved
 *         connections.
 */

#include "LibNM/NMObjects/Client.h"
#include "JuceHeader.h"

namespace Wifi { class ConnectionManager; }
namespace Wifi { class AccessPoint; }

class Wifi::ConnectionManager : public LibNM::Client::ConnectionHandler
{
public:
    ConnectionManager() { }

    virtual ~ConnectionManager() { }

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
     * @param toForget  The Wifi access point to remove from saved connections. 
     */
    void forgetConnection(const AccessPoint toForget) const;

private:
    /**
     * @brief  Signals that a connection is being opened.
     *
     * @param connection  The activating connection object.
     *
     * @param isNew       Whether the activating connection is new.
     */
    virtual void openingConnection(LibNM::ActiveConnection connection,
            bool isNew) override;

    /**
     * @brief  Signals that an attempt to open a connection failed.
     *
     * @param connection  The failed connection object.
     *
     * @param error       A GLib error object explaining the problem.
     *
     * @param isNew       Whether the failed connection was new.
     */
    virtual void openingConnectionFailed(LibNM::ActiveConnection connection,
            GError* error, bool isNew) override;
};
