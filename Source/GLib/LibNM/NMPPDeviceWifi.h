#pragma once
#include <nm-device-wifi.h>
#include "NMPPAccessPoint.h"
#include "NMPPConnection.h"
#include "GPPObject.h"

/**
 * @file NMPPDeviceWifi.h
 *
 * @brief A RAII container and C++ interface for LibNM NMDeviceWifi
 *        objects.
 */
class NMPPDeviceWifi : public GPPObject
{
public:
    /**
     * Creates a NMPPDeviceWifi object that shares a NMDeviceWifi*
     * with another NMPPDeviceWifi object
     *
     * @param toCopy  This object's NMDeviceWifi* will be shared with the
     *                new object.
     */
    NMPPDeviceWifi(const NMPPDeviceWifi& toCopy);
    
    /**
     * Create a NMPPDeviceWifi to contain a NMDeviceWifi object.
     * 
     * @toAssign  A valid NMDeviceWifi for this NMPPDeviceWifi to hold.
     */
    NMPPDeviceWifi(NMDeviceWifi* toAssign);
    
    /**
     * Creates a null NMPPDeviceWifi.
     */
    NMPPDeviceWifi() { }
    
    /**
     * Gets the current state of the wifi network device.
     * 
     * @return  the current device state, or NM_DEVICE_STATE_UNKNOWN if this
     *          object is null.
     */
    NMDeviceState getState() const;
    
    /**
     * Gets the reason for the current device state.
     * 
     * @return  an explanation for the value returned by getState(), or 
     *          NM_DEVICE_STATE_REASON_UNKNOWN if this object is null.
     */
    NMDeviceStateReason getStateReason() const;
    
    /**
     * Checks if NetworkManager manages this device.
     *
     * @return  true iff this device is managed by NetworkManager.
     */
    bool isManaged() const;
    
    /**
     * Gets the interface name of this wifi device.
     * 
     * @return  the interface name, or nullptr if this object is null.
     */
    const char* getInterface() const;
    
    /**
     * Gets the DBus path of this wifi device.
     * 
     * @return  the path value, or nullptr if this object is null.
     */
    const char* getPath() const;
    
    /**
     * Disconnects any connection that is active on this wifi device.  If there
     * is no active connection, or the object is null, nothing will happen.
     */
    void disconnect();
    
    /**
     * Gets the current active connection running on this device.
     * 
     * @return  the active connection object, or a null connection object if
     *          this object is not connected or is null.
     */
    NMPPConnection getActiveConnection() const;
    
    /**
     * Gets the list of connections available to activate on this device.
     * This might not load all saved connections.
     * 
     * @return  The list of known connections that are compatible with this
     *          device.
     */
    Array<NMPPConnection> getAvailableConnections() const;
    
    /**
     * Gets an access point object using the access point's path.
     * 
     * @param path  A valid wifi access point DBus path.
     *
     * @return  the matching access point object, or a null access point object
     *          if the access point was not found or this object is null.
     */
    NMPPAccessPoint getAccessPoint(const char* path) const;
    
    /**
     * Gets all access points visible to this device.
     * 
     * @return  An array containing one access point object for each nearby
     *          wifi access point visible to the device.
     */
    Array<NMPPAccessPoint> getAccessPoints() const;
    
    /**
     * Sends a request to the wifi device asking it to scan visible access 
     * points.
     */
    void requestScan();
    
    /**
     * Listeners receive notifications when the wifi device changes, a new
     * access point object is added, or an access point is removed.
     */
    class Listener : public GPPObject::SignalHandler
    {
    public:
        friend class NMPPDeviceWifi;
        Listener() { }
        virtual ~Listener() { }
    private:
	/**
	 * This method will be called whenever the wifi device state changes.
         * 
	 * @param device    The updated wifi device object.
	 *
	 * @param newState  The new device state value.
	 *
	 * @param oldState  The previous device state value.
	 *
	 * @param reason    The reason for the change in device state.
	 */
        virtual void stateChanged(NMPPDeviceWifi* device,
                NMDeviceState newState,
                NMDeviceState oldState,
                NMDeviceStateReason reason) = 0;
        
	/**
         * This method will be called whenever the wifi device detects a new
         * access point.
         * 
	 * @param addedAP  The new access point detected by the wifi device.
	 */
        virtual void accessPointAdded(NMPPAccessPoint addedAP) = 0;
        
	/**
         * This method will be called whenever the wifi device no longer detects
         * a wifi access point.
         * 
	 * @param removedAP  The nearby access point that the device can no
         *                   longer detect.
	 */
        virtual void accessPointRemoved(NMPPAccessPoint removedAP) = 0;
    };
    
    /**
     * Adds a listener object to this wifi device.
     * 
     * @param listener  This object will receive updates when the wifi device
     *                  state changes or the list of visible access points is
     *                  updated.
     */
    void addListener(Listener* listener);
    
    /**
     * Removes a listener object from this wifi device.
     * 
     * @param listener  This object will no longer receive updates on this
     *                  device's state or visible access points.
     */
    void removeListener(Listener* listener);
    
private:
    /**
     * The GCallback method called directly by LibNM code when sending 
     * state-changed signals.  This will use the signal data to call the 
     * listener object's stateChanged method.
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
     * The GCallback method called directly by LibNM code when sending 
     * access-point-added signals.  This will use the signal data to call the
     * listener object's accessPointAdded method
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
     * The GCallback method called directly by LibNM code when sending 
     * access-point-removed signals.  This will use the signal data to call the
     * listener object's accessPointRemoved method
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

    /**
     * Gets the GType of this object's stored GObject class.
     * 
     * @return NM_TYPE_DEVICE_WIFI
     */
    GType getType() const override;
    
    /**
     * Checks if a GObject's type allows it to be held by this object. 
     * 
     * @param toCheck  Any valid GObject, or nullptr.
     * 
     * @return  true iff toCheck is a NMDeviceWifi or is null. 
     */
    virtual bool isValidType(GObject* toCheck) const override;
    
    /**
     * Used to re-add a list of Listeners to new GObject data.
     * 
     * @param toTransfer  A list of Listener objects to add to this
     *                    NMPPDeviceWifi.
     */
    virtual void transferSignalHandlers(Array<SignalHandler*>& toTransfer);
};
