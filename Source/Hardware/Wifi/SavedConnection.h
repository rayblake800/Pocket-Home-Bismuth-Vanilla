#pragma once
#include "JuceHeader.h"
#include "NMPPConnection.h"
#include "GPPDBusProxy.h"

/**
 * @file SavedConnection.h
 * 
 * @brief Controls a NetworkManager saved connection object over DBus. 
 */

class SavedConnection : public GPPDBusProxy
{
public:
    /**
     * Create an empty object with no linked connection.
     */
    SavedConnection();
    
    /**
     * Create an object from an existing DBus Connection proxy.
     * 
     * @param toCopy  Existing connection object to copy.
     */
    SavedConnection(const SavedConnection& toCopy);
    
    /**
     * Initialize a SavedConnection from a DBus connection path.
     * 
     * @param path A valid DBus path to a NetworkManager saved connection.
     */
    SavedConnection(const char* path);
    
    virtual ~SavedConnection() { }
    
    /**
     * Gets the connection's DBus path.
     * 
     * @return the DBus path used to create this object, or the empty string if
     *         the connection is not valid.
     */
    const String& getPath() const;
    
    /**
     * Checks if this connection is a wifi connection.
     * 
     * @return true iff connection settings are for a wifi connection.
     */
    bool isWifiConnection() const;
    
    /**
     * Gets the NMPPConnection object generated from this connection's data.
     * Only wifi connections are supported, others are not guaranteed to work.
     * 
     * @return the NMPPConnection object for this connection, or nullptr if the
     *         connection is invalid.
     */
    NMPPConnection getNMConnection() const;
    
    /**
     * Replace the connection's existing wifi security settings.
     * 
     * @param newSettings  A GVariant* dictionary used to construct the
     *                     replacement settings.  Only the keys found in this
     *                     dictionary will be replaced, any others will be 
     *                     copied over from the old wireless security settings.
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
    
    /**
     * Compare SavedConnections using the connection path.
     * 
     * @param rhs  A connection to compare with this object.
     * 
     * @return  true iff this connection and rhs share a DBus path.
     */
    bool operator==(const SavedConnection& rhs) const;
    
    /**
     * Compare SavedConnections with NMConnections using the connection path.
     * 
     * @param rhs  A connection to compare with this object.
     * 
     * @return  true iff this connection and rhs share a DBus path.
     */
    bool operator==(NMConnection* rhs) const;
    
private:
    /**
     * Create a NMPPConnection object using this saved connection's data.
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
     * 
     * @param settingName  The name of a connection settings object to find.
     * 
     * @return true iff a settings object named settingName exists 
     */
    bool hasSetting(const char* settingName) const;
        
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
    
    NMPPConnection nmConnection;
    StringArray settingNames;
    String path;
};
