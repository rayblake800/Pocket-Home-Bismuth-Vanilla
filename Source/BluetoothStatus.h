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
     * Represents a missing connection point
     */
    static const BluetoothDevice null(){
        return {"","null",false,false};
    }
    
    bool isNull() const{
        return name.isEmpty() && macAddress == "null" 
                && !connected && !paired;
    }

    bool operator==(const BluetoothDevice rhs) const {
        return macAddress == rhs.macAddress;
    };

    bool operator!=(const BluetoothDevice rhs) const {
        return macAddress != rhs.macAddress;
    };
    
    String toString() const{
        return name;
    }

};

struct BluetoothStatus {
    OwnedArray<BluetoothDevice> devices;
    bool enabled = false;

    void populateFromJson(const var &json);
};