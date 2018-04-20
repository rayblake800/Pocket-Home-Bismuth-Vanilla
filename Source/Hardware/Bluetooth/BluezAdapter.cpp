#include "BluezAdapter.h"

BluezAdapter::BluezAdapter() :
GDBusProxyInterface("org.bluez", "/org/bluez/hci0", "org.bluez.Adapter1") { }

String BluezAdapter::getDeviceAddress()
{
    return getProperty<String>("Address");
}

String BluezAdapter::getDeviceName()
{
    return getProperty<String>("Name");
}

StringArray BluezAdapter::getUUIDs()
{
    return getProperty<StringArray>("UUIDs");
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
    StringArray uuids = getUUIDs();
    for (String& uuid : uuids)
    {
        DBG("\t\t" << uuid);
    }
}
#endif