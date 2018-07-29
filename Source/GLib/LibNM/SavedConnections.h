#pragma once
#include "GPPDBusProxy.h"
#include "NMPPAccessPoint.h"
#include "SavedConnection.h"

/**
 * @file SavedConnections.h
 * 
 * @brief Connects to NetworkManager to read saved network connections.
 * 
 * Only wifi connections are supported at this time.  The only role of this
 * class is to read in saved connections as SavedConnection objects, and return
 * all wifi connections.
 */

class SavedConnections : public GPPDBusProxy
{
public:
    SavedConnections();
    
    virtual ~SavedConnections() { }
    
    /**
     * Reads all connection paths from NetworkManager, and returns all the wifi
     * connections as SavedConnection objects.
     * 
     * @return all saved wifi connections.
     */
    juce::Array<SavedConnection> getWifiConnections() const;
    
    /**
     * Checks saved connection paths to see if one exists at the given path.
     * 
     * @param connectionPath  A DBus path to check for a connection.
     * 
     * @return  true iff connectionPath is a valid path to a saved connection. 
     */
    bool connectionExists(const juce::String& connectionPath) const;
    
    /**
     * Finds a saved connection from its path.  If no matching connection is
     * already loaded, the saved connection list will be updated in case the
     * requested connection was recently added.
     * 
     * @param connectionPath  The DBus path of a saved network connection.
     * 
     * @return  the connection at the given path, or a null object if no 
     *          connection was found. 
     */
    SavedConnection getConnection(const juce::String& connectionPath);
    
    /**
     * Finds all saved connections that are compatible with a given wifi
     * access point.
     * 
     * @param accessPoint  A wifi access point to check against the list of
     *                     saved connections.
     * 
     * @return  the list of all saved connections that could be activated with
     *          the given access point.
     */
    juce::Array<SavedConnection> findConnectionsForAP
    (const NMPPAccessPoint& accessPoint) const;
    
    /**
     * Check the list of saved connections against an updated connection path
     * list, adding any new connections and removing any deleted connections.
     */
    void updateSavedConnections();
private:
    
    /**
     * Get the list of all available connection paths
     * 
     * @return the list of paths, freshly updated over the DBus interface.
     */
    inline juce::StringArray getConnectionPaths();
    
    juce::Array<SavedConnection> connectionList;   
    juce::StringArray connectionPaths;
};