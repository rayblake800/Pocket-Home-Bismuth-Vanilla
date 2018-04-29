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
     * 
     * @param toAssign
     */
    NMPPDeviceWifi(NMDeviceWifi* toAssign);
    
    /**
     *
     */
    NMPPDeviceWifi();
    
    /**
     *
     * @return
     */
    NMDeviceState getState();
    
    /**
     * @return
     */
    NMDeviceStateReason getStateReason();
    
    /**
     * Checks if NetworkManager manages this device.
     *
     * @return  true iff this device is managed by NetworkManager.
     */
    bool isManaged();
    
    /**
     *
     * @return
     */
    const char* getInterface();
    
    /**
     *
     */
    const char* getPath();
    
    /**
     *
     */
    void disconnect();
    
    /**
     *
     * @return
     */
    NMPPConnection getActiveConnection();
    
    /**
     *
     * @param path
     *
     * @return
     */
    NMPPAccessPoint getAccessPoint(const char* path);
    
    /**
     *
     * @return
     */
    Array<NMPPAccessPoint> getAccessPoints();
    
    /**
     *
     */
    class WifiListener : public GPPObject::SignalHandler
    {
    public:
        WifiListener() { }
        virtual ~WifiListener() { }
    private:
	/**
	 *
	 * @param device
	 *
	 * @param newState
	 *
	 * @param oldState
	 *
	 * @param reason
	 */
        virtual void stateChanged(NMPPDeviceWifi* device,
                NMDeviceState newState,
                NMDeviceState oldState,
                NMDeviceStateReason reason) = 0;
        
	/**
	 * @param addedAP
	 */
        virtual void accessPointAdded(NMAPAccessPoint addedAP) = 0;
        
	/**
	 * @param removedAP
	 */
        virtual void accessPointRemoved(NMAPAccessPoint removedAP) = 0;
    };
    
    /**
     * @param listener
     */
    void addListener(WifiListener* listener);
    
    /**
     * @param listener
     */
    void removeListener(WifiListener* listener);
    
private:
    /**
     * @param device
     *
     * @param newState
     *
     * @param oldState
     *
     * @param reason
     *
     * @param listener
     */
    static void stateChangeCallback(
            NMDevice* device,
	    NMDeviceState newState,
            NMDeviceState oldState,
            NMDeviceStateReason reason,
            WifiListener* listener);
    
    /**
     * @param device
     *
     * @param ap
     *
     * @param listener
     */
    static void apAddedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
            WifiListener* listener);
    
    /**
     * @param device
     *
     * @param ap
     *
     * @param listener
     *
     */
    static void apRemovedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
            WifiListener* listener);

    
    GType getType() const override;
};
