#pragma once
/**
 * @file  LibNM/NMObjects/WifiSettings.h
 *
 * @brief  Holds wireless network connection settings.
 */

#include "LibNM/NMObjects/Settings.h";
#include <nm-setting-wireless.h>
#include <nm-access-point.h>

namespace LibNM { class WifiSettings; }
namespace LibNM { class SSID; }
namespace LibNM { enum class APMode; }

class LibNM::WifiSettings : public Settings
{
public:
    /**
     * @brief  Creates a WifiSettings object that shares data with another
     *         WifiSettings object.
     *
     * @param toCopy  The WifiSettings object that will share its LibNM data
     *                with the new object.
     */
    WifiSettings(const WifiSettings& toCopy);

    /**
     * @brief  Creates a WifiSettings object to hold existing wireless settings
     *         data.
     *
     * @param toAssign  The LibNM wireless settings object that the new 
     *                  WifiSettings object will hold.
     */
    WifiSettings(NMSettingWireless* toAssign);

    /**
     * @brief  Creates a new WifiSettings object holding a new, empty LibNM
     *         wireless settings object.
     */
    WifiSettings();

    virtual ~WifiSettings() { }

    /**
     * @brief  Gets the SSID saved to the wireless connection settings.
     *
     * @return  The main identifier for this WifiSettings object's 
     *          connection.
     */
    SSID getSSID() const;

    /**
     * @brief  Gets the access point mode saved to the wireless connection
     *         settings.
     *
     * @return  The access point mode for this WifiSettings object's
     *          connection.
     */
    APMode getMode() const;

    /**
     * @brief  Gets the list of all base ID values associated with this
     *         WifiSettings object's connection.
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
     * @brief  Changes the SSID saved to tbe Wifi settings.
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
};
