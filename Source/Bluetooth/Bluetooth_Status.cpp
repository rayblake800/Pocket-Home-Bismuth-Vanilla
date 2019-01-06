#include "Bluetooth_Status.h"

Bluetooth::Status::Status() { }

void Bluetooth::Status::populateFromJson(const juce::var &json)
{
    devices.clear();

    for (const auto &btDevice : *json.getArray())
    {
        auto device = new Device();
        device->name = btDevice["name"].toString();
        device->macAddress = btDevice["mac"].toString();
        device->connected = btDevice["connected"];
        device->paired = btDevice["paired"];
        devices.add(device);
    }
}
