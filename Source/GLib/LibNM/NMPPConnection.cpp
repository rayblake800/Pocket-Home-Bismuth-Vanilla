#include "NMPPConnection.h"

/*
 * Create a NMPPConnection sharing a GObject with an existing
 * NMPPConnection.
 */
NMPPConnection::NMPPConnection(const NMPPConnection& toCopy) :
GPPObject(toCopy, NM_TYPE_CONNECTION) { }

/**
 * Create a NMPPConnection to contain a NMConnection object.
 */
NMPPConnection::NMPPConnection(NMConnection* toAssign) :
GPPObject(G_OBJECT(toAssign), NM_TYPE_CONNECTION) { }
    
/*
 * Creates a null NMPPConnection.
 */
NMPPConnection::NMPPConnection() : GPPObject(NM_TYPE_CONNECTION) { }

/**
 * Check if this connection object and another could be describing the 
 * same network connection.
 */
bool NMPPConnection::connectionMatches(const NMPPConnection& rhs) const
{
    if(isNull())
    {
        return rhs.isNull();
    }
    else if(rhs.isNull())
    {
        return false;
    }
    bool compatible = false;
    callInMainContext([this, &rhs, &compatible](GObject* conObj)
    {
        GObject* rhsObj = rhs.getGObject();
        if(conObj == rhsObj)
        {
            compatible = true;
        }
        else
        {
            NMConnection* self = NM_CONNECTION(conObj);
            NMConnection* other = NM_CONNECTION(rhsObj);
            if(self != nullptr && other != nullptr)
            {
                compatible = nm_connection_compare(self, other,
                        NM_SETTING_COMPARE_FLAG_FUZZY);
            }
        }
        if(rhsObj != nullptr)
        {
            g_object_unref(rhsObj);
        }
    });
    return compatible;
}
   

/*
 * Add a new connection setting to this connection.  If the connection is
 * null, this will create a new NMConnection object.
 */
void NMPPConnection::addSetting(NMSetting* setting)
{
    if(isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    callInMainContext([this, setting](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            nm_connection_add_setting(connection, setting);
        }
    });
}

/*
 * Remove one of the connection settings from this connection.
 */
void NMPPConnection::removeSetting(GType settingType)
{
    callInMainContext([this, settingType](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            nm_connection_remove_setting(connection, settingType);
        }
    });
}
    
/*
 * Adds new wireless connection settings to this connection.  If this
 * connection is null, this will initialize it with a new NMConnection
 * object.
 */
void NMPPConnection::addWifiSettings(const GByteArray* ssid, bool isHidden)
{
    if(ssid != nullptr)
    {   
        NMSettingWireless* wifiSettings
                = (NMSettingWireless*) nm_setting_wireless_new();
        g_object_set(wifiSettings,
                NM_SETTING_WIRELESS_SSID,
                ssid,
                NM_SETTING_WIRELESS_HIDDEN,
                isHidden,
                nullptr);
        addSetting(NM_SETTING(wifiSettings));
    }
}

/*
 * Attempts to add WPA security settings to this connection.
 */
bool NMPPConnection::addWPASettings(const juce::String& psk)
{
    if(psk.length() < 8)
    {
        return false;
    }
    NMSettingWirelessSecurity* securitySettings
            = (NMSettingWirelessSecurity*) nm_setting_wireless_security_new();
    g_object_set(G_OBJECT(securitySettings),
            NM_SETTING_WIRELESS_SECURITY_PSK,
            psk.toRawUTF8(),
            NM_SETTING_WIRELESS_SECURITY_PSK_FLAGS,
            NM_SETTING_SECRET_FLAG_NONE, nullptr);
    addSetting(NM_SETTING(securitySettings));
    return true;
}

/*
 * Attempts to add WEP security settings to this connection.
 */
bool NMPPConnection::addWEPSettings(const juce::String& psk)
{
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
        DBG("NMPPConnection::" << __func__
                << ": Invalid WEP Key type, "
                << "psk.length() = " << psk.length()
                << ", not in [5,10,13,26]");
        return false;
    }
    NMSettingWirelessSecurity* securitySettings
            = (NMSettingWirelessSecurity*) nm_setting_wireless_security_new();
    g_object_set(G_OBJECT(securitySettings),
            NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE,
            keyType,
            NM_SETTING_WIRELESS_SECURITY_PSK_FLAGS,
            NM_SETTING_SECRET_FLAG_NONE,  nullptr);
    nm_setting_wireless_security_set_wep_key
            (securitySettings, 0, psk.toRawUTF8());
    addSetting(NM_SETTING(securitySettings));
    return true;
}


/*
 * Get one of this connection's setting objects.
 */
NMSetting* NMPPConnection::getSetting(GType settingType) const
{
    NMSetting* setting = nullptr;
    callInMainContext([this, &setting, &settingType](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            setting = nm_connection_get_setting(connection, settingType);
        }
    });
    return setting;
}

/*
 * Check the validity of this connection.
 */
bool NMPPConnection::verify(GError** error) const
{
    bool isValid = false;
    callInMainContext([this, &isValid, error](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            isValid = nm_connection_verify(connection, error);
        }
    });
    return isValid;
}

/*
 * Set the connection path stored by this object.  If the connection is null,
 * this will create a new NMConnection object.
 */
void NMPPConnection::setPath(const char* path)
{
    if(isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    callInMainContext([this, path](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            nm_connection_set_path(connection, path);
        }
    });
}

/*
 * Get the connection path stored by this object.
 */
const char* NMPPConnection::getPath() const
{
    const char* path = "";
    callInMainContext([this, &path](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            path = nm_connection_get_path(connection);
            if(path == nullptr)
            {
                path = "";
            }
        }
    });
    return path;
}

/*
 * Get a unique ID string for this connection.
 */
const char* NMPPConnection::getUUID() const
{
    const char* uuid = "";
    callInMainContext([this, &uuid](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            uuid = nm_connection_get_uuid(connection);
            if(uuid == nullptr)
            {
                uuid = "";
            }
        }
    });
    return uuid;
}

/*
 * Get the connection's NetworkManager ID string.
 */
const char* NMPPConnection::getID() const
{
    const char* conId = "";
    callInMainContext([&conId](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            conId = nm_connection_get_id(connection);
            if(conId == nullptr)
            {
                conId = "";
            }
        }
    });
    return conId;
}


#ifdef JUCE_DEBUG
/*
 * Prints all stored connection data to stdout.
 */
void NMPPConnection::printDebugOutput() const
{
    callInMainContext([](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            nm_connection_dump(connection);
        }
    });
}
#endif
    
