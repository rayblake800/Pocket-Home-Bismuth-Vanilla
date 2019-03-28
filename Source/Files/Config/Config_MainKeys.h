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
        static const DataKey wifiScanFreq
            ("Wifi AP scan frequency", DataKey::intType);
        
        //####################### String value keys: ###########################
        /* Sets the command prefix used to launch applications in a new terminal
           window: */
        static const DataKey termLaunchCommand
            ("terminal launch command", DataKey::stringType);
        /* Sets the name of the wifi interface */
        static const DataKey wifiInterface
            ("Wifi interface", DataKey::stringType);
        
        //######################## Boolean value keys: #########################
        /* Sets if the cursor should be shown */
        static const DataKey showCursor
            ("cursor", DataKey::boolType);
        /* Sets if the HomePage clock should be shown */
        static const DataKey showClock
            ("show clock", DataKey::boolType);
        /* Sets if the HomePage clock should use 12 hour or 24 hour time */
        static const DataKey use24HrMode
            ("use 24h mode", DataKey::boolType);
        /* Sets if the IPLabel should be shown on the HomePage */
        static const DataKey showIPOnHome
            ("use IP label on home page", DataKey::boolType);
        static const DataKey showIPOnSettings
            ("use IP label on settings page", DataKey::boolType);
        static const DataKey printLocalIP
            ("IPLabel prints local IP", DataKey::boolType);
        static const DataKey printPublicIP
            ("IPLabel prints public IP", DataKey::boolType);

        static const std::vector<DataKey> allKeys
        {
            wifiScanFreq,
            termLaunchCommand,
            wifiInterface,
            showCursor,
            showClock,
            use24HrMode,
            showIPOnHome,
            showIPOnSettings,
            printLocalIP,
            printPublicIP
        };
    }
}
