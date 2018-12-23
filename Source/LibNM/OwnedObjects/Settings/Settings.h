#pragma once
/**
 * @file  LibNM/Settings/Settings.h
 *
 * @brief  Stores a set of related network connection settings associated with a
 *         LibNM::Connection.
 */

#include "GLib/Object.h"
#include <nm-connection.h>

namespace LibNM { class Settings; }

class LibNM::Settings : public GLib::Object
{
public:
    /**
     * @brief  Creates a Settings object sharing data with another Settings
     *         object.
     *
     * @param toCopy       Another Settings object to copy.
     */
    Settings(const Settings& toCopy);

    /**
     * @brief  Creates a Settings object to contain NMSetting data.
     *
     * @param toAssign     A valid NMSetting pointer for this Settings object to
     *                     hold, or nullptr to create a null Settings object.
     * 
     * @param settingType  The subtype of NM_TYPE_SETTING this Settings object
     *                     will hold.
     */
    Settings(NMSetting* toAssign, const GType settingType);

    /**
     * @brief  Creates a null Settings object.
     * 
     * @param settingType  The subtype of NM_TYPE_SETTING this Settings object
     *                     will hold.
     */
    Settings(const GType settingType);

    virtual ~Settings() { }
};
