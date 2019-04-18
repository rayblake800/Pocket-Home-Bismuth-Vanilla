#pragma once
/**
 * @file  Wifi_AP_StrengthListener.h
 *
 * @brief  Provides a basis for classes that should receive updates whenever
 *         Wifi access point signal strength changes.
 */

#ifndef WIFI_IMPLEMENTATION
    #define STRENGTH_LISTENER_IMPLEMENTATION
    #define WIFI_IMPLEMENTATION
#endif
#include "Wifi_AP_UpdateInterface.h"
#ifdef STRENGTH_LISTENER_IMPLEMENTATION
    #undef STRENGTH_LISTENER_IMPLEMENTATION
    #undef WIFI_IMPLEMENTATION
#endif
#include "Wifi_AccessPoint.h"
#include "SharedResource_Handler.h"

namespace Wifi { namespace AP { class StrengthListener; } }
namespace Wifi { class Resource; }

/**
 * @brief  Tracks the signal strength of Wifi access points.
 *
 *  StrengthListener may choose to receive updates from all AccessPoint
 * objects, receive updates from a specific AccessPoint, or ignore all signal
 * strength updates.
 */
class Wifi::AP::StrengthListener : public UpdateInterface,
    public SharedResource::Handler<Resource>
{
public:
    /**
     * @brief  Creates a StrengthListener tracking all visible access point
     *         signal strengths.
     */
    StrengthListener();

    /**
     * @brief  Creates a StrengthListener tracking a specific access point's
     *         signal strength.
     *
     * @param toTrack  The access point this Listener will track.
     */
    StrengthListener(const AccessPoint toTrack);

    /**
     * @brief  Sets a single AccessPoint this Listener will track.
     *
     * @param toTrack  The only AccessPoint this Listener needs to receive
     *                 signal strength updates from.
     */
    void setTrackedAccessPoint(const AccessPoint toTrack);

    /**
     * @brief  Sets the Listener to receive updates when any AccessPoint signal
     *         strength updates.
     */
    void trackAllAccessPoints();

    /**
     * @brief  Sets the Listener to ignore all signal strength updates.
     */
    void ignoreAllUpdates();

private:
    /**
     * @brief  Notifies the Listener of a signal strength update.
     *
     *  By default, this takes no action. Override to handle the signal
     * strength update event.
     *
     * @param updatedAP  The AccessPoint with a new signal strength value.
     */
    virtual void signalStrengthUpdate(const AccessPoint updatedAP) { }

    /**
     * @brief  Receives all signal strength notifications, passing them on to
     *         the signalStrengthUpdate function if the changed access point is
     *         tracked by the Listener.
     *
     * @param updatedAP  The AccessPoint with a new signal strength value.
     */
    virtual void signalStrengthChanged(const AccessPoint updatedAP)
        final override;

    // If non-null, selects a specific access point this listener will track.
    AccessPoint trackedAP;

    // If false, all signal strength updates will be ignored.
    bool handleUpdates;

    // Prevent concurrent access when updating or changing update options.
    const juce::CriticalSection updateGuard;
};
