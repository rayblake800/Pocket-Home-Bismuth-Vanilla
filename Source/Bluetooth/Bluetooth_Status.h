#pragma once
/**
 * @file  Bluetooth_Status.h
 * 
 * TODO: Make this actually do something.
 */

#include "Bluetooth_Device.h"
#include "JuceHeader.h"

namespace Bluetooth { class Status; }

class Bluetooth::Status 
{
public:
    Status();
    
    juce::OwnedArray<Device> devices;
    bool enabled = false;

    void populateFromJson(const juce::var &json);
};
