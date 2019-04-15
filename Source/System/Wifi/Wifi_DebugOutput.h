#pragma once
/**
 * @file  Wifi_DebugOutput.h
 *
 * @brief  Converts assorted Wifi-related enums to text for debug output.
 */

#include <nm-device-wifi.h>

namespace Wifi
{
    /**
     * @brief  Gets text describing a NMState enum value.
     *
     * @param state  An enum representing an overall network state.
     *
     * @return       A text representation of the state value.
     */
    const char* nmStateString(NMState state);

    /**
     * @brief  Gets text describing a NMConnectivityState enum value.
     *
     * @param state  An enum representing the current network connection state.
     *
     * @return       A text representation of the state value.
     */
    const char* nmConnectivityStateString(NMConnectivityState state);


    /**
     * @brief  Gets text describing a NMDeviceState enum value.
     *
     * @param state  An enum representing the state of a network device.
     *
     * @return       A text representation of the state value.
     */
    const char* deviceStateString(NMDeviceState state);

    /**
     * @brief  Gets text describing a NMDeviceStateReason enum value.
     *
     * @param state  An enum representing the reason for a change in network
     *               device state.
     *
     * @return       A text representation of the stateReason value.
     */
    const char* deviceStateReasonString(NMDeviceStateReason stateReason);

    /**
     * @brief  Gets text describing a NMActiveConnectionState enum value.
     *
     * @param state  An enum representing the state of an active network
     *               connection.
     *
     * @return       A text representation of the state value.
     */
    const char* activeConnectionStateString(NMActiveConnectionState state);
}
