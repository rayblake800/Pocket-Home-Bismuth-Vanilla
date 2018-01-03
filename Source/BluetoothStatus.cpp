/*
  ==============================================================================

    BluetoothStatus.cpp
    Created: 3 Jan 2018 1:34:22am
    Author:  anthony

  ==============================================================================
*/

#include "BluetoothStatus.h"
void BluetoothStatus::populateFromJson(const var &json) {
    devices.clear();

    for (const auto &btDevice : *json.getArray()) {
        auto device = new BluetoothDevice();
        device->name = btDevice["name"].toString();
        device->macAddress = btDevice["mac"].toString();
        device->connected = btDevice["connected"];
        device->paired = btDevice["paired"];
        devices.add(device);
    }
}