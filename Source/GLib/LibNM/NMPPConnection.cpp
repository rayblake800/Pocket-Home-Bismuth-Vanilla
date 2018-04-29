#include "NMPPConnection.h"

/*
 * Create a NMPPConnection sharing a GObject with an existing
 * NMPPConnection.
 */
NMPPConnection::NMPPConnection(const NMPPConnection& toCopy)
{ 
    setGObject(toCopy);
}

/**
 * Create a NMPPConnection to contain a NMConnection object.
 */
NMPPConnection::NMPPConnection(NMConnection* toAssign)
{
    setGObject(G_OBJECT(toAssign));
}

/**
 * Check if this connection object and another could be describing the 
 * same network connection.
 */
bool NMPPConnection::connectionMatches(const NMPPConnection& rhs) const
{
    if(isNull())
    {
        return rhs.isNull();
    }
    else if(rhs.isNull())
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
        else
        {
            NMConnection* self = NM_CONNECTION(conObj);
            NMConnection* other = NM_CONNECTION(rhsObj);
            if(self != nullptr && other != nullptr)
            {
                compatible = nm_connection_compare(self, other,
                        NM_SETTING_COMPARE_FLAG_FUZZY);
            }
        }
        if(rhsObj != nullptr)
        {
            g_object_unref(rhsObj);
        }
    });
    return compatible;
}
    
/*
 * Add a new connection setting to this connection.  If the connection is
 * null, this will create a new NMConnection object.
 */
void NMPPConnection::addSetting(NMSetting* setting)
{
    if(isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
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
 * Set the connection path stored by this object.  If the connection is null,
 * this will create a new NMConnection object.
 */
void NMPPConnection::setPath(const char* path)
{
    if(isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    callInMainContext([this, path](GObject* conObj)
    {
        NMConnection* connection = NM_CONNECTION(conObj);
        if(connection != nullptr)
        {
            nm_connection_set_path(connection, path);
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
            uuid = nm_connection_get_uuid(connection);
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
 * Get the GType of the stored NMConnection object.
 */
GType NMPPConnection::getType() const
{
    return NM_TYPE_CONNECTION;
}

/*
 * Check if a GObject's type allows it to be held by this object. 
 */
bool NMPPConnection::isValidType(GObject* toCheck) const
{
    return NM_IS_CONNECTION(toCheck);
}

    