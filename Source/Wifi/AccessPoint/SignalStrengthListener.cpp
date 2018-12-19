#define WIFI_IMPLEMENTATION
#include "Wifi/AccessPoint/SignalStrengthListener.h"
#include "Wifi/AccessPointList/APList.h"

/*
 * Creates a SignalStrengthListener tracking all visible access point signal
 * strengths.
 */
Wifi::SignalStrengthListener::SignalStrengthListener() :
    trackedAP(AccessPoint()), handleUpdates(true) { }

/*
 * Creates a SignalStrengthListener tracking a specific access point's signal 
 * strength.
 */
Wifi::SignalStrengthListener::SignalStrengthListener
(const AccessPoint toTrack) : trackedAP(toTrack), handleUpdates(true) { }

/*
 * Sets a single AccessPoint this Listener will track.
 */
void Wifi::SignalStrengthListener::setTrackedAccessPoint
(const AccessPoint toTrack)
{
    const juce::ScopedLock updateLock(updateGuard);
    trackedAP = toTrack;
    handleUpdates = true;
}

/*
 * Sets the Listener to receive updates when any AccessPoint signal strength
 * updates.
 */
void Wifi::SignalStrengthListener::trackAllAccessPoints()
{
    const juce::ScopedLock updateLock(updateGuard);
    trackedAP = AccessPoint();
    handleUpdates = true;
}

/*
 * Sets the Listener to ignore all signal strength updates.
 */
void Wifi::SignalStrengthListener::ignoreAllUpdates()
{
    const juce::ScopedLock updateLock(updateGuard);
    trackedAP = AccessPoint();
    handleUpdates = false;
}

/*
 * Receives all signal strength notifications, passing them on to the
 * signalStrengthUpdate method if the changed access point is tracked by the
 * Listener.
 */
void Wifi::SignalStrengthListener::signalStrengthChanged
(const AccessPoint updatedAP)
{
    const juce::ScopedLock updateLock(updateGuard);
    if(handleUpdates)
    {
        if(trackedAP.isNull() || trackedAP == updatedAP)
        {
            signalStrengthUpdate(updatedAP);
        }
    }
}
