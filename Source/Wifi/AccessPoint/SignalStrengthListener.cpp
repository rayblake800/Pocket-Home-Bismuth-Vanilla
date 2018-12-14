#include "Wifi/AccessPoint/SignalStrengthListener.h"
#include "Wifi/AccessPointList/APList.h"

/*
 * Creates a SignalStrengthListener tracking all visible access point signal
 * strengths.
 */
Wifi::SignalStrengthListener::SignalStrengthListener() :
    trackedAP(AccessPoint()), handleUpdates(true) { }

/*
 * Sets a single AccessPoint this Listener will track.
 */
void Wifi::SignalStrengthListener::setTrackedAccessPoint
(const AccessPoint toTrack)
{
    trackedAP.set(toTrack);
    handleUpdates.set(true);
}

/*
 * Sets the Listener to receive updates when any AccessPoint signal strength
 * updates.
 */
void Wifi::SignalStrengthListener::trackAllAccessPoints()
{
    trackedAP.set(AccessPoint());
    handleUpdates.set(true);
}

/*
 * Sets the Listener to ignore all signal strength updates.
 */
void Wifi::SignalStrengthListener::ignoreAllUpdates()
{
    trackedAP.set(AccessPoint());
    handleUpdates.set(false);

}

/*
 * Receives all signal strength notifications, passing them on to the
 * signalStrengthUpdate method if the changed access point is tracked by the
 * Listener.
 */
void Wifi::SignalStrengthListener::signalStrengthChanged
(const AccessPoint updatedAP)
{
    if(handleUpdates.get())
    {
        AccessPoint tracked = trackedAP.get();
        if(tracked.isNull() || tracked == updatedAP)
        {
            signalStrengthUpdate(updatedAP);
        }
    }
}
