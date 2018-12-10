#pragma once
/**
 * @file  Wifi/Control/ConnectionResource.h
 *
 * @brief  Views, updates, adds, and removes both current and saved Wifi 
 *         connections.
 */

#include "LibNM/NMObjects/Client.h"
#include "SharedResource/Resource.h"
#include "JuceHeader.h"

namespace Wifi { class ConnectionResource; }
namespace Wifi { class AccessPoint; }

class Wifi::ConnectionResource : public LibNM::Client::ConnectionHandler
{
public:
    ConnectionResource() { }

    virtual ~ConnectionResource() { }

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

    /**
     * @brief  Checks if Wifi is currently connected.
     *
     * @return  Whether an active Wifi internet connection exists.
     */
    bool isWifiConnected() const;

    /**
     * @brief  Checks if Wifi is currently connecting.
     *
     * @return   Whether an active Wifi connection is being established.
     */
    bool isConnecting() const;

    /**
     * @brief  Gets the access point used by the current active or activating
     *         connection.
     *
     * @return  The access point in use, or a null AccessPoint if Wifi is not
     *          connected or connecting.
     */
    AccessPoint getActiveAP() const;

    /**
     * @brief  Checks if a saved network connection exists for a particular 
     *         access point.
     *
     * @param toCheck  A non-null AccessPoint representing a visible Wifi access
     *                 point.
     *
     * @return         Whether NetworkManager has a saved network connection
     *                 that is compatible with the given access point.
     */
    bool hasSavedConnection(const AccessPoint& toCheck);

    /**
     * @brief  Finds the last time the system was connected with a given access
     *         point.
     *
     * @param toCheck  An access point that is compatible with a saved Wifi
     *                 connection.
     *
     * @return         The last time a connection compatible with access point
     *                 toCheck was active, or the Unix epoch if no matching 
     *                 connection was found.
     */
    juce::Time lastConnectionTime(const AccessPoint& toCheck) const;

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
