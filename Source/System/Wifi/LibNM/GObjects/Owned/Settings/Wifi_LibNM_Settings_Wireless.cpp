#include "Wifi_LibNM_Settings_Wireless.h"
#include "Wifi_LibNM_SSID.h"
#include "Wifi_LibNM_APMode.h"
#include "GLib_ObjectPtr.h"

namespace WifiNMSettings = Wifi::LibNM::Settings;

// Creates a Wireless object to hold existing wireless settings data.
WifiNMSettings::Wireless::Wireless(NMSettingWireless* toAssign) :
Object(NM_SETTING(toAssign), NM_TYPE_SETTING_WIRELESS) { }


// Creates a new Wireless object holding a new, empty LibNM wireless settings
// object.
WifiNMSettings::Wireless::Wireless() :
Object(NM_SETTING(nm_setting_wireless_new()), NM_TYPE_SETTING_WIRELESS) { }

/**
 * @brief  Casts a GLib::ObjectPtr holding the connection settings data to the
 *         NMSettingWireless* type.
 *
 * @param settingsPtr  A RAII pointer object storing the settings object's
 *                     GObject* value.
 *
 * @return             The value stored in settingsPtr as a NMSettingWireless*,
 *                     or nullptr if settingsPtr does not hold a
 *                     NMSettingWireless*.
 */
static NMSettingWireless* toNMWireless(GLib::ObjectPtr& settingPtr)
{
    return NM_SETTING_WIRELESS((GObject*) settingPtr);
}


// Gets the SSID saved to the wireless connection settings.
Wifi::LibNM::SSID WifiNMSettings::Wireless::getSSID() const
{
    GLib::ObjectPtr settingsPtr(*this);
    if (settingsPtr != nullptr)
    {
        return SSID(nm_setting_wireless_get_ssid(toNMWireless(settingsPtr)));
    }
    return SSID();
}


// Gets the access point mode saved to the wireless connection settings.
Wifi::LibNM::APMode WifiNMSettings::Wireless::getMode() const
{
    GLib::ObjectPtr settingsPtr(*this);
    if (settingsPtr != nullptr)
    {
        juce::String mode = nm_setting_wireless_get_mode
                (toNMWireless(settingsPtr));
        if (mode == NM_SETTING_WIRELESS_MODE_ADHOC)
        {
            return APMode::adhoc;
        }
        if (mode == NM_SETTING_WIRELESS_MODE_INFRA)
        {
            return APMode::infrastructure;
        }
        if (mode == NM_SETTING_WIRELESS_MODE_AP)
        {
            return APMode::hotspot;
        }
    }
    return APMode::unknown;
}


// Gets the list of all base ID values associated with this Wireless object's
// connection.
juce::StringArray WifiNMSettings::Wireless::getSeenBSSIDs() const
{
    juce::StringArray seenBSSIDs;
    GLib::ObjectPtr settingsPtr(*this);
    if (settingsPtr != nullptr)
    {
        unsigned int bssidCount = nm_setting_wireless_get_num_seen_bssids
                (toNMWireless(settingsPtr));
        for (int i = 0; i < bssidCount; i++)
        {
            seenBSSIDs.add(juce::String(nm_setting_wireless_get_seen_bssid
                    (toNMWireless(settingsPtr), i)));
        }
    }
    return seenBSSIDs;
}


// Checks if this object's connection is hidden.
bool WifiNMSettings::Wireless::isHidden() const
{
    GLib::ObjectPtr settingsPtr(*this);
    if (settingsPtr != nullptr)
    {
        return nm_setting_wireless_get_hidden(toNMWireless(settingsPtr));
    }
    return false;
}


// Changes the SSID saved to the Wifi settings.
void WifiNMSettings::Wireless::setSSID(const SSID newSSID)
{
    setProperty(NM_SETTING_WIRELESS_SSID, newSSID.getByteArray());
}


// Changes the access point mode saved to the Wifi settings.
void WifiNMSettings::Wireless::setMode(const APMode newMode)
{
    const char* mode;
    switch(newMode)
    {
        case APMode::unknown:
            return;
        case APMode::adhoc:
            mode = NM_SETTING_WIRELESS_MODE_ADHOC;
            break;
        case APMode::infrastructure:
            mode = NM_SETTING_WIRELESS_MODE_INFRA;
            break;
        case APMode::hotspot:
            mode = NM_SETTING_WIRELESS_MODE_AP;
    }
    setProperty(NM_SETTING_WIRELESS_MODE, mode);
}


// Sets if the connection is hidden.
void WifiNMSettings::Wireless::setHidden(const bool isHidden)
{
    setProperty(NM_SETTING_WIRELESS_HIDDEN, isHidden);
}
