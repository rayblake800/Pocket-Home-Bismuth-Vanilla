#pragma once
#include "JuceHeader.h"

class BluetoothDevice
{
public:
    String name;
    String macAddress;
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

    String toString() const
    {
        return name;
    }

};