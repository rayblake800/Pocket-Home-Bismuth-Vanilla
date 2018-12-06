#pragma once
#include "LibNM/NMObjects/Object.h"
#include "LibNM/NMObjects/AccessPoint.h"
#include "LibNM/NMObjects/Connection.h"
#include <nm-active-connection.h>

/**
 * @file LibNM/ActiveConnection.h
 * 
 * @brief A RAII container and C++ interface for LibNM NMActiveConnection 
 *        objects.
 *
 *  ActiveConnection manages a NMActiveConnection object, used to represent an
 * active network connection. ActiveConnection provides information about the
 * connection's status, its properties, and the access point used to activate
 * the connection.
 */
class LibNM::ActiveConnection : public LibNM::Object
{
public:
    /**
     * @brief  Creates a ActiveConnection sharing a GObject with an existing
     *         ActiveConnection.
     * 
     * @toCopy  An existing connection object.
     */
    ActiveConnection(const ActiveConnection& toCopy);
    
    /**
     * @brief  Creates a ActiveConnection to contain a NMActiveConnection 
     *         object.
     * 
     * @toAssign  A valid NMActiveConnection for this ActiveConnection to 
     *            hold.
     */
    ActiveConnection(NMActiveConnection* toAssign);
    
    /**
     * @brief  Creates a null ActiveConnection.
     */
    ActiveConnection();
    
    virtual ~ActiveConnection() { }
    
    /**
     * @brief  Gets the path of the access point used to activate this connection.
     * 
     * @return  The DBus path of a NMAccessPoint object, or the empty string if
     *          this object is null.
     */
    const char* getAccessPointPath() const;
    
    /**
     * @brief  Checks if an access point object is linked to this active 
     *         connection.
     * 
     * @param accessPoint  A wifi access point object.
     * 
     * @return             Whether the access point's path matches the 
     *                     connection's access point path, or false if this
     *                     object or the access point are null.
     */
    bool isConnectedAccessPoint(const AccessPoint& accessPoint) const;

    /**
     * @brief  Gets a unique ID string for this connection.
     * 
     * @return  The connection's UUID string, or the empty string if this object
     *          is null.
     */
    const char* getUUID() const;
    
    /**
     * @brief  Gets the connection's NetworkManager ID string.
     * 
     * @return  The connection's ID string, or the empty string if this object
     *          is null.
     */
    const char* getID() const; 
    
    /**
     * @brief  Gets this object's active connection state.
     * 
     * @return  The connection state if this object holds an active connection,
     *          NM_ACTIVE_CONNECTION_STATE_UNKNOWN otherwise.
     */
    NMActiveConnectionState getConnectionState() const;
};
