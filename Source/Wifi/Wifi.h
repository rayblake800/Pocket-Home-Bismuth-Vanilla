#pragma once

/**
 * @file  Wifi/Wifi.h
 *
 * @brief  Controls the Wifi device, manages network connections, and tracks
 *         wifi status.
 */
namespace Wifi
{
    /* ###################  Wifi Component Classes: ############################
     * These classes create UI Components that allow the user to view or control
     * Wifi status.
     */

    /**
     * @brief  Shows the signal strength of the active Wifi connection.
     */
    class Icon;

    /**
     * @brief  Enables or disables Wifi, shows the active connection, and opens
     *         the wifi settings page.
     */
    class ControlWidget;

    /**
     * @brief  Shows all visible Wifi access points, and allows the user to
     *         control the Wifi connection.
     */
    class SettingsPage;

    /* #####################  Wifi Control Classes: ############################
     * These classes are used to control the Wifi device and the current Wifi
     * connection.
     */

    /**
     * @brief  Sends basic commands to the wifi device.
     */
    class Controller;
    
    /**
     * @brief  Controls the active Wifi connection.
     */
    class ConnectionManager;

    /* ###################### Wifi State Monitoring: ###########################
     * These classes track and stores the current state of the Wifi device.
     */

    /**
     * @brief  Represents a visible Wifi access point.
     */
    class AccessPoint;

    /**
     * @brief  Caches and updates all visible access points.
     */
    class APList;

    /**
     * @brief  Keeps a record of connection attempts.
     */
    class ConnectionHistory;

    /**
     * @brief  Tracks the active Wifi connection state.
     */
    class ConnectionTracker;
    
    /**
     * @brief  Tracks the current state of the Wifi device.
     */
    class DeviceTracker;

    /* ######################## Wifi Listeners: ###############################
     * These classes receive notifications when Wifi data updates.
     */

    /**
     * @brief  Listens for changes in a single access point's signal strength.
     */
    class SignalStrengthListener;

    /**
     * @brief  Listens for changes to the current wifi connection state.
     */
    class ConnectionListener;

    /**
     * @brief  Listens for changes to the current wifi device's state.
     */
    class DeviceListener;
}
