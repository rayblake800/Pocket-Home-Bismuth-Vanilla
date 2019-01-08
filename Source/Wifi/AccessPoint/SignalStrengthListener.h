#pragma once
#include "Wifi/AccessPoint/SignalUpdateInterface.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "SharedResource_Handler.h"

/**
 * @file  Wifi/Listeners/SignalStrengthListener.h
 *
 * @brief  Receives updates whenever Wifi access point signal strength changes.
 *
 *  SignalStrengthListener may choose to receive updates from all AccessPoint
 * objects, receive updates from a specific AccessPoint, or ignore all 
 * signal strength updates. This choice may be changed at any time.
 */

namespace Wifi { class SignalStrengthListener; }
namespace Wifi { namespace APList { class ListResource; } }

class Wifi::SignalStrengthListener : public SignalUpdateInterface,
    public SharedResource::Handler<APList::ListResource>
{
public:
    /**
     * @brief  Creates a SignalStrengthListener tracking all visible access 
     *         point signal strengths.
     */
    SignalStrengthListener();
    
    /**
     * @brief  Creates a SignalStrengthListener tracking a specific access
     *         point's signal strength.
     *
     * @param toTrack  The access point this Listener will track.
     */
    SignalStrengthListener(const AccessPoint toTrack);
    
    /**
     * @brief  Sets a single AccessPoint this Listener will track.
     *
     * @param toTrack  The only AccessPoint this Listener needs to recieve
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
     *  By default, this takes no action. Override to handle the signal strength
     * update event.
     *
     * @param updatedAP  The AccessPoint with a new signal strength value.
     */
    virtual void signalStrengthUpdate(const AccessPoint updatedAP) { }

    /**
     * @brief  Receives all signal strength notifications, passing them on to
     *         the signalStrengthUpdate method if the changed access point is
     *         tracked by the Listener.
     *
     * @param updatedAP  The AccessPoint with a new signal strength value.
     */
    virtual void signalStrengthChanged(const AccessPoint updatedAP)
        final override;
    
    /* If non-null, selects a specific access point this listener will track. */
    AccessPoint trackedAP;

    /* If false, all signal strength updates will be ignored. */
    bool handleUpdates;

    /* Prevent concurrent access when updating or changing update options. */
    const juce::CriticalSection updateGuard;
};
