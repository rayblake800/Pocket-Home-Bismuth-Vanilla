#pragma once
#include "LibNM/LibNM.h"
#include "LibNM/AccessPoint.h"
#include "LibNM/SavedConnection.h"
#include "GLib/DBus/DBusProxy.h"

/**
 * @file LibNM/SavedConnectionLoader.h
 * 
 * @brief Connects to NetworkManager to read saved network connections.
 * 
 *  Only wifi connections are supported at this time. The only role of this
 * class is to read in saved connections as SavedConnection objects, and return
 * all wifi connections.
 */

class LibNM::SavedConnectionLoader : public GLib::DBusProxy
{
public:
    SavedConnectionLoader();
    
    virtual ~SavedConnectionLoader() { }
    
    /**
     * @brief  Reads all connection paths from NetworkManager, and returns all 
     *         the wifi connections as SavedConnection objects.
     * 
     * @return  All saved wifi connections.
     */
    juce::Array<SavedConnection> getWifiConnections() const;
    
    /**
     * @brief  Checks saved connection paths to see if one exists at the given 
     *         path.
     * 
     * @param connectionPath  A DBus path to check for a connection.
     * 
     * @return                Whether connectionPath is a valid path to a saved 
     *                        connection. 
     */
    bool connectionExists(const juce::String& connectionPath) const;
    
    /**
     * @brief  Finds a saved connection from its path.  
     *  
     *  If no matching connection is already loaded, the saved connection list 
     * will be updated in case the requested connection was recently added.
     * 
     * @param connectionPath  The DBus path of a saved network connection.
     * 
     * @return                The connection at the given path, or a null object
     *                        if no connection was found. 
     */
    SavedConnection getConnection(const juce::String& connectionPath);
    
    /**
     * @brief  Finds all saved connections that are compatible with a given wifi
     *         access point.
     * 
     * @param accessPoint  A wifi access point to check against the list of
     *                     saved connections.
     * 
     * @return             The list of all saved connections that could be 
     *                     activated with the given access point.
     */
    juce::Array<SavedConnection> findConnectionsForAP
    (const AccessPoint& accessPoint) const;
    
    /**
     * @brief  Checks the list of saved connections against an updated 
     *         connection path list, adding any new connections and removing any
     *         deleted connections.
     */
    void updateSavedConnections();

private:
    /**
     * @brief  Gets the list of all available connection paths
     * 
     * @return  The list of paths, freshly updated over the DBus interface.
     */
    inline juce::StringArray getConnectionPaths() const;
    
    /* All loaded saved connections */
    juce::Array<SavedConnection> connectionList;   

    /* Paths to all saved connections, in the same order as connectionList */
    juce::StringArray connectionPaths;
};
