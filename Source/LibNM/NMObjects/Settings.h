#pragma once
/**
 * @file  LibNM/NMObjects/Settings.h
 *
 * @brief  Stores a set of related network connection settings associated with a
 *         LibNM::Connection.
 */

#include "LibNM/NMObjects/Object.h"

namespace LibNM { class Settings; }
struct NMSetting;

class LibNM::Settings : public LibNM::Object
{
protected:
    /**
     * @brief  Creates a Settings object sharing data with another Settings
     *         object.
     *
     * @param toCopy       Another Settings object to copy.
     * 
     * @param settingType  The subtype of NM_TYPE_SETTING this Settings object
     *                     will hold.
     */
    Settings(const Settings& toCopy, const GType settingType);

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

public:
    virtual ~Settings() { }
};

