#pragma once
#include "LibNM/NMObjects/AccessPoint.h"
#include "LibNM/NMObjects/Connection.h"
#include "LibNM/NMObjects/ActiveConnection.h"
#include "GLib/SignalHandler.h"
#include <nm-device-wifi.h>

/**
 * @file LibNM/DeviceWifi.h
 *
 * @brief A RAII container and C++ interface for LibNM NMDeviceWifi
 *        objects.
 *
 *  DeviceWifi represents a system Wifi device recognized by NetworkManager. It
 * provides information about the Wifi device's state, loads AccessPoint objects
 * representing access points visible to the device, and provides limited
 * methods for controlling the device.
 *
 *  DeviceWifi provides methods to close any connection active on the device,
 * and to make the device re-scan visible access points. Enabling or disabling
 * the device, and creating new connections using the device is handled by the
 * LibNM::Client class.
 *
 *  DeviceWifi also provides a Listener object interface that may be used to
 * track NMDeviceWifi signals. Listeners are notified when the active connection
 * changes, access points are discovered or lost, or the device's state changes. 
 */
class LibNM::DeviceWifi : public LibNM::Object
{
public:
    /**
     * @brief  Creates a DeviceWifi object that shares a NMDeviceWifi* with 
     *         another DeviceWifi object
     *
     * @param toCopy  This object's NMDeviceWifi* will be shared with the
     *                new object.
     */
    DeviceWifi(const DeviceWifi& toCopy);
    
    /**
     * @brief  Creates a DeviceWifi to contain a NMDeviceWifi object.
     * 
     * @toAssign  A valid NMDeviceWifi for this DeviceWifi to hold.
     */
    DeviceWifi(NMDeviceWifi* toAssign);
    
    /**
     * @brief  Creates a null DeviceWifi.
     */
    DeviceWifi();
    
    /**
     * @brief  Gets the current state of the wifi network device.
     * 
     * @return  The current device state, or NM_DEVICE_STATE_UNKNOWN if this
     *          object is null.
     */
    NMDeviceState getState() const;
    
    /**
     * @brief  Gets the reason for the current device state.
     * 
     * @return  An explanation for the value returned by getState(), or 
     *          NM_DEVICE_STATE_REASON_UNKNOWN if this object is null.
     */
    NMDeviceStateReason getStateReason() const;
    
    /**
     * @brief  Checks if NetworkManager manages this device.
     *
     * @return  Whether this device is managed by NetworkManager.
     */
    bool isManaged() const;
    
    /**
     * @brief  Gets the interface name of this wifi device.
     * 
     * @return  The interface name, or the empty string if this object is null.
     */
    const char* getInterface() const;
    
    /**
     * @brief  Disconnects any connection that is active on this wifi device. 
     *         
     * If there is no active connection, or the object is null, nothing will 
     * happen.
     */
    void disconnect();
    
    /**
     * @brief  Gets the current active connection running on this device.
     * 
     * @return  The active connection object, or a null connection object if
     *          this object is not connected or is null.
     */
    ActiveConnection getActiveConnection() const;
    
    /**
     * @brief  Gets the list of connections available to activate on this 
     *         device.
     *
     * This might not load all saved connections.
     * 
     * @return  The list of known connections that are compatible with this
     *          device.
     */
    juce::Array<Connection> getAvailableConnections() const;
    
    /**
     * @brief  Finds the first available connection that is compatible with a 
     *         specific wifi access point.
     * 
     * @param accessPoint  A wifi access point object.
     * 
     * @return             The first available connection that could be used to 
     *                     activate this access point, or a null connection if 
     *                     this device is null, the access point is null, or no 
     *                     matching connection is found. 
     */
    Connection getAvailableConnection
    (const AccessPoint& accessPoint) const;
    
    /**
     * @brief  Gets an access point object using the access point's path.
     * 
     * @param path  A valid wifi access point DBus path.
     *
     * @return      The matching access point object, or a null access point 
     *              if the access point was not found or this object is null.
     */
    AccessPoint getAccessPoint(const char* path) const;
    
    /**
     * @brief  Gets the active connection's access point.
     *
     * @return  The active access point object, or a null access point object
     *          if this object is disconnected or null.
     */
    AccessPoint getActiveAccessPoint() const;
    
    /**
     * @brief  Gets all access points visible to this device.
     * 
     * @return  An array containing one access point object for each nearby
     *          wifi access point visible to the device.
     */
    juce::Array<AccessPoint> getAccessPoints() const;
    
    /**
     * @brief  Checks if a specific connection is present in the list of 
     *         available device connections.
     * 
     * @param toFind  A connection object to find.
     * 
     * @return        Whether a matching connection is already known to this 
     *                wifi device.
     */
    bool hasConnectionAvailable(const Connection& toFind) const;
    
    /**
     * @brief  Sends a request to the wifi device asking it to scan visible 
     *         access points.
     */
    void requestScan();
    
    /**
     * @brief  Listeners receive notifications when the wifi device changes, a 
     *         new access point object is added, or an access point is removed.
     */
    class Listener : public GLib::SignalHandler
    {
    public:
        friend class DeviceWifi;
        Listener() { }
        
        virtual ~Listener() { }
        
    private:
       /**
        * @brief  Subscribes to all relevant signals from a single GObject 
        *         signal source.
        * 
        * @param source  A NMDeviceWifi GObject this signal handler should 
        *                track.
        */
        virtual void connectAllSignals(GObject* source) override;
 
	   /**
	    * @brief  This method will be called whenever the wifi device state 
        *         changes.
	    *
	    * @param newState  The new device state value.
	    *
	    * @param oldState  The previous device state value.
	    *
	    * @param reason    The reason for the change in device state.
        */
        virtual void stateChanged(NMDeviceState newState,
                NMDeviceState oldState,
                NMDeviceStateReason reason) = 0;
        
	   /**
        * @brief  This method will be called whenever the wifi device detects a
        *         new access point.
        * 
    	* @param addedAP  The new access point detected by the wifi device.
    	*/
        virtual void accessPointAdded(AccessPoint addedAP) = 0;
        
	   /**
        * @brief  This method will be called whenever the wifi device no longer
        *         detects a wifi access point.
        * 
	    * @param removedAP  The nearby access point that the device can no
        *                   longer detect.
	    */
        virtual void accessPointRemoved(AccessPoint removedAP) = 0;
        
       /**
        * @brief  This method will be called whenever the device's active 
        *         connection changes.
        * 
        * @param active   The new active connection. If the device has
        *                 disconnected, this will be a null object.
        */
        virtual void activeConnectionChanged(ActiveConnection active) = 0;
        
       /**
        * @brief  Converts generic property change notifications into 
        *         activeConnectionChanged calls.
        * 
        * @param source    The GObject that emitted the signal. This should be a
        *                  NMDeviceWifi object.
        * 
        * @param property  This should be the active connection property, 
        *                  "active-connection"
        */
        virtual void propertyChanged(GObject* source,
                juce::String property) override;
    };
    
    /**
     * @brief  Adds a listener to this wifi device.
     * 
     * @param listener  This object will receive updates when the wifi device
     *                  state changes or the list of visible access points is
     *                  updated.
     */
    void addListener(Listener& listener);
    
private:
    /**
     * @brief  The GCallback method called directly by LibNM code when sending 
     *         state-changed signals.  
     *
     * This will use the signal data to call the listener object's stateChanged 
     * method.
     * 
     * @param device    The GObject sending the state change signal.
     *
     * @param newState  The new device state.
     *
     * @param oldState  The previous device state.
     *
     * @param reason    The reason for the change in state.
     *
     * @param listener  An object set up to handle signals from this device.
     */
    static void stateChangeCallback(
            NMDevice* device,
	        NMDeviceState newState,
            NMDeviceState oldState,
            NMDeviceStateReason reason,
            Listener* listener);
    
    /**
     * @brief  The GCallback method called directly by LibNM code when sending 
     *         access-point-added signals.  
     *
     * This will use the signal data to call the listener object's 
     * accessPointAdded method
     * 
     * @param device    The GObject sending the signal.
     *
     * @param ap        An access point object representing a newly discovered
     *                  access point.
     *
     * @param listener  An object set up to handle signals from this device.
     */
    static void apAddedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
            Listener* listener);
    
    /**
     * @brief  The GCallback method called directly by LibNM code when sending 
     *         access-point-removed signals.  
     *
     * This will use the signal data to call the listener object's 
     * accessPointRemoved method.
     * 
     * @param device    The GObject sending the signal.
     *
     * @param ap        An access point object representing a wifi access point
     *                  that is no longer visible.
     *
     * @param listener  An object set up to handle signals from this device.
     */
    static void apRemovedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
            Listener* listener);
};
