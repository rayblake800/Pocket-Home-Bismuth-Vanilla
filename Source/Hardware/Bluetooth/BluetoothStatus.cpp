#include "BluetoothStatus.h"
#include "BluezAdapter.h"

BluetoothStatus::BluetoothStatus()
{
#    ifdef JUCE_DEBUG
    BluezAdapter bluetoothAdapter;
    bluetoothAdapter.printDebug();
    
    DBG("Setting discoverable:");
    bluetoothAdapter.setDiscoverable(true);
    bluetoothAdapter.printDebug();
    
#    endif

}

void BluetoothStatus::populateFromJson(const var &json)
{
    devices.clear();

    for (const auto &btDevice : *json.getArray())
    {
        auto device = new BluetoothDevice();
        device->name = btDevice["name"].toString();
        device->macAddress = btDevice["mac"].toString();
        device->connected = btDevice["connected"];
        device->paired = btDevice["paired"];
        devices.add(device);
    }
}