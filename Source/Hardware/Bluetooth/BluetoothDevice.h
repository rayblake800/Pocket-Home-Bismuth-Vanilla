#pragma once
#include "JuceHeader.h"

class BluetoothDevice : public ReferenceCountedObject
{
public:
    typedef ReferenceCountedObjectPtr<BluetoothDevice> Ptr;
    String name;
    String macAddress;
    bool connected;
    bool paired;

    bool operator==(const BluetoothDevice rhs) const
    {
        return macAddress == rhs.macAddress;
    };

    bool operator!=(const BluetoothDevice rhs) const
    {
        return macAddress != rhs.macAddress;
    };

    String toString() const
    {
        return name;
    }

};