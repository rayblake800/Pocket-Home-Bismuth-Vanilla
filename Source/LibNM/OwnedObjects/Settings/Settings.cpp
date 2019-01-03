#include "LibNM/OwnedObjects/Settings/Settings.h"
#include <nm-connection.h>

/*
 * Creates a Settings object sharing data with another Settings object.
 */
LibNM::Settings::Settings(const Settings& toCopy) :
GLib::Owned::Object(toCopy, toCopy.getType()) { }

/*
 * Creates a Settings object to contain NMSetting data.
 */
LibNM::Settings::Settings(NMSetting* toAssign, const GType settingType) :
GLib::Owned::Object(G_OBJECT(toAssign), settingType) { }

/*
 * @brief  Creates a null Settings object.
 */
LibNM::Settings::Settings(const GType settingType) :
GLib::Owned::Object(settingType) { }
