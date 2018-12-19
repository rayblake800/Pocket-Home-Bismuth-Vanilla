#define WIFI_IMPLEMENTATION
#include "Wifi/AccessPointList/VisibleAPListener.h"
#include "Wifi/AccessPointList/APList.h"
#include "Wifi/AccessPoint/AccessPoint.h"

Wifi::VisibleAPListener::VisibleAPListener() { }

/*
 * Gets the list of all visible access points from the APList.
 */
juce::Array<Wifi::AccessPoint> Wifi::VisibleAPListener::getVisibleAPs() const
{
    SharedResource::LockedPtr<APList> apList = getReadLockedResource();
    return apList->getAccessPoints();
}

/*
 * Signals that a new access point was discovered.
 */
void Wifi::VisibleAPListener::accessPointAdded(const AccessPoint newAP) { }

/*
 * Signals that a tracked access point was lost.
 */
void Wifi::VisibleAPListener::accessPointRemoved
(const AccessPoint removedAP) { }
