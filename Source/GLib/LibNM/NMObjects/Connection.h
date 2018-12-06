#pragma once
#include "LibNM/NMObjects/Object.h"
#include <nm-connection.h>

/**
 * @file  LibNM/Connection.h
 * 
 * @brief  A RAII container and C++ interface for LibNM NMConnection objects.
 *
 *  Connection objects represent a potential network connection tracked by the
 * NetworkManager. Connection objects hold all settings used by NetworkManager
 * when attempting to add an active connection. Connections are not guaranteed
 * to be complete or valid.
 *
 *  The connection interface provides methods to read, edit, and verify
 * connection properties. To actually activate a Connection, use the 
 * LibNM::Client object held by the NMThread.
 */

class LibNM::Connection : public LibNM::Object
{
public:
    /**
     * @brief  Creates a Connection sharing a GObject with an existing 
     *         Connection.
     * 
     * @toCopy  An existing connection object.
     */
    Connection(const Connection& toCopy);
    
    /**
     * @brief  Creates a Connection to contain a NMConnection object.
     * 
     * @toAssign  A valid NMConnection for this Connection to hold.
     */
    Connection(NMConnection* toAssign);
    
    /**
     * @brief  Creates a null Connection.
     */
    Connection();
    
    virtual ~Connection() { }
    
    /**
     * @brief  Checks if this connection object and another could be describing 
     *         the same network connection.
     * 
     * @param rhs  Another connection to compare with this one.
     * 
     * @return     Whether this connection object and rhs could be describing 
     *             the same network connection.
     */
    bool connectionMatches(const Connection& rhs) const;
    
    /**
     * @brief  Adds a new connection setting to this connection. 
     *
     * If the connection is null, this will initialize it with a new 
     * NMConnection object.
     * 
     * @param setting  A valid NMSetting object. Ownership of this setting
     *                 will be transferred to the NMConnection object.
     */
    void addSetting(NMSetting* setting);
    
    /**
     * @brief  Removes one of the connection settings from this connection.
     * 
     * @param settingType  A setting type to search for in the connection. If a
     *                     matching NMSetting object is found, it will be
     *                     removed and unreferenced.
     */
    void removeSetting(GType settingType);
    
    /**
     * @brief  Adds new wireless connection settings to this connection.  
     *
     * If this connection is null, this will initialize it with a new 
     * NMConnection object.
     * 
     * @param ssid      The connection's SSID byte string.
     * 
     * @param isHidden  Sets if this connection is a hidden connection.
     *                  TODO: research and support hidden connections properly.
     */
    void addWifiSettings(const GByteArray* ssid, bool isHidden = false);
    
    /**
     * @brief  Attempts to add WPA security settings to this connection.
     *
     * If this connection is null, this will initialize it with a new 
     * NMConnection object.
     *
     * @param psk   The WPA security key code. This must be at least eight
     *              characters long.
     * 
     * @return      Whether the psk was valid and the setting was added.
     */
    bool addWPASettings(const juce::String& psk);
    
    /**
     * @brief  Attempts to add WEP security settings to this connection.
     *
     * If this connection is null, this will initialize it with a new 
     * NMConnection object.
     *
     * @param  psk  The WEP security key or passphrase. This must be five, ten,
     *              thirteen, or twenty-six characters long.
     * 
     * @return      Whether the psk was valid and the setting was added.
     */
    bool addWEPSettings(const juce::String& psk);
    
    /**
     * @brief  Gets one of this connection's setting objects.
     * 
     * @param settingType  A setting type to search for in the connection.
     * 
     * @return             The requested NMSetting object if one is stored with 
     *                     this type, or nullptr if no matching setting is 
     *                     found.
     */
    NMSetting* getSetting(GType settingType) const;
    
    /**
     * @brief Checks the validity of this connection.
     * 
     * @param error  The address of a GError*, set to nullptr. If an error 
     *               occurs, a GError will be created and *error will be set to
     *               its address. If this occurs, the error will need to be
     *               cleared with g_error_clear()
     * 
     * @return  Whether this connection's settings define a valid network
     *          connection.
     */
    bool verify(GError** error) const;
    
    /**
     * @brief  Sets the connection path stored by this object.  
     *
     * If the connection is null, this will create a new NMConnection object. 
     * The new path has no effect on the actual DBus path, and the caller is 
     * entirely responsible for making sure that this is the correct path.
     * 
     * @param path  The DBus path where this connection is saved.
     */
    void setPath(const char* path);
    
    /**
     * @brief  Gets the connection path stored by this object.
     * 
     * @return  The connection's stored path, or the empty string if no path is 
     *          stored.
     */
    const char* getPath() const;

    /**
     * @brief  Gets a unique ID string for this connection.
     * 
     * @return  The connection's UUID string, or the empty string if the
     *          connection is null.
     */
    const char* getUUID() const;
    
    /**
     * @brief  Gets the connection's NetworkManager ID string.
     * 
     * @return  The connection's ID string, or the empty string if the 
     *          connection is null.
     */
    const char* getID() const;
    
#ifdef JUCE_DEBUG
    /**
     * @brief  Prints all stored connection data to stdout.
     */
    void printDebugOutput() const;
#endif
};
