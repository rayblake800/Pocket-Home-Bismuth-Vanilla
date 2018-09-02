#pragma once

#include "NMPPClient.h"
#include "SavedConnections.h"
#include "WindowFocus.h"
#include "WifiStateManager.h"

/**
 * @file LibNMInterface.h
 * 
 * @brief Connects to the LibNM NetworkManager to track and control the wifi 
 * device.
 * 
 * As a WifiStateManager::NetworkInterface, only the WifiStateManager should
 * directly interact with this class.
 *
 * The NetworkManager interface is built with GLib's C-based object system.
 * To reduce the amount of messy C-style code and error-prone manual memory
 * management, NetworkManager objects are packaged into C++ wrapper objects
 * defined in Source/GLib/LibNM.
 *
 * The latest version of NetworkManager available in the Debian Jesse 
 * repositories is 0.9.10, which is considerably out of date.  Supporting
 * such an old version has required a lot of compromises and complications
 * that should be re-examined before reusing this code in any project that
 * can expect a newer NetworkManager version.
 */

class LibNMInterface : public WifiStateManager::NetworkInterface, 
        public NMPPClient::ConnectionHandler 
{
public:
    /**
     * Initializes the LibNM client interface and connects signal handlers. The
     * wifi device will be loaded from the interface name loaded from the
     * MainConfigFile, or selected automatically if the config value is empty.
     *
     * @param wifiLock  The lock used by the WifiStateManager to control access
     *                  to the network interface.  LibNMInterface will acquire
     *                  the lock when it is updated by NetworkManager.  The lock
     *                  should never be acquired within the GLib event thread.
     */
    LibNMInterface(juce::ReadWriteLock& wifiLock);

    virtual ~LibNMInterface();
protected:
    /**
     * Checks if the network manager found a valid wifi device.
     * 
     * @return  true iff a NetworkManager managed wifi device was found.
     */
    bool wifiDeviceFound() override;
    
    /**
     * Checks the NMDevice state to see if wifi is enabled.
     * 
     * @return  true iff wifi is enabled.
     */
    bool isWifiEnabled() override;

    /**
     * Checks the NMDevice state to see if wifi is connecting to an access 
     * point.
     * 
     * @return  true iff wifi is connecting.
     */
    bool isWifiConnecting() override;

    /**
     * Checks the NMDevice state to see if wifi is connected to an access point.
     * 
     * @return  true iff wifi is connected.
     */
    bool isWifiConnected() override;

    /**
     * Requests information on the active access point from the NMDevice.
     * 
     * @return a WifiAccessPoint object representing the access point connected
     *         or connecting to the wifi device, or a null access point if there
     *         is no active access point.
     */
    WifiAccessPoint getActiveAP() override;

    /**
     * Requests information on all wifi access points detected by the wifi
     * device.
     * 
     * @return  a list of all visible access points within range of the wifi
     *          device, or an empty list if wifi is disabled.
     */
    juce::Array<WifiAccessPoint> getVisibleAPs() override;

    /**
     * Begin opening a connection to a wifi access point.
     * 
     * @param toConnect  The access point that should be connected to. 
     * 
     * @param psk        The access point's security key. This will be ignored
     *                   if the access point is unsecured.
     */
    void connectToAccessPoint(const WifiAccessPoint& toConnect,
            juce::String psk = juce::String()) override;

    /**
     * Checks the wifi device list, connection state, and active and pending 
     * connections.
     */
    void updateAllWifiData();

    /**
     * Close the currently active/activating connection, if one exists.
     */
    void disconnect() override;

    /**
     * Asynchronously turns on the wifi radio.
     */
    void enableWifi() override;

    /**
     * Asynchronously turns off the wifi radio.
     */
    void disableWifi() override;
       
    /**
     * Finds the current network state of an access point object.
     * 
     * @param accessPoint  The access point to check.
     * 
     * @return  the access point's current state. 
     */
    WifiStateManager::AccessPointState getAPState
    (const WifiAccessPoint& accessPoint) override;
      
    /**
     * Finds the last time a connection was active using a specific access
     * point.
     * 
     * @param accessPoint  A wifi access point object.
     * 
     * @return  The last time a connection compatible with this access point
     *          was active, or the Unix epoch time if no compatible
     *          compatible connection has a saved timestamp.
     */
    juce::Time lastConnectionTime(const WifiAccessPoint& accessPoint) override;

private:       
    /**
     * Finds and sets active and saved connection paths for a wifi access point.
     *
     * 
     * @param accessPoint  The access point to update.  For both the access 
     *                     point's active and saved path strings, this will
     *                     attempt to find a compatible connection object and
     *                     store its path, or set the path variable to the empty
     *                     string if no matching connection is found. 
     */
    void setAccessPointPaths(WifiAccessPoint& accessPoint);
    
    /**
     * Notify listeners and save the connecting access point if starting to
     * connect.
     * 
     * @param connection  A new active connection object representing the 
     *                    added connection. This connection object will 
     *                    probably not be completely connected yet.
     * 
     * @param isNew       True if the connection was just added to the
     *                    network manager, false if it was a known
     *                    connection that was re-activated.
     */
    void openingConnection(NMPPActiveConnection connection,
                bool isNew) override;
    
    /**
     * Notify listeners that a connection attempt failed.
     * 
     * @param connection  The connection that failed to activate.  This
     *                    may be a null connection.
     * 
     * @param error       A GError object describing the problem.  This 
     *                    error object will be freed by this method.
     * 
     * @param isNew       True iff the connection was just added to the 
     *                    network manager. 
     */
    virtual void openingConnectionFailed(NMPPActiveConnection connection, 
            GError* error, bool isNew) override;
    
    //################# Network Manager Client Updates #########################
    /**
     * Called whenever wireless is enabled or disabled, this method will update
     * the wifi state accordingly.
     * 
     * @param enabled  true if wifi was enabled, false if it was disabled.
     */
    void wirelessEnabledChange(bool enabled);
    

    
    //#################### Wifi Device Updates #################################
    /**
     * This method will be called whenever the wifi device state changes.
     *
     * @param newState  The new device state value.
     *
     * @param oldState  The previous device state value.
     *
     * @param reason    The reason for the change in device state.
     */
    void stateChanged(NMDeviceState newState, NMDeviceState oldState,
            NMDeviceStateReason reason);
    
    /**
     * This method will be called whenever the wifi device detects a new
     * access point.
     * 
     * @param addedAP  The new access point detected by the wifi device.
     */
    void accessPointAdded(NMPPAccessPoint addedAP);

    /**
     * This method will be called whenever the wifi device no longer detects
     * a wifi access point.
     * 
     * @param removedAP  The nearby access point that the device can no
     *                   longer detect.
     */
    void accessPointRemoved(NMPPAccessPoint removedAP);

    /**
     * This method will be called whenever the device's active connection
     * changes.
     * 
     * @param active   The new active connection.  If the device has
     *                 disconnected, this will be a null object
     *                 (TODO: will it really be null, or just not connected?
     *                 test this)
     */
    virtual void activeConnectionChanged(NMPPActiveConnection active);
    
    /**
     *#######################  Listener Classes  ###############################
     * These objects listen to signals from NetworkManager classes and pass them
     * on to the LibNMInterface.  All GPPObject Listeners share a parent class,
     * so LibNMInterface can't just inherit all Listener classes itself.
     */
    class ClientListener : public NMPPClient::Listener
    {
    public:
        ClientListener(LibNMInterface& interface, NMPPClient& client) :
        interface(interface)
        {
            if(!client.isNull())
            {
                client.addListener(*this);
            }
        }
        virtual ~ClientListener() { }
    private:
        void wirelessStateChange(bool wifiEnabled) override
        {
            interface.wirelessEnabledChange(wifiEnabled);
        }
        LibNMInterface& interface;
    };
    
    class DeviceListener : public NMPPDeviceWifi::Listener
    {
    public:
        DeviceListener(LibNMInterface& interface, NMPPDeviceWifi& device) :
        interface(interface)
        {
            if(!device.isNull())
            {
                device.addListener(*this);
            }
        }
        virtual ~DeviceListener() { }
    private:
        void stateChanged(NMDeviceState newState, NMDeviceState oldState,
                NMDeviceStateReason reason) override
        {
            interface.stateChanged(newState, oldState, reason);
        }
        void accessPointAdded(NMPPAccessPoint addedAP) override
        {
            interface.accessPointAdded(addedAP);
        }
        void accessPointRemoved(NMPPAccessPoint removedAP) override
        {
            interface.accessPointRemoved(removedAP);
        }
        void activeConnectionChanged(NMPPActiveConnection active) override
        {
            interface.activeConnectionChanged(active);
        }
        LibNMInterface& interface;
    };
    
    juce::ScopedPointer<ClientListener> clientListener;
    juce::ScopedPointer<DeviceListener> deviceListener;
    
    /**
     * Restricts concurrent access to the wifi data.  This is a reference to the
     * ReadWriteLock used by WifiStateManager, so it should not be locked
     * within this class in any method the WifiStateManager could call.
     *
     * The only reason this object is accessible here at all is so LibNM
     * callback functions can acquire the lock when updating data.  Even then,
     * the lock must never be acquired within the Glib event thread, as this 
     * can easily create a deadlock where the GLib thread waits for the
     * wifiLock, while another thread holding the wifiLock waits for access to
     * the GLib thread.
     */
    juce::ReadWriteLock& wifiLock;

    //The client used to access the network manager.
    NMPPClient client;
    //The active wifi device.
    NMPPDeviceWifi wifiDevice;
            
    //The last known state of the wifi device.
    NMDeviceState lastNMState = NM_DEVICE_STATE_UNKNOWN;
    //All connections saved with NetworkManager.
    SavedConnections savedConnections;
    
    //The current active/activating wifi connection, or a null object.
    NMPPActiveConnection activeConnection;
    //The access point used by the active connection, or a null object.
    WifiAccessPoint activeAP;
    
    /**
     * When creating a new connection, the connection's access point will
     * be saved here.  If a connection fails due to invalid security settings,
     * this value can be checked to see if the connection is a new one.
     * 
     * New connections that are not valid are safe to delete immediately, but
     * this application should not be deleting the user's pre-existing
     * connections, even if they seem to be invalid.
     */
    WifiAccessPoint newConnectionAP;
    
    /**
     * Used by getAPState to track failed connection attempts.
     * Whenever an attempted connection fails due to invalid security, the
     * connection's access point will be added to this list.  If a successful
     * connection is created, all access points that could use that connection
     * will be removed from this list. 
     */
    juce::Array<WifiAccessPoint> failedConnectionAPs;
    
    //All access points visible to the wifi device.
    juce::Array<NMPPAccessPoint> visibleAPs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LibNMInterface)
};
