#pragma once

#include "NMPPClient.h"
#include "SavedConnections.h"
#include "WindowFocus.h"
#include "WifiStateManager.h"

/**
 * @file LibNMInterface.h
 * 
 * @brief Connects to the libnm NetworkManager to track and control wifi device.
 * 
 * As a WifiStateManager::NetworkInterface, only the WifiStateManager should
 * directly interact with this class.
 */

class LibNMInterface : public WifiStateManager::NetworkInterface, 
        public NMPPClient::ConnectionHandler 
{
public:
    /**
     * @param wifiLock  The lock used by the WifiStateManager to control access
     *                  to the network interface.  This will be used to ensure
     *                  the interface is locked when receiving updates from 
     *                  NetworkManager.
     */
    LibNMInterface(CriticalSection& wifiLock);

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
     * Requests information on the connected access point from the NMDevice.
     * 
     * @return a WifiAccessPoint object representing the access point connected
     *         to the wifi device, or WifiAccessPoint() if not connected.
     */
    WifiAccessPoint getConnectedAP() override;

    /**
     * Requests information on the connecting access point from the NMDevice.
     * 
     * @return  a WifiAccessPoint object representing the access point 
     *          connecting to the wifi device, or WifiAccessPoint() if not 
     *          connecting.
     */
    WifiAccessPoint getConnectingAP() override;

    /**
     * Request information on all wifi access points detected by the NMDevice.
     * 
     * @return  a list of all visible access points within range of the wifi
     *          device, or an empty list if wifi is disabled.
     */
    Array<WifiAccessPoint> getVisibleAPs() override;

    /**
     * Begin opening a connection to a wifi access point.
     * 
     * @param toConnect  The access point that should be connected to. 
     * 
     * @param psk        The access point's security key. This will be ignored
     *                   if the access point is unsecured.
     */
    void connectToAccessPoint(WifiAccessPoint toConnect,
            String psk = String()) override;

    /**
     * Checks the wifi device list, connection state, and active and pending 
     * connections.
     */
    void updateAllWifiData();

    /**
     * Close the currently active connection, if one exists.
     */
    void disconnect() override;
    
    /**
     * Cancel any activating wifi connection.
     */
    void stopConnecting() override;

    /**
     * Asynchronously turns on the wifi radio.
     */
    void enableWifi() override;

    /**
     * Asynchronously turns off the wifi radio.
     */
    void disableWifi() override;

private:       
    /**
     * Notify listeners and save the connecting access point if starting to
     * connect.
     * 
     * @param connection  A new active connection object representing the 
     *                    added connection. This connection object might not
     *                    be completely connected yet.
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
     * #param connection  The connection that failed to activate.  This
     *                    may be a null connection.
     * 
     * @param error       A GError object describing the problem.  This 
     *                    error object should not be freed.
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
     *                 disconnected, this will be a null object.
     */
    virtual void activeConnectionChanged(NMPPActiveConnection active);
    
    /**
     *#######################  Listener Classes  ###############################
     * These objects listen to signals from NetworkManager classes and pass them
     * on to the LibNMInterface.  All GPPObject Listeners share a parent class,
     * so one object can't simply inherit multiple listener classes directly.
     */
    class ClientListener : public NMPPClient::Listener
    {
    public:
        ClientListener(LibNMInterface& interface, NMPPClient& client) :
        interface(interface)
        {
            if(!client.isNull())
            {
                client.addSignalHandler(this);
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
                device.addSignalHandler(this);
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
    
    ScopedPointer<ClientListener> clientListener;
    ScopedPointer<DeviceListener> deviceListener;
    
    //Prevent concurrent access to the wifi data.  This should be locked
    //any time any of the data members listed beneath it is accessed.
    CriticalSection& wifiLock;

    //The client used to access the network manager.
    NMPPClient client;
    //The active wifi device.
    NMPPDeviceWifi wifiDevice;
            
    //The last known state of the wifi device.
    NMDeviceState lastNMState = NM_DEVICE_STATE_UNKNOWN;
    //All connections saved with NetworkManager.
    SavedConnections savedConnections;
    
    //The current active wifi connection, or a null object.
    NMPPActiveConnection activeConnection;
    //The access point used by the active connection, or a null object.
    NMPPAccessPoint connectedAP;
    
    //The connection currently being activated, or a null object.
    NMPPActiveConnection activatingConnection;
    //The access point used by the activating connection, or a null object.
    NMPPAccessPoint connectingAP;
    
    //All access points visible to the wifi device.
    Array<NMPPAccessPoint> visibleAPs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LibNMInterface)
};
