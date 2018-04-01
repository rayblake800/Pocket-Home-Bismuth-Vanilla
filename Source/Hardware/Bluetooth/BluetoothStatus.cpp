#include "BluetoothStatus.h"
#include "gio/gio.h"

BluetoothStatus::BluetoothStatus()
{
    const gchar* name = "org.bluez";
    const gchar* path = "/org/bluez/hci0";
    const gchar* interfaceName = "org.bluez.Adapter1";
    GError * error = nullptr;
    GDBusProxy* bluezProxy = g_dbus_proxy_new_for_bus_sync(
            G_BUS_TYPE_SYSTEM,
            G_DBUS_PROXY_FLAGS_NONE,
            nullptr,
            name,
            path,
            interfaceName,
            nullptr,
            &error);
    DBG("Name=" << String(g_dbus_proxy_get_name(bluezProxy)));
    DBG("Owner=" << String(g_dbus_proxy_get_name_owner(bluezProxy)));
    DBG("Path=" << String(g_dbus_proxy_get_object_path(bluezProxy)));
    DBG("Interface=" << String(g_dbus_proxy_get_interface_name(bluezProxy)));
    DBG("Timeout=" << String(g_dbus_proxy_get_default_timeout(bluezProxy)));
    gchar** properties = g_dbus_proxy_get_cached_property_names(bluezProxy);
    if (properties == nullptr)
    {
        DBG("Properties=none");
    }
    else
    {
        for (gchar* prop = properties[0]; prop != NULL; prop++)
        {
            DBG("Property="<<String(prop));
        }
    }
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