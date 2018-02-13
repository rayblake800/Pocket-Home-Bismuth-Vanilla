/**
 * @file BluetoothStatus.h
 * 
 * TODO: documentation, figure out why bluetooth settings are disabled
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