#include "LibNM/OwnedObjects/Settings/WifiSettings.h"
#include "LibNM/Data/SSID.h"
#include "LibNM/Data/APMode.h"
#include "LibNM/ContextTest.h"
#include "GLib/SmartPointers/ObjectPtr.h"

/*
 * Creates a WifiSettings object that shares data with another WifiSettings
 * object.
 */
LibNM::WifiSettings::WifiSettings(const WifiSettings& toCopy) : Settings(toCopy)
{
    ASSERT_NM_CONTEXT;
}

/*
 * Creates a WifiSettings object to hold existing wireless settings data.
 */
LibNM::WifiSettings::WifiSettings(NMSettingWireless* toAssign) :
Settings(NM_SETTING(toAssign), NM_TYPE_SETTING_WIRELESS) 
{ 
    ASSERT_NM_CONTEXT;
}

/*
 * Creates a new WifiSettings object holding a new, empty LibNM wireless
 * settings object.
 */
LibNM::WifiSettings::WifiSettings() :
Settings(NM_SETTING(nm_setting_wireless_new()), NM_TYPE_SETTING_WIRELESS) 
{ 
    ASSERT_NM_CONTEXT;
}

/**
 * @brief  Casts a GLib::ObjectPtr holding the connection settings data to 
 *         the NMSettingWireless* type.
 *
 * @param settingsPtr  A RAII pointer object storing the settings object's 
 *                     GObject* value.
 *
 * @return             The value stored in settingsPtr as a 
 *                     NMSettingWireless*, or nullptr if settingsPtr 
 *                     does not hold a NMSettingWireless*.
 */
static NMSettingWireless* toNMWifiSettings(GLib::ObjectPtr& settingPtr)
{
    return NM_SETTING_WIRELESS((GObject*) settingPtr);
}

/*
 * Gets the SSID saved to the wireless connection settings.
 */
LibNM::SSID LibNM::WifiSettings::getSSID() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr settingsPtr(*this);
    if(settingsPtr != nullptr)
    {
        return SSID(nm_setting_wireless_get_ssid
                (toNMWifiSettings(settingsPtr)));
    }
    return SSID();
}

/*
 * Gets the access point mode saved to the wireless connection settings.
 */
LibNM::APMode LibNM::WifiSettings::getMode() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr settingsPtr(*this);
    if(settingsPtr != nullptr)
    {
        juce::String mode = nm_setting_wireless_get_mode
                (toNMWifiSettings(settingsPtr));
        if(mode == NM_SETTING_WIRELESS_MODE_ADHOC)
        {
            return APMode::adhoc;
        }
        if(mode == NM_SETTING_WIRELESS_MODE_INFRA)
        {
            return APMode::infrastructure;
        }
        if(mode == NM_SETTING_WIRELESS_MODE_AP)
        {
            return APMode::hotspot;
        }
    }
    return APMode::unknown;
}

/*
 * Gets the list of all base ID values associated with this WifiSettings
 * object's connection.
 */
juce::StringArray LibNM::WifiSettings::getSeenBSSIDs() const
{
    ASSERT_NM_CONTEXT;
    juce::StringArray seenBSSIDs;
    GLib::ObjectPtr settingsPtr(*this);
    if(settingsPtr != nullptr)
    {
        unsigned int bssidCount = nm_setting_wireless_get_num_seen_bssids
                (toNMWifiSettings(settingsPtr));
        for(int i = 0; i < bssidCount; i++)
        {
            seenBSSIDs.add(juce::String(nm_setting_wireless_get_seen_bssid
                    (toNMWifiSettings(settingsPtr), i)));
        }
    }
    return seenBSSIDs;
}

/*
 * Checks if this object's connection is hidden.
 */
bool LibNM::WifiSettings::isHidden() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr settingsPtr(*this);
    if(settingsPtr != nullptr)
    {
        return nm_setting_wireless_get_hidden(toNMWifiSettings(settingsPtr));
    }
    return false;
}

/*
 * Changes the SSID saved to tbe Wifi settings.
 */
void LibNM::WifiSettings::setSSID(const SSID newSSID)
{
    ASSERT_NM_CONTEXT;
    setProperty(NM_SETTING_WIRELESS_SSID, newSSID.getByteArray());
}

/*
 * Changes the access point mode saved to the Wifi settings.
 */
void LibNM::WifiSettings::setMode(const APMode newMode)
{
    ASSERT_NM_CONTEXT;
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

/*
 * Sets if the connection is hidden.
 */
void LibNM::WifiSettings::setHidden(const bool isHidden)
{
    ASSERT_NM_CONTEXT;
    setProperty(NM_SETTING_WIRELESS_HIDDEN, isHidden);
}
