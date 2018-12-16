#pragma once
/**
 * @file  LibNM/NMObjects/WifiSettings.h
 *
 * @brief  Holds wireless network connection settings.
 */

#include "NMObjects/Settings.h";
#include <nm-setting-wireless.h>
#include <nm-access-point.h>

namespace LibNM { class WifiSettings; }
namespace LibNM { class SSID; }

class LibNM::WifiSettings : public Settings
{
public:
    WifiSettings(const WifiSettings& toCopy);

    WifiSettings(NMSettingWireless* toAssign);

    WifiSettings();

    SSID getSSID() const;

    NM80211Mode getMode() const;

    juce::StringArray getSeenBSSIDs() const;

    bool isHidden() const;

    void setSSID(const SSID newSSID);

    void setMode(const NM80211Mode newMode);

    void setHidden(const bool isHidden);
};
