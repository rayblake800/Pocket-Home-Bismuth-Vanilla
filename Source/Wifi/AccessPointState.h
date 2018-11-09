#pragma once

/**
 * @file  AccessPointState.h
 *
 * @brief  Describes the current state of a Wifi access point.
 */

enum class AccessPointState
{
    /* WifiAccessPoint contains no access point data. */
    nullAP,
    /* The access point is not visible to the wifi device. */
    missingAP,
    /* The access point is visible but not being used. */
    disconnectedAP,
    /* A connection is being created through this access point. */
    connectingAP,
    /* An active connection exists using this access point. */
    connectedAP,
    /* A connection using this access point is being closed. */
    disconnectingAP,
    /* Connecting failed because the security key was invalid. */
    invalidSecurityAP
};

#ifdef JUCE_DEBUG    

/**
 * @brief  Converts an AccessPointState to a string for debugging. 
 */
static const char* apStateString(AccessPointState state)
{
    switch(state)
    {
        case AccessPointState::nullAP:
            return "nullAP";
        case AccessPointState::missingAP:
            return "missingAP";
        case AccessPointState::disconnectedAP:
            return "disconnectedAP";
        case AccessPointState::connectingAP:
            return "connectingAP";
        case AccessPointState::connectedAP:
            return "connectedAP";
        case AccessPointState::disconnectingAP:
            return "disconnectingAP";
        case AccessPointState::invalidSecurityAP:
            return "invalidSecurityAP";
    }
    return "apStateString: missing AccessPointState!";
}
#endif

