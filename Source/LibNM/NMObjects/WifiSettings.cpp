#include "LibNM/NMObjects/WifiSettings.h"
#include "GLib/SmartPointers/ObjectPtr.h"
#include "LibNM/APData/SSID.h"
#include "LibNM/APData/APMode.h"

typedef GLib::ObjectPtr<NMSettingWireless*> NMSettingWirelessPtr;

/*
 * Creates a WifiSettings object that shares data with another WifiSettings
 * object.
 */
LibNM::WifiSettings::WifiSettings(const WifiSettings& toCopy) :
Settings(toCopy, NM_TYPE_SETTING_WIRELESS) { }

/*
 * Creates a WifiSettings object to hold existing wireless settings data.
 */
LibNM::WifiSettings::WifiSettings(NMSettingWireless* toAssign) :
Settings(NM_SETTING(toAssign), NM_TYPE_SETTING_WIRELESS) { }

/*
 * Creates a new WifiSettings object holding a new, empty LibNM wireless
 * settings object.
 */
LibNM::WifiSettings::WifiSettings() :
Settings(NM_SETTING(nm_setting_wireless_new()), NM_TYPE_SETTING_WIRELESS) { }

/*
 * Gets the SSID saved to the wireless connection settings.
 */
LibNM::SSID LibNM::WifiSettings::getSSID() const
{
    if(!isNull())
    {
        NMSettingWirelessPtr wifiSettings = getGObject();
        return SSID(nm_setting_wireless_get_ssid(wifiSettings);
    }
}

/*
 * Gets the access point mode saved to the wireless connection settings.
 */
APMode LibNM::WifiSettings::getMode() const
{
    NMSettingWirelessPtr wifiSettings = getGObject();
    const char* mode = n
}

/*
 * Gets the list of all base ID values associated with this WifiSettings
 * object's connection.
 */
juce::StringArray LibNM::WifiSettings::getSeenBSSIDs() const
{
}

/*
 * Checks if this object's connection is hidden.
 */
bool LibNM::WifiSettings::isHidden() const
{
}

/*
 * Changes the SSID saved to tbe Wifi settings.
 */
void LibNM::WifiSettings::setSSID(const SSID newSSID)
{
}

/*
 * Changes the access point mode saved to the Wifi settings.
 */
void LibNM::WifiSettings::setMode(const APMode newMode)
{
}

/*
 * Sets if the connection is hidden.
 */
void LibNM::WifiSettings::setHidden(const bool isHidden)
{
}
