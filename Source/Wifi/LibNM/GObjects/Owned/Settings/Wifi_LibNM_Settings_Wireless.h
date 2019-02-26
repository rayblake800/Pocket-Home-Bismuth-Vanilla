#pragma once
/**
 * @file  Wifi_LibNM_Settings_Wireless.h
 *
 * @brief  Holds wireless network connection settings.
 */

#include "Wifi_LibNM_Settings_Object.h"
#include <nm-setting-wireless.h>
#include <nm-access-point.h>

namespace Wifi { namespace LibNM 
{ 
    namespace Settings { class Wireless; }
    class SSID;
    enum class APMode;
} }

class Wifi::LibNM::Settings::Wireless : public Object
{
public:
    /**
     * @brief  Creates a wireless settings object that shares data with another
     *         settings object.
     *
     * @param toCopy  The Wireless object that will share its LibNM data
     *                with the new object.
     */
    Wireless(const Wireless& toCopy);

    /**
     * @brief  Creates a Wireless object to hold existing wireless settings
     *         data.
     *
     * @param toAssign  The LibNM wireless settings object that the new 
     *                  Wireless object will hold.
     */
    Wireless(NMSettingWireless* toAssign);

    /**
     * @brief  Creates a new Wireless object holding a new, empty LibNM
     *         wireless settings object.
     */
    Wireless();

    virtual ~Wireless() { }

    /**
     * @brief  Gets the SSID saved to the wireless connection settings.
     *
     * @return  The main identifier for this Wireless object's 
     *          connection.
     */
    SSID getSSID() const;

    /**
     * @brief  Gets the access point mode saved to the wireless connection
     *         settings.
     *
     * @return  The access point mode for this Wireless object's
     *          connection.
     */
    APMode getMode() const;

    /**
     * @brief  Gets the list of all base ID values associated with this
     *         Wireless object's connection.
     *
     * @return  The list of all access point Mac addresses known to be used
     *          by this object's connection.
     */
    juce::StringArray getSeenBSSIDs() const;

    /**
     * @brief  Checks if this object's connection is hidden.
     *
     * @return  Whether the object's connection is a hidden Wifi connection. 
     */
    bool isHidden() const;

    /**
     * @brief  Changes the SSID saved to the Wifi settings.
     *
     * @param newSSID  The new connection ID value to save to Wifi settings.
     */
    void setSSID(const SSID newSSID);

    /**
     * @brief  Changes the access point mode saved to the Wifi settings.
     *
     * @param newMode  The new access point mode to set.
     */
    void setMode(const APMode newMode);

    /**
     * @brief  Sets if the connection is hidden.
     *
     * @param isHidden  Whether this object's connection should be marked as
     *                  hidden.
     */
    void setHidden(const bool isHidden);

private:
    JUCE_LEAK_DETECTOR(Wifi::LibNM::Settings::Wireless);
};
