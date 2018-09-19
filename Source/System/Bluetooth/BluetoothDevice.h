#pragma once
#include "JuceHeader.h"

class BluetoothDevice
{
public:
    juce::String name;
    juce::String macAddress;
    bool connected;
    bool paired;
    
    bool isNull() const
    {
        return macAddress.isEmpty();
    }

    bool operator==(const BluetoothDevice rhs) const
    {
        return macAddress == rhs.macAddress;
    }

    bool operator!=(const BluetoothDevice rhs) const
    {
        return macAddress != rhs.macAddress;
    }

    juce::String toString() const
    {
        return name;
    }

};