#include "LibNM/NMObjects/WifiSecuritySettings.h"

/*
 * Creates a WifiSecuritySettings object sharing data with another
 * WifiSecuritySettings object.
 */
LibNM::WifiSecuritySettings::WifiSecuritySettings
(const WifiSecuritySettings& toCopy)
{
}

/*
 * Creates a WifiSecuritySettings object to hold existing LibNM settings data.
 */
LibNM::WifiSecuritySettings::WifiSecuritySettings
(NMSettingWirelessSecurity* toAssign)
{
}

/*
 * Creates a WifiSecuritySettings object holding a new, empty LibNM security
 * settings object.
 */
LibNM::WifiSecuritySettings::WifiSecuritySettings()
{
}

/*
 * Adds WPA security settings to this settings object.
 */
void LibNM::WifiSecuritySettings::addWPASettings(const juce::String psk)
{
}

/*
 * Adds WEP security settings to this settings object.
 */
void LibNM::WifiSecuritySettings::addWEPSettings(const juce::String psk)
{
}

/*
 * Gets the basic security type defined by this settings object.
 */
SecurityType LibNM::WifiSecuritySettings::getSecurityType() const
{
}