#pragma once
#include <nm-active-connection.h>
#include "GPPObject.h"

/**
 * @file NMPPActiveConnection.h
 * 
 * @brief A RAII container and C++ interface for LibNM NMActiveConnection 
 *        objects.
 */

class NMPPActiveConnection : public GPPObject
{
public:
    /**
     * Creates a NMPPActiveConnection sharing a GObject with an existing
     * NMPPActiveConnection.
     * 
     * @toCopy  An existing connection object.
     */
    NMPPActiveConnection(const NMPPActiveConnection& toCopy);
    
    /**
     * Creates a NMPPActiveConnection to contain a NMActiveConnection object.
     * 
     * @toAssign  A valid NMActiveConnection for this NMPPActiveConnection to 
     *            hold.
     */
    NMPPActiveConnection(NMActiveConnection* toAssign);
    
    /**
     * Creates a null NMPPActiveConnection.
     */
    NMPPActiveConnection() { }
    
    virtual ~NMPPActiveConnection() { }
    
    /**
     * Get the connection path stored by this object.
     * 
     * @return  the connection's stored path, or the empty string if this object 
     *          is null.
     */
    const char* getPath() const;

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
    
private:
    /**
     * Get the GType of the stored NMConnection object.
     * 
     * @return NM_TYPE_ACTIVE_CONNECTION
     */
    GType getType() const override;
    
    /**
     * Check if a GObject's type allows it to be held by this object. 
     * 
     * @param toCheck  Any valid GObject, or nullptr.
     * 
     * @return  true iff toCheck is a NMActiveConnection or is null. 
     */
    virtual bool isValidType(GObject* toCheck) const override;
};