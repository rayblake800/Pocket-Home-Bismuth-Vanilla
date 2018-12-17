#pragma once
/**
 * @file  LibNM/NMObjects/ConnectionSettings.h
 *
 * @brief  Holds basic network connection settings.
 */

#include "LibNM/NMObjects/Settings.h"
#include <nm-setting-connection.h>

namespace LibNM { class ConnectionSettings; }

class LibNM::ConnectionSettings : public Settings
{
public:
    /**
     * @brief  Creates a ConnectionSettings object that shares data with another
     *         ConnectionSettings object.
     *
     * @param toCopy  The ConnectionSettings object that will share its LibNM
     *                data with the new object.
     */
    ConnectionSettings(const ConnectionSettings& toCopy);

    /**
     * @brief  Creates a ConnectionSettings object to hold existing LibNM
     *         connection settings.
     *
     * @param toAssign  The LibNM connections settings object that the new
     *                  ConnectionSettings object will hold.
     */
    ConnectionSettings(NMSettingConnection* toAssign);

    /**
     * @brief  Creates a null ConnectionSettings object holding no data.
     */
    ConnectionSettings();

    virtual ~ConnectionSettings() { }

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
};
