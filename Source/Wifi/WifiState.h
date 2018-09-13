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

#ifdef juce_debug    
/**
 * converts a wifistate to a string for debugging.
 */
static const char* wifiStateString(WifiState state)
{
    switch (state)
    {
        case missingNetworkDevice:
            return "missingNetworkDevice";
        case disabled:
            return "disabled";
        case turningOn:
            return "turningOn";
        case enabled:
            return "enabled";
        case turningOff:
            return "turningOff";
        case connecting:
            return "connecting";
        case missingPassword:
            return "missingPassword";
        case connected:
            return "connected";
        case disconnecting:
            return "disconnecting";
    }
    return "unexpected state value!";
}
#endif
