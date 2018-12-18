#include "LibNM/NMObjects/ActiveConnection.h"
#include "LibNM/NMObjects/AccessPoint.h"
#include "GLib/SmartPointers/ObjectPtr.h"

/* Rename smart pointer for brevity: */
typedef GLib::ObjectPtr<NMActiveConnection*> NMActiveConnectionPtr;

/*
 * Creates a ActiveConnection sharing a GObject with an existing
 * ActiveConnection.
 */
LibNM::ActiveConnection::ActiveConnection(const ActiveConnection& toCopy) :
LibNM::Object(toCopy, NM_TYPE_ACTIVE_CONNECTION)
{ 
    ASSERT_CORRECT_CONTEXT;
}
 
/*
 * Creates a ActiveConnection to contain a NMActiveConnection object.
 */
LibNM::ActiveConnection::ActiveConnection(NMActiveConnection* toAssign) :
LibNM::Object(NM_OBJECT(toAssign), NM_TYPE_ACTIVE_CONNECTION) 
{ 
    ASSERT_CORRECT_CONTEXT;
}
    
/*
 * Creates a null ActiveConnection.
 */
LibNM::ActiveConnection::ActiveConnection() :
LibNM::Object(NM_TYPE_ACTIVE_CONNECTION) { }
    
/*
 * Gets the path of the access point used to activate this connection.
 */
const char* LibNM::ActiveConnection::getAccessPointPath() const
{
    ASSERT_CORRECT_CONTEXT;
    const char* path = "";
    NMActiveConnectionPtr connection(NM_ACTIVE_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        path = nm_active_connection_get_specific_object(connection);
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
    ASSERT_CORRECT_CONTEXT;
    if(isNull() || accessPoint.isNull())
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
    ASSERT_CORRECT_CONTEXT;
    const char* uuid = "";
    NMActiveConnectionPtr connection(NM_ACTIVE_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        uuid = nm_active_connection_get_uuid(connection);
    }
    return uuid;
}

/*
 * Gets the connection's NetworkManager ID string.
 */
const char* LibNM::ActiveConnection::getID() const
{
    ASSERT_CORRECT_CONTEXT;
    const char* conId = "";
    NMActiveConnectionPtr connection(NM_ACTIVE_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        conId = nm_active_connection_get_id(connection);
    }
    return conId;
}

/*
 * Gets this object's active connection state.
 */
NMActiveConnectionState LibNM::ActiveConnection::getConnectionState() const
{
    ASSERT_CORRECT_CONTEXT;
    NMActiveConnectionState state = NM_ACTIVE_CONNECTION_STATE_UNKNOWN;
    NMActiveConnectionPtr connection(NM_ACTIVE_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        state = nm_active_connection_get_state(connection);
    }
    return state;
}
