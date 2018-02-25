/**
 * @file BluetoothStatus.h
 * 
 * TODO: Make this actually do something.
 */
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

struct BluetoothStatus {
    struct BluetoothDevice {
        String name, macAddress;
        bool connected = false;
        bool paired = false;
    };
    OwnedArray<BluetoothDevice> devices;
    bool enabled = false;

    void populateFromJson(const var &json);
};