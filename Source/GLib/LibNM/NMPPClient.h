#pragma once
#include <nm-client.h>
#include "NMPPConnection.h"
#include "NMPPActiveConnection.h"
#include "NMPPDeviceWifi.h"
#include "GSignalHandler.h"
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
    juce::Array<NMPPDeviceWifi> getWifiDevices() const;
    
    /**
     * Gets a specific wifi device using its interface name.
     * 
     * @param interface  The interface name of the desired wifi device.
     * 
     * @return  the requested wifi device, or a null object if no valid device
     *          is found.
     */
    NMPPDeviceWifi getWifiDeviceByIface(const char* interface) const;
        
    /**
     * Gets a specific wifi device using its DBus path.
     * 
     * @param path  The DBus path of the desired wifi device.
     * 
     * @return  the requested wifi device, or a null object if no valid device
     *          is found.
     */
    NMPPDeviceWifi getWifiDeviceByPath(const char* path) const;
    
    /**
     * Gets the list of all active connections known to the network manager.
     * 
     * @return  an array of all active network connections.
     */
    juce::Array<NMPPActiveConnection> getActiveConnections() const;
    
    /**
     * Gets the primary active network connection.
     * 
     * @return  an active NMPPConnection object, or a null object if there is no
     *          primary connection.
     */
    NMPPActiveConnection getPrimaryConnection() const;
    
    /**
     * Gets the connection being activated by the network manager.
     * 
     * @return  the activating connection as an active NMPPConnection object,
     *          or a null object if there is no activating connection.
     */
    NMPPActiveConnection getActivatingConnection() const;
    
    /**
     * Deactivates an active network connection.
     * 
     * @param activeCon  The network connection to deactivate.  If this 
     *                   connection is not active, nothing will happen.
     */
    void deactivateConnection(NMPPActiveConnection& activeCon);
    
    /**
     * Checks if wireless connections are currently enabled.
     * 
     * @return  true iff wifi is enabled.
     */
    bool wirelessEnabled() const;
    
    /**
     * Sets whether wireless connections are enabled.
     * 
     * @param enabled  If true, wifi will be enabled, if false, wifi will be
     *                 disabled.
     */
    void setWirelessEnabled(bool enabled);
    
    /**
     * ConnectionHandler objects implement methods for handling connection
     * activation attempts.  When attempting to activate a new connection,
     * a connection handler must be provided to receive the resulting new
     * connection object along with any errors that occur.
     */
    class ConnectionHandler
    {
    public:
        friend class NMPPClient;
        ConnectionHandler() { }
        virtual ~ConnectionHandler() { }
        
    private:
        /**
         * This function will be called whenever starting to activate a 
         * connection succeeds.
         * 
         * @param connection  A new active connection object representing the 
         *                    added connection. This connection object might not
         *                    be completely connected yet.
         * 
         * @param isNew       True if the connection was just added to the
         *                    network manager, false if it was a known
         *                    connection that was re-activated.
         */
        virtual void openingConnection(NMPPActiveConnection connection,
                bool isNew) = 0;
        
        /**
         * This function will be called whenever starting to activate a
         * connection fails.
         * 
         * #param connection  The connection that failed to activate.  This
         *                    may be a null connection.
         * 
         * @param error       A GError object describing the problem.  This 
         *                    error object must be freed by the 
         *                    ConnectionHandler
         * 
         * @param isNew       True iff the connection was just added to the 
         *                    network manager. 
         */
        virtual void openingConnectionFailed(NMPPActiveConnection connection, 
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
         * @param handler     The connection handler object that attempted to 
         *                    activate the connection.
         */
        static void activateCallback(NMClient* client,
                NMActiveConnection* connection,
                GError* error,
                NMPPClient::ConnectionHandler* handler);
        
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
         * @param handler     The connection handler object that attempted to 
         *                    activate the connection.
         */
        static void addActivateCallback(NMClient* client,
                NMConnection* connection,
                const char* path,
                GError* error,
                NMPPClient::ConnectionHandler* handler);
    };
    
    /**
     * Activates a wifi network connection, attempting to set it as the primary
     * network connection.
     * 
     * @param connection   A network connection to activate.  If this is a new 
     *                     connection, it will be saved by the network manager.
     * 
     * @param wifiDevice   The system network device used to activate the
     *                     connection. 
     * 
     * @param accessPoint  The wifi access point used to activate the 
     *                     connection.
     * 
     * @param handler      The connectionHandler object that will receive the 
     *                     new connection object.
     * 
     * @param useSaved     If true, the client will attempt to re-activate an
     *                     existing connection.  Otherwise, a new connection
     *                     will be created even if an existing one is found.  
     */
    void activateConnection(
            const NMPPConnection& connection,
            const NMPPDeviceWifi& wifiDevice,
            const NMPPAccessPoint& accessPoint,
            ConnectionHandler* handler,
            bool useSaved = true);
    
    /**
     * Listeners receive updates whenever wireless is enabled or disabled.
     */
    class Listener : public GSignalHandler
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
        void propertyChanged(NMPPClient& source, juce::String property)
        override;  
    };
    
    /**
     * Adds a listener to this network manager client.
     * 
     * @param listener  The object that will receive updates when wireless is
     *                  enabled or disabled.
     */
    void addListener(Listener& listener);
};