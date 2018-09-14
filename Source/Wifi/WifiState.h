#pragma once

/**
 * @file WifiState.h
 *
 * @brief Describes the current state of the system wifi device.
 */
enum class WifiState
{
    
    //no wifi device found
    missingNetworkDevice,
    //Wifi device is disabled.
    disabled,
    //Wifi device is being turned on.
    turningOn,
    //Wifi is turned on but not connected.
    enabled,
    //Wifi device is being turned on.
    turningOff,
    //Wifi device is connecting to an access point.
    connecting,
    //Connecting access point needs a password.
    missingPassword,
    //Wifi device is connected to an access point.
    connected,
    //wifi device is closing its connection to an access point.
    disconnecting
};

#ifdef JUCE_DEBUG    
/**
 * converts a wifistate to a string for debugging.
 */
static const char* wifiStateString(WifiState state)
{
    switch (state)
    {
        case WifiState::missingNetworkDevice:
            return "missingNetworkDevice";
        case WifiState::disabled:
            return "disabled";
        case WifiState::turningOn:
            return "turningOn";
        case WifiState::enabled:
            return "enabled";
        case WifiState::turningOff:
            return "turningOff";
        case WifiState::connecting:
            return "connecting";
        case WifiState::missingPassword:
            return "missingPassword";
        case WifiState::connected:
            return "connected";
        case WifiState::disconnecting:
            return "disconnecting";
    }
    return "unexpected state value!";
}
#endif
