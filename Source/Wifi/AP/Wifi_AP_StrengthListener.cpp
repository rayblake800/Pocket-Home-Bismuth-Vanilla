#define WIFI_IMPLEMENTATION
#include "Wifi_Resource.h"
#include "Wifi_AP_StrengthListener.h"

/*
 * Creates a StrengthListener tracking all visible access point signal
 * strengths.
 */
Wifi::AP::StrengthListener::StrengthListener() :
    trackedAP(AccessPoint()), handleUpdates(true) { }

/*
 * Creates a StrengthListener tracking a specific access point's signal 
 * strength.
 */
Wifi::AP::StrengthListener::StrengthListener
(const AccessPoint toTrack) : trackedAP(toTrack), handleUpdates(true) { }

/*
 * Sets a single AccessPoint this Listener will track.
 */
void Wifi::AP::StrengthListener::setTrackedAccessPoint
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
void Wifi::AP::StrengthListener::trackAllAccessPoints()
{
    const juce::ScopedLock updateLock(updateGuard);
    trackedAP = AccessPoint();
    handleUpdates = true;
}

/*
 * Sets the Listener to ignore all signal strength updates.
 */
void Wifi::AP::StrengthListener::ignoreAllUpdates()
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
void Wifi::AP::StrengthListener::signalStrengthChanged
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
