#include "LibNM/Settings/WifiSettings.h"
#include "LibNM/Data/SSID.h"
#include "LibNM/Data/APMode.h"
#include "LibNM/ContextTest.h"
#include "GLib/SmartPointers/ObjectPtr.h"

typedef GLib::ObjectPtr<GObject*> GObjectPtr;
typedef GLib::ObjectPtr<NMSettingWireless*> NMSettingWirelessPtr;

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

/*
 * Gets the SSID saved to the wireless connection settings.
 */
LibNM::SSID LibNM::WifiSettings::getSSID() const
{
    ASSERT_NM_CONTEXT;
    NMSettingWirelessPtr wifiSettings = NM_SETTING_WIRELESS(getGObject());
    if(wifiSettings != nullptr)
    {
        return SSID(nm_setting_wireless_get_ssid(wifiSettings));
    }
    return SSID();
}

/*
 * Gets the access point mode saved to the wireless connection settings.
 */
LibNM::APMode LibNM::WifiSettings::getMode() const
{
    ASSERT_NM_CONTEXT;
    NMSettingWirelessPtr wifiSettings = NM_SETTING_WIRELESS(getGObject());
    if(wifiSettings != nullptr)
    {
        juce::String mode = nm_setting_wireless_get_mode(wifiSettings);
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
    NMSettingWirelessPtr wifiSettings = NM_SETTING_WIRELESS(getGObject());
    if(wifiSettings != nullptr)
    {
        unsigned int bssidCount 
            = nm_setting_wireless_get_num_seen_bssids(wifiSettings);
        for(int i = 0; i < bssidCount; i++)
        {
            seenBSSIDs.add(juce::String(nm_setting_wireless_get_seen_bssid
                        (wifiSettings, i)));
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
    NMSettingWirelessPtr wifiSettings = NM_SETTING_WIRELESS(getGObject());
    if(wifiSettings != nullptr)
    {
        return nm_setting_wireless_get_hidden(wifiSettings);
    }
    return false;
}

/*
 * Changes the SSID saved to tbe Wifi settings.
 */
void LibNM::WifiSettings::setSSID(const SSID newSSID)
{
    ASSERT_NM_CONTEXT;
    GObjectPtr settings = getGObject();
    if(settings != nullptr)
    {
        g_object_set(settings,
                NM_SETTING_WIRELESS_SSID,
                newSSID.getByteArray(),
                nullptr);
    }
}

/*
 * Changes the access point mode saved to the Wifi settings.
 */
void LibNM::WifiSettings::setMode(const APMode newMode)
{
    ASSERT_NM_CONTEXT;
    GObjectPtr settings = getGObject();
    if(settings != nullptr)
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
        g_object_set(settings,
                NM_SETTING_WIRELESS_MODE,
                mode,
                nullptr);
    }
}

/*
 * Sets if the connection is hidden.
 */
void LibNM::WifiSettings::setHidden(const bool isHidden)
{
    ASSERT_NM_CONTEXT;
    GObjectPtr settings = getGObject();
    if(settings != nullptr)
    {
        g_object_set(settings,
                NM_SETTING_WIRELESS_HIDDEN,
                isHidden,
                nullptr);
    }
}
