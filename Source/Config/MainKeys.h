#pragma once
#include "DataKey.h"

/**
 * @file MainKeys.h
 *
 * @brief  Provides the keys for all parameters in the main.json configuration
 *         file.
 */
namespace Config
{
    namespace MainKeys
    {
        //###################### Integer value keys: ###########################
        /* Sets the maximum number of AppMenu::Scrolling rows per screen.*/
        static const DataKey scrollingMenuRowsKey
            ("scrolling AppMenu max rows", DataKey::intType);
        /* Sets the width in menu items of the paged AppMenu's grid of visible
           menu items. */
        static const DataKey pagedMenuWidthKey
            ("paged AppMenu grid width", DataKey::intType);
        /* Sets the height in menu items of the paged AppMenu's grid of visible
           menu items. */
        static const DataKey pagedMenuHeightKey
            ("paged AppMenu grid height", DataKey::intType);
        /* Sets how frequently, in milliseconds, to scan for new Wifi access 
           points while the Wifi page is open. */
        static const DataKey wifiScanFreqKey
            ("Wifi AP scan frequency", DataKey::intType);

        
        //####################### String value keys: ###########################
        /* Sets the HomePage background color or image. */
        static const DataKey backgroundKey
            ("background", DataKey::stringType);
        /* Sets the AppMenuComponent type to use on the home page. */
        static const DataKey menuTypeKey
            ("app menu type", DataKey::stringType);
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

        /* All possible values of menuTypeKey */
        static const juce::StringArray menuTypes = 
        {
            "Scrolling menu",
            "Paged menu"
        };

        static const std::vector<DataKey>& allKeys
        {
            scrollingMenuRowsKey,
            pagedMenuWidthKey,
            pagedMenuHeightKey,
            wifiScanFreqKey,
            backgroundKey,
            menuTypeKey,
            termLaunchCommandKey,
            wifiInterfaceKey,
            showCursorKey,
            showClockKey,
            use24HrModeKey
        };
    }
}
 
