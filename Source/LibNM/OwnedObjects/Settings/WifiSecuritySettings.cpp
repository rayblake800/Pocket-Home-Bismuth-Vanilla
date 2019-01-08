#include "LibNM/OwnedObjects/Settings/WifiSecuritySettings.h"
#include "LibNM/Data/SecurityType.h"
#include "LibNM/ContextTest.h"
#include "GLib_ObjectPtr.h"

/* Relevant key management setting strings: */
static const constexpr char* unsecuredTypeKey     = "none";
static const constexpr char* wepTypeKey           = "ieee8021x";
static const constexpr char* wpaTypeKey           = "wpa-psk";
static const constexpr char* wpaEnterpriseTypeKey = "wpa-eap";


/*
 * Creates a WifiSecuritySettings object sharing data with another
 * WifiSecuritySettings object.
 */
LibNM::WifiSecuritySettings::WifiSecuritySettings
(const WifiSecuritySettings& toCopy) : Settings(toCopy) 
{
    ASSERT_NM_CONTEXT;
}

/*
 * Creates a WifiSecuritySettings object to hold existing LibNM settings data.
 */
LibNM::WifiSecuritySettings::WifiSecuritySettings
(NMSettingWirelessSecurity* toAssign) : 
Settings(NM_SETTING(toAssign), NM_TYPE_SETTING_WIRELESS_SECURITY) 
{
    ASSERT_NM_CONTEXT;
}

/*
 * Creates a WifiSecuritySettings object holding a new, empty LibNM security
 * settings object.
 */
LibNM::WifiSecuritySettings::WifiSecuritySettings() :
Settings(NM_SETTING(nm_setting_wireless_security_new()),
        NM_TYPE_SETTING_WIRELESS_SECURITY) 
{
    ASSERT_NM_CONTEXT;
}

/*
 * Adds WPA security settings to this settings object.
 */
bool LibNM::WifiSecuritySettings::addWPASettings(const juce::String psk)
{
    ASSERT_NM_CONTEXT;
    if(psk.length() >= 8)
    {
        GLib::ObjectPtr settingsPtr(*this);
        if(settingsPtr != nullptr)
        {
            g_object_set(settingsPtr,
                NM_SETTING_WIRELESS_SECURITY_PSK,
                psk.toRawUTF8(),
                NM_SETTING_WIRELESS_SECURITY_PSK_FLAGS,
                NM_SETTING_SECRET_FLAG_NONE, nullptr);
            return true;
        }
    }
    else //Invalid psk!
    {
        jassertfalse;
    }
    return false;
}

/**
 * @brief  Casts a GLib::ObjectPtr holding the connection settings data to 
 *         the NMSettingWirelessSecurity* type.
 *
 * @param settingsPtr  A RAII pointer object storing the settings object's 
 *                     GObject* value.
 *
 * @return             The value stored in settingsPtr as a 
 *                     NMSettingWirelessSecurity*, or nullptr if settingsPtr 
 *                     does not hold a NMSettingWirelessSecurity*.
 */
static NMSettingWirelessSecurity* toNMSecuritySettings
(GLib::ObjectPtr& settingPtr)
{
    return NM_SETTING_WIRELESS_SECURITY((GObject*) settingPtr);
}

/*
 * Adds WEP security settings to this settings object.
 */
bool LibNM::WifiSecuritySettings::addWEPSettings(const juce::String psk)
{
    ASSERT_NM_CONTEXT;
    const char* keyType = nullptr;
    if (psk.length() == 10 || psk.length() == 26)
    {
        keyType = (const char*) NM_WEP_KEY_TYPE_KEY;
    }
    //valid passphrase format: length 5 or length 14
    else if (psk.length() == 5 || psk.length() == 13)
    {
        keyType = (const char*) NM_WEP_KEY_TYPE_PASSPHRASE;
    }
    else
    {
        DBG("Connection::" << __func__
                << ": Invalid WEP Key type, "
                << "psk.length() = " << psk.length()
                << ", not in [5,10,13,26]");
        jassertfalse;
        return false;
    }
    
    GLib::ObjectPtr settingsPtr(*this);
    if(settingsPtr != nullptr)
    {
        g_object_set(settingsPtr,
                NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE,
                keyType,
                NM_SETTING_WIRELESS_SECURITY_PSK_FLAGS,
                NM_SETTING_SECRET_FLAG_NONE,  nullptr);

        nm_setting_wireless_security_set_wep_key(
                toNMSecuritySettings(settingsPtr), 0,
                psk.toRawUTF8());
        return true;
    }
    return false;
}


/*
 * Gets the basic security type defined by this settings object.
 */
LibNM::SecurityType LibNM::WifiSecuritySettings::getSecurityType() const
{
    ASSERT_NM_CONTEXT;
    GLib::ObjectPtr settingsPtr(*this);
    if(settingsPtr != nullptr)
    {
        juce::String securityType;
        if(securityType == wepTypeKey)
        {
            return SecurityType::securedWEP;
        }
        if(securityType == wpaTypeKey)
        {
            return SecurityType::securedWPA;
        }
        if(securityType == wpaEnterpriseTypeKey)
        {
            return SecurityType::securedRSN;
        }
    }
    return SecurityType::unsecured;
}
