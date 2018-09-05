#pragma once

/**
 * @file AccessPointState
 *
 * @brief Describes the current state of a Wifi access point.
 */

enum AccessPointState
{
    // WifiAccessPoint contains no access point data.
    nullAP,
    // The access point is not visible to the wifi device.
    missingAP,
    // The access point is visible but not being used.
    disconnectedAP,
    // A connection is being created through this access point.
    connectingAP,
    // An active connection exists using this access point.
    connectedAP,
    // A connection using this access point is being closed.
    disconnectingAP,
    // Connecting failed because the security key was invalid.
    invalidSecurityAP
};

#ifdef JUCE_DEBUG    

/**
 * Converts an AccessPointState to a string for debugging. 
 */
static const char* apStateString(AccessPointState state)
{
    switch(state)
    {
        case nullAP:
            return "nullAP";
        case missingAP:
            return "missingAP";
        case disconnectedAP:
            return "disconnectedAP";
        case connectingAP:
            return "connectingAP";
        case connectedAP:
            return "connectedAP";
        case disconnectingAP:
            return "disconnectingAP";
        case invalidSecurityAP:
            return "invalidSecurityAP";
    }
    return "apStateString: missing AccessPointState!";
}
#endif

