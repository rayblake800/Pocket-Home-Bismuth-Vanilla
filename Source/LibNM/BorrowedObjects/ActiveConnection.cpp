#include "LibNM/BorrowedObjects/ActiveConnection.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/ContextTest.h"
    
/*
 * Creates a null ActiveConnection.
 */
LibNM::ActiveConnection::ActiveConnection() : 
    GLib::Borrowed::Object(NM_TYPE_ACTIVE_CONNECTION) { }
    
/*
 * Gets the path of the access point used to activate this connection.
 */
const char* LibNM::ActiveConnection::getAccessPointPath() const
{
    ASSERT_NM_CONTEXT;
    const char* path = "";
    if(!isNull())
    {
        path = nm_active_connection_get_specific_object(getNMObjectPtr());
        if(path == nullptr)
        {
            path = "";
        }
    }
    return path;
}

/*
 * Checks if an access point object is linked to this active connection.
 */
bool LibNM::ActiveConnection::isConnectedAccessPoint
(const AccessPoint& accessPoint) const
{
    ASSERT_NM_CONTEXT;
    if(!isNull() || accessPoint.isNull())
    {
        return false;
    }
    const char* apPath = getAccessPointPath();
    return strcmp(apPath, accessPoint.getPath()) == 0
            && strcmp(apPath, "") != 0; 
}

/*
 * Get a unique ID string for this connection.
 */
const char* LibNM::ActiveConnection::getUUID() const
{
    ASSERT_NM_CONTEXT;
    const char* uuid = "";
    if(!isNull())
    {
        uuid = nm_active_connection_get_uuid(getNMObjectPtr());
    }
    return uuid;
}

/*
 * Gets the connection's NetworkManager ID string.
 */
const char* LibNM::ActiveConnection::getID() const
{
    ASSERT_NM_CONTEXT;
    const char* conId = "";
    if(!isNull())
    {
        conId = nm_active_connection_get_id(getNMObjectPtr());
    }
    return conId;
}

/*
 * Gets this object's active connection state.
 */
NMActiveConnectionState LibNM::ActiveConnection::getConnectionState() const
{
    ASSERT_NM_CONTEXT;
    NMActiveConnectionState state = NM_ACTIVE_CONNECTION_STATE_UNKNOWN;
    if(!isNull())
    {
        state = nm_active_connection_get_state(getNMObjectPtr());
    }
    return state;
}

/*
 * Gets the object's stored LibNM object data.
 */
NMActiveConnection* LibNM::ActiveConnection::getNMObjectPtr() const
{
    return NM_ACTIVE_CONNECTION(getGObject());
}
