#pragma once
#include <nm-device-wifi.h>
#include "NMPPAccessPoint.h"
#include "NMPPConnection.h"
#include "GPPObject.h"

class NMPPDeviceWifi : public GPPObject
{
public:
    NMPPDeviceWifi(const NMPPDeviceWifi& toCopy);
    
    NMPPDeviceWifi(NMDeviceWifi* toAssign);
    
    NMPPDeviceWifi();
    
    NMDeviceState getState();
    
    NMDeviceStateReason getStateReason();
    
    bool isManaged();
    
    const char* getInterface();
    
    const char* getPath();
    
    void disconnect();
    
    NMPPConnection getActiveConnection();
    
    NMPPAccessPoint getAccessPoint(const char* path);
    
    Array<NMPPAccessPoint> getAccessPoints();
    
    class WifiListener : public GPPObject::SignalHandler
    {
    public:
        WifiListener() { }
        virtual ~WifiListener() { }
    private:
        virtual void stateChanged(NMPPDeviceWifi* device,
                NMDeviceState newState,
                NMDeviceState oldState,
                NMDeviceStateReason reason) = 0;
        
        virtual void accessPointAdded(NMAPAccessPoint addedAP) = 0;
        
        virtual void accessPointRemoved(NMAPAccessPoint removedAP) = 0;
    };
    
    void addListener(WifiListener* listener);
    
    void removeListener(WifiListener* listener);
    
private:
    static void stateChangeCallback(
            NMDevice* device,
	    NMDeviceState new_state,
            NMDeviceState old_state,
            NMDeviceStateReason reason,
            WifiListener* listener);
    
    static void apAddedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
            WifiListener* listener);
    
    static void apRemovedCallback(NMDeviceWifi* device, NMAccessPoint* ap,
            WifiListener* listener);
};