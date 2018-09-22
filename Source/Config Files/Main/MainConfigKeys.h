#pragma once
#include "ConfigKey.h"

/**
 * @file MainConfigKeys.h
 *
 * @brief  Provides the keys for all parameters in the main.json configuration
 *         file.
 */
namespace MainConfigKeys
{
    //###################### Integer value keys: ###############################
    /* Sets the maximum number of AppMenu icon rows per screen. */
    static const ConfigKey maxRowsKey
        ("max menu row count", ConfigKey::intType);
    /* Sets the maximum number of AppMenu icon columns per screen. */
    static const ConfigKey maxColumnsKey
        ("max menu column count", ConfigKey::intType);
    /* Sets how frequently, in milliseconds, to scan for new Wifi access points
       while the Wifi page is open. */
    static const ConfigKey wifiScanFreqKey
        ("Wifi AP scan frequency", ConfigKey::intType);

    
    //####################### String value keys: ###############################
    /* Sets the HomePage background color or image. */
    static const ConfigKey backgroundKey
        ("background", ConfigKey::stringType);
    /* Sets the AppMenuComponent type to use on the home page. */
    static const ConfigKey menuTypeKey
        ("app menu type", ConfigKey::stringType);
    static const ConfigKey termLaunchCommandKey
        ("terminal launch command", ConfigKey::stringType);
    /* Sets the name of the wifi interface */
    static const ConfigKey wifiInterfaceKey
        ("Wifi interface", ConfigKey::stringType);

    
    //######################## Boolean value keys: #############################
    /* Sets if the cursor should be shown */
    static const ConfigKey showCursorKey
        ("cursor", ConfigKey::boolType);
    /* Sets if the HomePage clock should be shown */
    static const ConfigKey showClockKey
        ("show clock", ConfigKey::boolType);
    /* Sets if the HomePage clock should use 12 hour or 24 hour time */
    static const ConfigKey use24HrModeKey
        ("use 24h mode", ConfigKey::boolType);

    /* All possible values of menuTypeKey */
    static const juce::StringArray menuTypes = 
    {
        "Scrolling menu",
        "Paged menu"
    };

    static const std::vector<ConfigKey>& allKeys
    {
        maxRowsKey,
        maxColumnsKey,
        backgroundKey,
        menuTypeKey,
        shutdownCommandKey,
        restartCommandKey,
        sleepCommandKey,
        termLaunchCommandKey,
        wifiInterfaceKey,
        showCursorKey,
        showClockKey,
        use24HrModeKey,
        maxRowsKey,
        maxColumnsKey
    };
};
