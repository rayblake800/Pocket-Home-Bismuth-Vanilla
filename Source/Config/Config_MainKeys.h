#pragma once
#include "Config_DataKey.h"

/**
 * @file  Config_MainKeys.h
 *
 * @brief  Provides the keys for all parameters in the main.json configuration
 *         file.
 */
namespace Config
{
    namespace MainKeys
    {
        //###################### Integer value keys: ###########################
        /* Sets how frequently, in milliseconds, to scan for new Wifi access 
           points while the Wifi page is open. */
        static const DataKey wifiScanFreqKey
            ("Wifi AP scan frequency", DataKey::intType);
        
        //####################### String value keys: ###########################
        /* Sets the command prefix used to launch applications in a new terminal
           window: */
        static const DataKey termLaunchCommandKey
            ("terminal launch command", DataKey::stringType);
        /* Sets the name of the wifi interface */
        static const DataKey wifiInterfaceKey
            ("Wifi interface", DataKey::stringType);
        
        //######################## Boolean value keys: #########################
        /* Sets if the cursor should be shown */
        static const DataKey showCursorKey
            ("cursor", DataKey::boolType);
        /* Sets if the HomePage clock should be shown */
        static const DataKey showClockKey
            ("show clock", DataKey::boolType);
        /* Sets if the HomePage clock should use 12 hour or 24 hour time */
        static const DataKey use24HrModeKey
            ("use 24h mode", DataKey::boolType);

        static const std::vector<DataKey> allKeys
        {
            wifiScanFreqKey,
            termLaunchCommandKey,
            wifiInterfaceKey,
            showCursorKey,
            showClockKey,
            use24HrModeKey
        };
    }
}
 
