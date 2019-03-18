#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_AP_UpdateInterface.h
 *
 * @brief  The interface used by the Wifi::Signal::APModule to send updates 
 *         to all Wifi::AP::StrengthListener objects.
 */

namespace Wifi { namespace AP { class UpdateInterface; } }
namespace Wifi { class AccessPoint; }

class Wifi::AP::UpdateInterface
{
public:
    UpdateInterface() { }

    virtual ~UpdateInterface() { }

    /**
     * @brief  Called whenever a AccessPoint object's signal strength changes.
     *
     * @param updatedAP  The access point with a new signal strength value.
     */
    virtual void signalStrengthChanged(const AccessPoint updatedAP) = 0;
};
