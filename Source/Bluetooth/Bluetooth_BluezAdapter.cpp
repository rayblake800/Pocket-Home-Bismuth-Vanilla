#include "Bluetooth_BluezAdapter.h"

Bluetooth::BluezAdapter::BluezAdapter() :
GLib::DBusProxy("org.bluez", "/org/bluez/hci0", "org.bluez.Adapter1") { }

juce::String Bluetooth::BluezAdapter::getDeviceAddress()
{
    return getProperty<juce::String>("Address");
}

juce::String Bluetooth::BluezAdapter::getDeviceName()
{
    return getProperty<juce::String>("Name");
}

juce::StringArray Bluetooth::BluezAdapter::getUUIDs()
{
    return getProperty<juce::StringArray>("UUIDs");
}

bool Bluetooth::BluezAdapter::isPowered()
{
    return getProperty<bool>("Powered");
}

bool Bluetooth::BluezAdapter::isDiscoverable()
{
    return getProperty<bool>("Discoverable");
}

bool Bluetooth::BluezAdapter::isPairable()
{
    return getProperty<bool>("Pairable");
}

void Bluetooth::BluezAdapter::startDiscovery()
{
    callMethod("startDiscovery");
}

void Bluetooth::BluezAdapter::stopDiscovery()
{
    callMethod("stopDiscovery");
}

void Bluetooth::BluezAdapter::setPowered(bool powered)
{
    setProperty<bool>("Powered", powered);
}

void Bluetooth::BluezAdapter::setDiscoverable(bool discoverable)
{
    setProperty<bool>("Discoverable", discoverable);
}

void Bluetooth::BluezAdapter::setPairable(bool pairable)
{
    setProperty<bool>("Pairable", pairable);
}

#ifdef JUCE_DEBUG

void Bluetooth::BluezAdapter::printDebug()
{
    DBG("Bluetooth adapter " << getDeviceName());
    DBG("\taddress:" << getDeviceAddress());
    DBG("\tPowered:" << (isPowered() ? "true" : "false"));
    DBG("\tDiscoverable:" << (isDiscoverable() ? "true" : "false"));
    DBG("\tPairable:" << (isPairable() ? "true" : "false"));
    DBG("\tUUIDs:");
    juce::StringArray uuids = getUUIDs();
    for (juce::String& uuid : uuids)
    {
        DBG("\t\t" << uuid);
    }
}
#endif
