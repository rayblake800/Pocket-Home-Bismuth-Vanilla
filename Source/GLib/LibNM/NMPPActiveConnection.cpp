#include "NMPPActiveConnection.h"

/*
 * Creates a NMPPActiveConnection sharing a GObject with an existing
 * NMPPActiveConnection.
 */
NMPPActiveConnection::NMPPActiveConnection(const NMPPActiveConnection& toCopy)
{ 
    setGObject(toCopy);
}
 
/*
 * Creates a NMPPActiveConnection to contain a NMActiveConnection object.
 */
NMPPActiveConnection::NMPPActiveConnection(NMActiveConnection* toAssign)
{
    setGObject(G_OBJECT(toAssign));
}

/*
 * Get the connection path stored by this object.
 */
const char* NMPPActiveConnection::getPath() const
{
    const char* path = "";
    callInMainContext([this, &path](GObject* conObj)
    {
        NMObject* connection = NM_OBJECT(conObj);
        if(connection != nullptr)
        {
            path = nm_object_get_path(connection);
            if(path == nullptr)
            {
                path = "";
            }
        }
    });
    return path;
}
    
/*
 * Gets the path of the access point used to activate this connection.
 */
const char* NMPPActiveConnection::getAccessPointPath() const
{
    const char* path = "";
    callInMainContext([this, &path](GObject* conObj)
    {
        NMActiveConnection* connection = NM_ACTIVE_CONNECTION(conObj);
        if(connection != nullptr)
        {
            path = nm_active_connection_get_specific_object(connection);
            if(path == nullptr)
            {
                path = "";
            }
        }
    });
    return path;
}

/*
 * Checks if an access point object is linked to this active connection.
 */
bool NMPPActiveConnection::isConnectedAccessPoint
(const NMPPAccessPoint& accessPoint) const
{
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
const char* NMPPActiveConnection::getUUID() const
{
    const char* uuid = "";
    callInMainContext([this, &uuid](GObject* conObj)
    {
        NMActiveConnection* connection = NM_ACTIVE_CONNECTION(conObj);
        if(connection != nullptr)
        {
            uuid = nm_active_connection_get_uuid(connection);
        }
    });
    return uuid;
}

/*
 * Get the connection's NetworkManager ID string.
 */
const char* NMPPActiveConnection::getID() const
{
    const char* conId = "";
    callInMainContext([this, &conId](GObject* conObj)
    {
        NMActiveConnection* connection = NM_ACTIVE_CONNECTION(conObj);
        if(connection != nullptr)
        {
            conId = nm_active_connection_get_id(connection);
        }
    });
    return conId;
}

/*
 * Get this object's active connection state.
 */
NMActiveConnectionState NMPPActiveConnection::getConnectionState() const
{
    NMActiveConnectionState state = NM_ACTIVE_CONNECTION_STATE_UNKNOWN;
    callInMainContext([this, &state](GObject* conObj)
    {
        NMActiveConnection* connection = NM_ACTIVE_CONNECTION(conObj);
        if(connection != nullptr)
        {
            state = nm_active_connection_get_state(connection);
        }
    });
    return state;
}

/*
 * Get the GType of the stored NMConnection object.
 */
GType NMPPActiveConnection::getType() const
{
    return NM_TYPE_ACTIVE_CONNECTION;
}

/*
 * Check if a GObject's type allows it to be held by this object. 
 */
bool NMPPActiveConnection::isValidType(GObject* toCheck) const
{
    return toCheck == nullptr || NM_IS_ACTIVE_CONNECTION(toCheck);
}
