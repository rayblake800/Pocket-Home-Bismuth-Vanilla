#include "BluezAdapter.h"

BluezAdapter::BluezAdapter() :
GLib::DBusProxy("org.bluez", "/org/bluez/hci0", "org.bluez.Adapter1") { }

juce::String BluezAdapter::getDeviceAddress()
{
    return getProperty<juce::String>("Address");
}

juce::String BluezAdapter::getDeviceName()
{
    return getProperty<juce::String>("Name");
}

juce::StringArray BluezAdapter::getUUIDs()
{
    return getProperty<juce::StringArray>("UUIDs");
}

bool BluezAdapter::isPowered()
{
    return getProperty<bool>("Powered");
}

bool BluezAdapter::isDiscoverable()
{
    return getProperty<bool>("Discoverable");
}

bool BluezAdapter::isPairable()
{
    return getProperty<bool>("Pairable");
}

void BluezAdapter::startDiscovery()
{
    callMethod("startDiscovery");
}

void BluezAdapter::stopDiscovery()
{
    callMethod("stopDiscovery");
}

void BluezAdapter::setPowered(bool powered)
{
    setProperty<bool>("Powered", powered);
}

void BluezAdapter::setDiscoverable(bool discoverable)
{
    setProperty<bool>("Discoverable", discoverable);
}

void BluezAdapter::setPairable(bool pairable)
{
    setProperty<bool>("Pairable", pairable);
}

#ifdef JUCE_DEBUG

void BluezAdapter::printDebug()
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
