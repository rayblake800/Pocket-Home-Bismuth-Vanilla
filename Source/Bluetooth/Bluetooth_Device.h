#pragma once
/**
 * @file  Bluetooth_Device.h
 *
 * @brief  Early placeholder for a class that will represent a visible bluetooth
 *         device.
 */

#include "JuceHeader.h"

namespace Bluetooth { class Device; }

class Bluetooth::Device
{
public:
    Device() { }
    virtual ~Device() { }
    
    bool isNull() const
    {
        return macAddress.isEmpty();
    }

    bool operator==(const Device rhs) const
    {
        return macAddress == rhs.macAddress;
    }

    bool operator!=(const Device rhs) const
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
