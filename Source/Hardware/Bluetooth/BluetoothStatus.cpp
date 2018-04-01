#include "BluetoothStatus.h"
#include "gio/gio.h"

static GDBusProxy* bluezProxy = nullptr;

String getPropStr(const gchar* name)
{
    GVariant* var = g_dbus_proxy_get_cached_property(bluezProxy, name);
    const GVariantType* type = g_variant_get_type(var);
    if (*type == *G_VARIANT_TYPE_STRING)
    {
        gsize length;
        return String(g_variant_get_string(var, &length));
    }
    if (*type == *G_VARIANT_TYPE_UINT32)
    {
        return String(g_variant_get_uint32(var));
    }
    if (*type == *G_VARIANT_TYPE_BOOLEAN)
    {
        return String(g_variant_get_boolean(var) ? "true" : "false");
    }
    return String("unhandled type ") + String(g_variant_get_type_string(var));
}

BluetoothStatus::BluetoothStatus()
{

    const gchar* name = "org.bluez";
    const gchar* path = "/org/bluez/hci0";
    const gchar* interfaceName = "org.bluez.Adapter1";
    GError * error = nullptr;
    bluezProxy = g_dbus_proxy_new_for_bus_sync(
            G_BUS_TYPE_SYSTEM,
            G_DBUS_PROXY_FLAGS_NONE,
            nullptr,
            name,
            path,
            interfaceName,
            nullptr,
            &error);
    DBG("Proxy Name=" << String(g_dbus_proxy_get_name(bluezProxy)));
    DBG("Proxy Owner=" << String(g_dbus_proxy_get_name_owner(bluezProxy)));
    DBG("Proxy Path=" << String(g_dbus_proxy_get_object_path(bluezProxy)));
    DBG("Proxy Interface=" << String(g_dbus_proxy_get_interface_name(bluezProxy)));
    DBG("Proxy Timeout=" << String(g_dbus_proxy_get_default_timeout(bluezProxy)));

    DBG("Address=" << getPropStr("Address"));
    DBG("AddressType=" << getPropStr("AddressType"));
    DBG("Name=" << getPropStr("Name"));
    DBG("Alias=" << getPropStr("Alias"));
    DBG("Class=" << getPropStr("Class"));
    DBG("Name=" << getPropStr("Name"));
    DBG("Powered=" << getPropStr("Powered"));
    DBG("Discoverable=" << getPropStr("Discoverable"));
    DBG("Pairable=" << getPropStr("Pairable"));
    DBG("PairableTimeout=" << getPropStr("PairableTimeout"));
    DBG("DiscoverableTimeout=" << getPropStr("DiscoverableTimeout"));
    DBG("Discovering=" << getPropStr("Discovering"));

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