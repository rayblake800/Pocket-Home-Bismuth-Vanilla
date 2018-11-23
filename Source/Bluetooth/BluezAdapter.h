#pragma once
#include "GLib/DBus/DBusProxy.h"

class BluezAdapter : private GLib::DBusProxy
{
public:
    BluezAdapter();

    virtual ~BluezAdapter() { }
    
    juce::String getDeviceAddress();
    
    juce::String getDeviceName();
    
    juce::StringArray getUUIDs();
    
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
