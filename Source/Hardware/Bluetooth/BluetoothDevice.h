#pragma once
#include "JuceHeader.h"

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