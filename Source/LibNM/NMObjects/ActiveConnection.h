#pragma once
#include <nm-active-connection.h>
#include "LibNM/LibNM.h"
#include "LibNM/AccessPoint.h"
#include "LibNM/Connection.h"
#include "GLib/Object.h"

/**
 * @file LibNM/ActiveConnection.h
 * 
 * @brief A RAII container and C++ interface for LibNM NMActiveConnection 
 *        objects.
 */

class LibNM::ActiveConnection : public GLib::Object
{
public:
    /**
     * Creates a ActiveConnection sharing a GObject with an existing
     * ActiveConnection.
     * 
     * @toCopy  An existing connection object.
     */
    ActiveConnection(const ActiveConnection& toCopy);
    
    /**
     * Creates a ActiveConnection to contain a NMActiveConnection object.
     * 
     * @toAssign  A valid NMActiveConnection for this ActiveConnection to 
     *            hold.
     */
    ActiveConnection(NMActiveConnection* toAssign);
    
    /**
     * Creates a null ActiveConnection.
     */
    ActiveConnection();
    
    virtual ~ActiveConnection() { }
    
    /**
     * Get the connection path stored by this object.
     * 
     * @return  the connection's stored path, or the empty string if this object 
     *          is null.
     */
    const char* getPath() const;
    
    /**
     * Gets the path of the access point used to activate this connection.
     * 
     * @return  the DBus path of a NMAccessPoint object, or the empty string if
     *          this object is null.
     */
    const char* getAccessPointPath() const;
    
    /**
     * Checks if an access point object is linked to this active connection.
     * 
     * @param accessPoint  A wifi access point object.
     * 
     * @return  true if the access point's path matches the connection's access
     *          point path, false if there is no match, this object is null,
     *          or the access point is null.
     *          
     */
    bool isConnectedAccessPoint(const AccessPoint& accessPoint) const;

    /**
     * Get a unique ID string for this connection.
     * 
     * @return the connection's UUID string, or the empty string if this object
     *         is null.
     */
    const char* getUUID() const;
    
    /**
     * Get the connection's NetworkManager ID string.
     * 
     * @return the connection's ID string, or the empty string if this object
     *         is null.
     */
    const char* getID() const; 
    
    /**
     * Get this object's active connection state.
     * 
     * @return  the connection state if this object holds an active connection,
     *          NM_ACTIVE_CONNECTION_STATE_UNKNOWN otherwise.
     */
    NMActiveConnectionState getConnectionState() const;
};
