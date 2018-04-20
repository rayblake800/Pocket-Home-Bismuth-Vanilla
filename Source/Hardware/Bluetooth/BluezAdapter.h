/**
 * 
 */
#pragma once
#include "GDBusProxyInterface.h"

class BluezAdapter : private GDBusProxyInterface
{
public:
    BluezAdapter();

    virtual ~BluezAdapter() { }
    
    String getDeviceAddress();
    
    String getDeviceName();
    
    StringArray getUUIDs();
    
    bool isPowered();
    
    bool isDiscoverable();
    
    bool isPairable();
    
    void startDiscovery();
    
    void stopDiscovery();
    
    void setPowered(bool powered);
    
    void setDiscoverable(bool discoverable);
    
    void setPairable(bool pairable);
    
#ifdef JUCE_DEBUG
    void printDebug();
#endif
};
