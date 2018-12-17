#pragma once
/**
 * @file  Source/LibNM/NMObjects/WifiSecuritySettings.h
 *
 * @brief  Holds wireless network connection security settings.
 */

#include "LibNM/NMObjects/Settings.h"
#include <nm-setting-wireless-security.h>

namespace LibNM { class WifiSecuritySettings; }
namespace LibNM { enum class SecurityType; }

class LibNM::WifiSecuritySettings : public Settings
{
public:
    /**
     * @brief  Creates a WifiSecuritySettings object sharing data with another
     *         WifiSecuritySettings object.
     *
     * @param toCopy  The object that will share its LibNM data with the new
     *                object.
     */
    WifiSecuritySettings(const WifiSecuritySettings& toCopy);

    /**
     * @brief  Creates a WifiSecuritySettings object to hold existing LibNM
     *         settings data.
     *
     * @param toAssign  The LibNM security settings object that the new
     *                  WifiSecuritySettings object will hold.
     */
    WifiSecuritySettings(NMSettingWirelessSecurity* toAssign);

    /**
     * @brief  Creates a WifiSecuritySettings object holding a new, empty
     *         LibNM security settings object.
     */
    WifiSecuritySettings();

    virtual ~WifiSecuritySettings() { }

    /**
     * @brief  Adds WPA security settings to this settings object.
     *
     * @param psk  The WPA key or passphrase used to secure this object's
     *             connection.
     */
    void addWPASettings(const juce::String psk);

    /**
     * @brief  Adds WEP security settings to this settings object.
     *
     * @param psk  The WEP key used to secure this object's connection.
     */
    void addWEPSettings(const juce::String psk);

    /**
     * @brief  Gets the basic security type defined by this settings object.
     *
     * @return  The type of security used by this object's connection. 
     */
    SecurityType getSecurityType() const;
};
