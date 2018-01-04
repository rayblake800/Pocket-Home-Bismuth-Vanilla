/*
  ==============================================================================

    BluetoothStatus.h
    Created: 3 Jan 2018 1:34:22am
    Author:  anthony

  ==============================================================================
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