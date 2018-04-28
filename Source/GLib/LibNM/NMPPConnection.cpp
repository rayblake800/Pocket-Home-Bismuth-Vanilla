#include "NMPPConnection.h"

/*
 * Create a NMPPConnection sharing a GObject with an existing
 * NMPPConnection.
 */
NMPPConnection::NMPPConnection(const NMPPConnection& toCopy) :
GPPObject(static_cast<const GPPObject&>(toCopy)) { }

/**
 * Create a NMPPConnection to contain a NMConnection object.
 */
NMPPConnection::NMPPConnection(NMConnection* toAssign) :
GPPObject(G_OBJECT(toAssign)) { }

/**
 * Create a void NMPPConnection.
 */
NMPPConnection::NMPPConnection() :
GPPObject() { }

/**
 * Check if this connection object and another could be describing the 
 * same network connection.
 */
bool NMPPConnection::connectionMatches(const NMPPConnection& rhs) const
{
    if(isVoid())
    {
        return rhs.isVoid();
    }
    else if(rhs.isVoid())
    {
        return false;
    }
    bool compatible = false;
    callInMainContext([this, &rhs, &compatible](GObject* conObj)
    {
        GObject* rhsObj = rhs.getGObject();
        if(conObj == rhsObj)
        {
            compatible = true;
        }
        NMConnection* self = NM_CONNECTION(conObj);
        NMConnection* other = NM_CONNECTION(rhsObj);
        else if(self != nullptr && other != nullptr)
        {
            compatible = nm_connection_compare(self, other,
                    NM_SETTING_COMPARE_FLAG_FUZZY);
        }
        if(other != nullptr)
        {
            g_object_unref(G_OBJECT(other));
        }
    });
    return compatible;
}
    
/*
 * Add a new connection setting to this connection.  If the connection is
 * void, this will create a new NMConnection object.
 */
void NMPPConnection::addSetting(NMSetting* setting)
{
    if(isVoid())
    {
        assignData(nm_connection_new());
    }
    callInMainContext([this, setting](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            nm_connection_add_setting(connection, setting);
        }
    });
}

/*
 * Remove one of the connection settings from this connection.
 */
void NMPPConnection::removeSetting(GType settingType)
{
    callInMainContext([this, settingType](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            nm_connection_remove_setting(connection, settingType);
        }
    });
}

/*
 * Get one of this connection's setting objects.
 */
NMSetting* NMPPConnection::getSetting(GType settingType) const
{
    NMSetting* setting = nullptr;
    callInMainContext([this, &setting, &settingType](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            setting = nm_connection_get_setting(connection, settingType);
        }
    });
    return setting;
}

/*
 * Check the validity of this connection.
 */
bool NMPPConnection::verify(GError** error) const
{
    bool isValid = false;
    callInMainContext([this, &isValid, error](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            isValid = nm_connection_verify(connection, error);
        }
    });
    return isValid;
}

/*
 * Set the connection path stored by this object.  If the connection is void,
 * this will create a new NMConnection object.
 */
void NMPPConnection::setPath(const char* path)
{
    if(isVoid())
    {
        assignData(nm_connection_new());
    }
    callInMainContext([this, path](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            nm_connection_set_path(conRef, path);
        }
    });
}

/*
 * Get the connection path stored by this object.
 */
const char* NMPPConnection::getPath() const
{
    const char* path = nullptr;
    callInMainContext([this, &path](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            path = nm_connection_get_path(connection);
        }
    });
    return path;
}

/*
 * Get a unique ID string for this connection.
 */
const char* NMPPConnection::getUUID() const
{
    const char* uuid = nullptr;
    callInMainContext([this, &uuid](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            uuid = nm_connection_get_uuid(conRef);
        }
    });
    return uuid;
}

/*
 * Get the connection's NetworkManager ID string.
 */
const char* NMPPConnection::getID() const
{
    const char* conId = nullptr;
    callInMainContext([this, &conId](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            conId = nm_connection_get_id(connection);
        }
    });
    return conId;
}

/*
 * Checks if the connection held by this object is an active connection.
 */
bool NMPPConnection::isActive() const
{
    bool active = false;
    callInMainContext([this, &active](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            active = NM_IS_ACTIVE_CONNECTION(conRef);
        }
    });
    return active;
}

/*
 * Get this object's active connection state.
 */
NMActiveConnectionState NMPPConnection::getConnectionState() const
{
    NMActiveConnectionState state = NM_ACTIVE_CONNECTION_STATE_UNKNOWN;
    callInMainContext([this, &state](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr && NM_IS_ACTIVE_CONNECTION(conRef))
        {
            state = nm_active_connection_get_state
                    (NM_ACTIVE_CONNECTION(connection));
        }
    });
    return state;
}

/*
 * Get the GType of the stored NMConnection object.
 */
GType NMPPConnection::getType() const
{
    return NM_TYPE_CONNECTION;
}