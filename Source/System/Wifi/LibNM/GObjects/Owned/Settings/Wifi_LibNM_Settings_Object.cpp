#include "Wifi_LibNM_Settings_Object.h"
#include "Wifi_LibNM_ContextTest.h"
#include <nm-connection.h>

namespace WifiNMSettings = Wifi::LibNM::Settings;

/*
 * Creates a Settings object to contain NMSetting data.
 */
WifiNMSettings::Object::Object
(NMSetting* toAssign, const GType settingType) :
GLib::Owned::Object(G_OBJECT(toAssign), settingType) { }

/*
 * @brief  Creates a null Settings object.
 */
WifiNMSettings::Object::Object(const GType settingType) :
GLib::Owned::Object(settingType) { }
