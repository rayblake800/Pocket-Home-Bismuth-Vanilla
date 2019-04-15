#pragma once
/**
 * @file  Wifi_LibNM_Settings_Connection.h
 *
 * @brief  Holds basic network connection settings.
 */

#include "Wifi_LibNM_Settings_Object.h"
#include <nm-setting-connection.h>

namespace Wifi { namespace LibNM { namespace Settings { class Connection; } } }

/**
 * @brief  Manages a NMSettingsConnection* value, used to store basic network
 *         connection settings.
 */
class Wifi::LibNM::Settings::Connection : public Object
{
public:
    /**
     * @brief  Creates a Settings::Connection object to hold existing LibNM
     *         connection settings.
     *
     * @param toAssign  The LibNM connections settings object that the new
     *                  object will hold.
     */
    Connection(NMSettingConnection* toAssign);

    /**
     * @brief  Creates a null object holding no data.
     */
    Connection();

    virtual ~Connection() { }

    /**
     * @brief  Gets the basic ID used by this object's connection.
     *
     * @return  The connection's display name.
     */
    juce::String getID() const;

    /**
     * @brief  Gets the UUID used by this object's connection.
     *
     * @return  The connection's unique UUID string.
     */
    juce::String getUUID() const;

    /**
     * @brief  Gets the network interface used by this object's connection.
     *
     * @return  The connection's network interface name.
     */
    juce::String getConnectionInterface() const;

    /**
     * @brief  Gets the last timestamp saved for this object's connection.
     *
     * @return  The last time the connection was successfully activated
     *          completely.
     */
    juce::Time getTimestamp() const;

    JUCE_LEAK_DETECTOR(Wifi::LibNM::Settings::Connection);
};
