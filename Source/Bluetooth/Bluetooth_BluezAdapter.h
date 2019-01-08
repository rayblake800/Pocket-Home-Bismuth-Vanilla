#pragma once
/**
 * @file  Bluetooth_BluezAdapter.h
 *
 * @brief  Extremely incomplete DBus proxy class that will eventually be used
 *         to control Bluetooth connections.
 */

#include "GLib_DBusProxy.h"

namespace Bluetooth { class BluezAdapter; }

class Bluetooth::BluezAdapter : private GLib::DBusProxy
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
