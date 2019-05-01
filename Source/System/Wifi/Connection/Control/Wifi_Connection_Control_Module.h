#ifndef WIFI_IMPLEMENTATION
    #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Connection_Control_Module.h
 *
 * @brief  Updates, adds, and removes both current and saved Wifi connections.
 */

#include "Wifi_Module.h"
#include "Wifi_LibNM_ConnectionHandler.h"
#include <gio/gio.h>

namespace Wifi
{
    namespace Connection
    {
        class Event;
        namespace Control { class Module; }
    }
    class Resource;
    class AccessPoint;
    namespace LibNM
    {
        class AccessPoint;
        class ConnectionHandler;
        class ActiveConnection;
    }
}

/**
 * @brief  Handles the process of creating, opening, and closing Wifi network
 *         connections.
 *
 *  The Control::Module object handles the process of creating a new network
 * connection, or closing an existing one. It determines whether saved
 * connections can be reused, which access point device should be used to open
 * the connection, and what kind of security settings to create in order to
 * activate a new connection.
 */
class Wifi::Connection::Control::Module : public Wifi::Module,
        public LibNM::ConnectionHandler, protected juce::Timer
{
public:
    /**
     * @brief  Connects the module to its Resource.
     *
     * @param parentResource  The Wifi::Resource object instance.
     */
    Module(Resource& parentResource);

    /**
     * @brief  Ensures all pending connection data is removed.
     */
    virtual ~Module();

    /**
     * @brief  Checks if the Control::Module is currently attempting to open a
     *         Wifi connection.
     *
     * @return  Whether a connection attempt is in progress.
     */
    bool tryingToConnect() const;

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
     * @brief  Continues the connection attempt if a pending connection is
     *         currently in progress.
     *
     *  This function should only be called within the LibNM::Thread::Module's
     * thread.
     */
    void continueConnectionAttempt();

    /**
     * @brief  Asynchronously closes the active Wifi connection. If there is no
     *         active wifi connection, no action is taken.
     */
    void disconnect();

    /**
     * @brief  Notifies the Control::Module that a new access point was
     *         spotted, just in case it is needed to establish a connection.
     *
     * @param addedAP  An access point object that was just added.
     */
    void signalAPAdded(LibNM::AccessPoint newAP);

    /**
     * @brief  Notifies the Control::Module that wireless networking was
     *         disabled.
     */
    void signalWifiDisabled();

    /**
     * @brief  Notifies the Control::Module when a new connection event is
     *         added.
     *
     *  If this is a connection failure attempt and the Module is attempting to
     * connect, this event will not have been sent to the Record::Module. The
     * Control::Module is responsible for deciding when a Wifi connection
     * attempt has failed, passing the failure event to the Record::Module at
     * that point.
     *
     * @param newEvent  The new Wifi connection event to report.
     */
    void wifiEventRecorded(const Event newEvent);

private:
    /**
     * @brief  Clears all saved data being used for an ongoing Wifi connection
     *         attempt.
     */
    void clearPendingConnectionData();

    /**
     * @brief  Cancels any pending connection attempt.
     */
    void cancelPendingConnection();

    /**
     * @brief  Closes the active Wifi connection. Only call this within the
     *         LibNM thread.
     */
    void internalDisconnect();

    /**
     * @brief  Cancels a pending connection event if it doesn't finish within a
     *         timeout period.
     */
    void timerCallback() override;

    /**
     * @brief  Signals that a connection is being opened.
     *
     * @param connection  A new active connection object representing the added
     *                    connection. This connection object might not be
     *                    completely connected yet.
     */
    void openingConnection(LibNM::ActiveConnection connection) override;

    /**
     * @brief  Signals that an attempt to open a connection failed.
     *
     * @param connection  The connection that failed to activate. This may be a
     *                    null connection.
     *
     * @param error       A GError object describing the problem.
     */
    virtual void openingConnectionFailed(LibNM::ActiveConnection connection,
            GError* error) override;

    // Pending connection data:

    // Whether the module is attempting to open a connection:
    bool connectionStarted = false;

    // Whether a connection is currently being activated:
    bool connectionActivating = false;

    // The security key used to establish the connection:
    juce::String pendingPSK;

    // Whether a new connection is being used to open the connection:
    bool creatingNewConnection = false;
};
