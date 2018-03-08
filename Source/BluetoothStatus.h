/**
 * @file BluetoothStatus.h
 * 
 * TODO: Make this actually do something.
 */
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

struct BluetoothDevice {
    String name;
    String macAddress;
    bool connected;
    bool paired;

    /**
     * Represents an invalid or missing connection point
     */
    static const BluetoothDevice null;

    bool operator==(const BluetoothDevice rhs) const {
        return macAddress == rhs.macAddress;
    };

    bool operator!=(const BluetoothDevice rhs) const {
        return macAddress != rhs.macAddress;
    };

};

struct BluetoothStatus {
    OwnedArray<BluetoothDevice> devices;
    bool enabled = false;

    void populateFromJson(const var &json);
};