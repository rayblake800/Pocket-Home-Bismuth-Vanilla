#pragma once
#include <nm-connection.h>
#include "JuceHeader.h"
#include "GDBusProxyInterface.h"

/**
 * @file SavedConnection.h
 * 
 * @brief Controls a NetworkManager saved connection object over DBus. 
 */

class SavedConnection : public GDBusProxyInterface
{
public:
    /**
     * Create an empty object with no linked connection.
     */
    SavedConnection();
    
    /**
     * Initialize a SavedConnection from a DBus connection path.
     * 
     * @param path A valid DBus path to a NetworkManager saved connection.
     */
    SavedConnection(const char* path);
    
    virtual ~SavedConnection() { }
    
    /**
     * Checks if this connection is a wifi connection.
     * 
     * @return true iff connection settings are for a wifi connection.
     */
    bool isWifiConnection();
    
    /**
     * Gets the NMConnection object generated from this connection's data.
     * Only wifi connections are supported, others are not guaranteed to work.
     * 
     * @return the NMConnection object for this connection, or nullptr if the
     *         connection is invalid.
     */
    NMConnection* getNMConnection();
    
    /**
     * Replace the connection's existing wifi security settings.
     * 
     * @param newSettings  A GVariant* dictionary used to construct the
     *                     replacement settings.
     */
    void updateWifiSecurity(GVariant* newSettings);
    
    /**
     * Removes any saved WEP key or WPA PSK attached to this connection.
     */
    void removeSecurityKey();
    
    /**
     * Deletes this connection from the list of saved connections.  This object
     * will be invalid after this method is called.
     */
    void deleteConnection();
    
private:
    /**
     * Create a NMConnection object using this saved connection's data.
     * Only wifi connections are supported, others are not guaranteed to work.
     */
    void createNMConnection();
    
    /**
     * Returns one of this connection's settings objects.
     * 
     * @param name  The name of the connection setting to find.
     * 
     * @return the setting with the matching name, if found. If this value is
     *         non-null, it will need to be freed with g_variant_unref. 
     */
    GVariant* getSetting(const char* name);
    
    /**
     * Returns the value of a specific property for a specific settings
     * object
     * 
     * @param settingName  The name of the connection setting to search.
     * 
     * @param propName     The name of the property to search for in the
     *                     settings object.
     * 
     * @return the matching property, if found. If this value is
     *         non-null, it will need to be freed with g_variant_unref. 
     */
    GVariant* getSettingProp(const char* settingName,
            const char* propName);
        
    /**
     * Returns the value of a specific property for a specific settings
     * object
     * 
     * @param settingsObject  A settings object extracted from this connection.
     * 
     * @param propName        The name of the property to search for in the
     *                        settings object.
     * 
     * @return the matching property, if found. If this value is
     *         non-null, it will need to be freed with g_variant_unref. 
     */
    GVariant* getSettingProp(GVariant* settingsObject,
            const char* propName);
    
    /**
     * Checks if this connection has a particular setting type.
     * Don't use this if you actually need any data from the setting, in that
     * case it's more effective to just get the setting object and check if 
     * it is null.
     * 
     * @param settingName  The name of a connection settings object to find.
     * 
     * @return true iff a settings object named settingName exists 
     */
    bool hasSetting(const char* settingName);
        
    /**
     * Checks if this connection has a specific settings property in a specific 
     * settings type. Don't use this if you actually need any data from the 
     * setting, in that case it's more effective to just get the setting object
     * and check if getSettingParam returns null.
     * 
     * @param settingName  The name of a connection settings object to find.
     * 
     * @param propName     The name of a settings property to check for.
     * 
     * @return true iff a settings object named settingName exists and contains
     *         a property called propName.
     */
    bool hasSettingProperty(const char* settingName, const char* propName);
    
    NMConnection* nmConnection = nullptr;
    String path;
};