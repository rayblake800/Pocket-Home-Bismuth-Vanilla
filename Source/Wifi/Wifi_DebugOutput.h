#pragma once
#include <nm-device-wifi.h>

/** 
 * @file  Wifi_DebugOutput.h
 * 
 * @brief Converts assorted Wifi-related enums to text for debug output.
 */


const char* nmStateString(NMState state);

const char * nmConnectivityStateString(NMConnectivityState state);

const char * deviceStateString(NMDeviceState state);

const char * deviceStateReasonString(NMDeviceStateReason stateReason);
   
const char * activeConnectionStateString(NMActiveConnectionState state);
