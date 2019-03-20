#pragma once
/**
 * @file  Wifi_LibNM_Settings_Object.h
 *
 * @brief  Stores a set of related network connection settings associated with a
 *         LibNM::Connection.
 */

#include "GLib_Owned_Object.h"
#include <nm-connection.h>

namespace Wifi { namespace LibNM { namespace Settings { class Object; } } }

/**
 * @brief  Manages a NMSetting* value, used to store network connection 
 *         settings.
 *
 *  NMSetting is an abstract type inherited by many specialized connection 
 * setting types. Wifi::LibNM classes that manage any type of NMSetting value
 * should inherit Settings::Object.
 */
class Wifi::LibNM::Settings::Object : public GLib::Owned::Object
{
public:
    /**
     * @brief  Creates a Settings::Object to contain NMSetting data.
     *
     * @param toAssign     A valid NMSetting pointer for this Settings::Object 
     *                     to hold, or nullptr to create a null Object.
     * 
     * @param settingType  The subtype of NM_TYPE_SETTING this object will hold.
     */
    Object(NMSetting* toAssign, const GType settingType);

    /**
     * @brief  Creates a null Settings object.
     * 
     * @param settingType  The subtype of NM_TYPE_SETTING this Settings object
     *                     will hold.
     */
    Object(const GType settingType);

    virtual ~Object() { }

    JUCE_LEAK_DETECTOR(Wifi::LibNM::Settings::Object);
};
