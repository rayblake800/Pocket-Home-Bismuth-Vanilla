#pragma once
/**
 * @file  Bluetooth/BluetoothDevice.h
 *
 * @brief  Represents a visible bluetooth device.
 */

#include "JuceHeader.h"

class BluetoothDevice
{
public:
    BluetoothDevice() { }
    virtual ~BluetoothDevice() { }
    
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

//private:
    juce::String name;
    juce::String macAddress;
    bool connected;
    bool paired;
};
