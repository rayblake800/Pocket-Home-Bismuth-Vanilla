#pragma once
#include "GDBusProxyInterface.h"
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

class SavedConnections : public GDBusProxyInterface
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
    Array<SavedConnection> getWifiConnections();
};