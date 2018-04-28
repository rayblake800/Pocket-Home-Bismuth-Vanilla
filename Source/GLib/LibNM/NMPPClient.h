#pragma once
#include <nm-client.h>
#include "NMPPConnection.h"
#include "NMPPDeviceWifi.h"
#include "GPPObject.h"

/**
 * #file NMPPClient.h
 * 
 * @brief A RAII container and C++ interface for the LibNM NMClient class.
 */

class NMPPClient : public GPPObject
{
public:
    /**
     * Create a NMPPClient holding a new NMClient object.
     */
    NMPPClient();
    
    /**
     * Create a NMPPClient that shares a NMClient with another NMPPClient.
     * 
     * @param toCopy  Holds the NMClient object that will be shared with the
     *                new NMPPClient.
     */
    NMPPClient(const NMPPClient& toCopy);
    
    /**
     * Get all wifi devices from Network Manager.
     * 
     * @return  All NMDevices found that are wifi devices, packaged into an
     *          array of NMPPDeviceWifi objects.
     */
    Array<NMPPDeviceWifi> getWifiDevices();
    
    /**
     * Gets a specific wifi device using its interface name.
     * 
     * @param interface  The interface name of the desired wifi device.
     * 
     * @return  the requested wifi device, or a void object if no valid device
     *          is found.
     */
    NMPPDeviceWifi getWifiDeviceByIface(const char* interface);
        
    /**
     * Gets a specific wifi device using its DBus path.
     * 
     * @param path  The DBus path of the desired wifi device.
     * 
     * @return  the requested wifi device, or a void object if no valid device
     *          is found.
     */
    NMPPDeviceWifi getWifiDeviceByPath(const char* path);
    
    /**
     * Gets the list of all active connections known to the network manager.
     * 
     * @return  an array of all active network connections.
     */
    Array<NMPPConnection> getActiveConnections();
    
    /**
     * Gets the primary active network connection.
     * 
     * @return  an active NMPPConnection object, or a void object if there is no
     *          primary connection.
     */
    NMPPConnection getPrimaryConnection();
    
    /**
     * Gets the connection being activated by the network manager.
     * 
     * @return  the activating connection as an active NMPPConnection object,
     *          or a void object if there is no activating connection.
     */
    NMPPConnection getActivatingConnection();
    
    /**
     * Deactivates an active network connection.
     * 
     * @param activeCon  The network connection to deactivate.  If this 
     *                   connection is not active, nothing will happen.
     */
    void deactivateConnection(const NMPPConnection& activeCon);
    
    /**
     * Checks if wireless connections are currently enabled.
     * 
     * @return  true iff wifi is enabled.
     */
    bool wirelessEnabled();
    
    /**
     * Sets whether wireless connections are enabled.
     * 
     * @param enabled  If true, wifi will be enabled, if false, wifi will be
     *                 disabled.
     */
    void setWirelessEnabled(bool enabled);
    
    /**
     * An interface for objects that can create new active connections.
     */
    class ConnectionHandler
    {
    public:
        ConnectionHandler() { }
        virtual ~ConnectionHandler();
        
        /**
         * Activate an existing network connection.
         * 
         * @param client       A valid NMPPClient object.
         * 
         * @param connection   A network connection that has been saved by the
         *                     network manager.
         * 
         * @param accessPoint  The wifi access point used to activate the
         *                     connection.
         */
        void activateConnection(
                const NMPPClient& client,
                const NMPPConnection& connection,
                const NMPPAccessPoint& accessPoint);
        
        /**
         * Add and activate a new network connection.
         * 
         * @param client       A valid NMPPClient object.
         * 
         * @param connection   A new network connection object.
         * 
         * @param wifiDevice   The system network device used to activate the
         *                     connection. 
         * 
         * @param accessPoint  The wifi access point used to activate the
         *                     connection.
         */
        void addAndActivateConnection(
                const NMPPClient& client,
                const NMPPConnection& connection,
                const NMPPDeviceWifi& wifiDevice,
                const NMPPAccessPoint& accessPoint);
        
    private:
        /**
         * This function will be called whenever starting to activate a 
         * connection succeeds.
         * 
         * @param client      The NMPPClient holding the activating connection.
         * 
         * @param connection  A new active connection object representing the 
         *                    added connection.
         * 
         * @param isNew       True iff the connection was just added to the
         *                    network manager. 
         */
        virtual void openingConnection(NMPPClient* client,
                NMPPConnection connection, bool isNew) = 0;
        
        /**
         * This function will be called whenever starting to activate a
         * connection fails.
         * 
         * @param client  The NMPPClient that failed to activate the connection.
         * 
         * @param error   A GError object describing the problem.  If this
         *                value is not null, it will need to be released with
         *                g_error_free().
         * 
         * @param isNew   True iff the connection was just added to the network
         *                manager. 
         */
        virtual void openingConnectionFailed(NMPPClient* client,
                GError* error, bool isNew) = 0;
        
        /**
         * The NMClientActivateFn called by LibNM when activating an existing
         * connection, used to create a call to openingConnection or to 
         * openingConnectionFailed.
         * 
         * @param client      The NMClient object activating the connection.
         * 
         * @param connection  The new activating connection object.
         * 
         * @param error       A GError object describing any problems, or
         *                    nullptr if nothing went wrong.
         * 
         * @param listener    The listener object that attempted to activate
         *                    the connection.
         */
        static void activateCallback(NMClient* client,
                NMActiveConnection* connection,
                GError* error,
                NMPPClient::Listener* listener);
        
        /**
         * The NMClientAddActivateFn called by LibNM when adding and activating
         * a new connection, used to create a call to openingConnection or 
         * openingConnectionFailed.
         * 
         * @param client      The NMClient object activating the connection.
         * 
         * @param connection  The new activating connection object.
         * 
         * @param path        The DBus path of the new saved connection object.
         * 
         * @param error       A GError object describing any problems, or
         *                    nullptr if nothing went wrong.
         * 
         * @param listener    The listener object that attempted to activate
         *                    the connection.
         */
        static void addActivateCallback(NMClient* client,
                NMConnection* connection,
                const char* path,
                GError* error,
                NMPPClient::Listener* listener);
    };
    
    /**
     * Listeners receive updates whenever wireless is enabled or disabled.
     */
    class Listener : public GPPObject::SignalHandler
    {
    public:
        Listener() { }
        
        virtual ~Listener() { }
        
    private:
        /**
         * This method will be called on registered listeners whenever 
         * wireless is enabled or disabled.
         * 
         * @param wifiEnabled  True if wifi was enabled, false if wifi was
         *                     disabled.
         */
        virtual void wirelessStateChange(bool wifiEnabled) = 0;
        
        /**
         * Converts generic propertyChanged calls to class-specific 
         * wirelessStateChange calls.
         * 
         * @param source    The updated NMClient object.
         * 
         * @param property  This should always be the "wireless-enabled"
         *                  property.
         */
        void propertyChanged(GPPObject* source, String property) override;  
    };
    
    /**
     * Adds a listener to this network manager client.
     * 
     * @param listener  The object that will receive updates when wireless is
     *                  enabled or disabled.
     */
    void addListener(Listener* listener);
    
    /**
     * Removes a listener from this network manager client.
     * 
     * @param listener  This object will no longer receive updates when wireless
     *                  is enabled or disabled.
     */
    void removeListener(Listener* listener);

private:
    /**
     * Gets the GType of this object's stored GObject class.
     * 
     * @return NM_TYPE_CLIENT
     */
    GType getType() const override;
    
    /**
     * Checks if a GObject's type allows it to be held by this object. 
     * 
     * @param toCheck  Any valid GObject, or nullptr.
     * 
     * @return  true iff toCheck is a NMClient or is null. 
     */
    virtual bool isValidType(GObject* toCheck) const override;
    
    /**
     * Used to re-add a list of Listeners to new GObject data.
     * 
     * @param toTransfer  A list of Listener objects to add to this
     *                    NMPPClient.
     */
    virtual void transferSignalHandlers(Array<SignalHandler*>& toTransfer);
};