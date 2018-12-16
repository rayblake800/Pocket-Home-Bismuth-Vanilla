#include "LibNM/NMObjects/Settings.h"
#include <nm-connection.h>

/*
 * Creates a Settings object sharing data with another Settings object.
 */
LibNM::Settings::Settings(const Settings& toCopy, const GType settingType) :
LibNM::Object(toCopy, settingType) { }

/*
 * Creates a Settings object to contain NMSetting data.
 */
LibNM::Settings::Settings(NMSetting* toAssign, const GType settingType) :
LibNM::Object(NM_OBJECT(toAssign), settingType) { }

/*
 * @brief  Creates a null Settings object.
 */
LibNM::Settings::Settings(const GType settingType) :
LibNM::Object(settingType) { }
