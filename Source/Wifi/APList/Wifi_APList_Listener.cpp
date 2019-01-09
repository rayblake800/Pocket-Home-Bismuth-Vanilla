#define WIFI_IMPLEMENTATION
#include "Wifi_APList_Listener.h"
#include "Wifi_APList_ListResource.h"
#include "Wifi_AccessPoint.h"

Wifi::APList::Listener::Listener() { }

/*
 * Gets the list of all visible access points from the access point list 
 * resource.
 */
juce::Array<Wifi::AccessPoint> Wifi::APList::Listener::getVisibleAPs() const
{
    SharedResource::LockedPtr<ListResource> apList = getReadLockedResource();
    return apList->getAccessPoints();
}

/*
 * Signals that a new access point was discovered.
 */
void Wifi::APList::Listener::accessPointAdded(const AccessPoint newAP) { }

/*
 * Signals that a tracked access point was lost.
 */
void Wifi::APList::Listener::accessPointRemoved
(const AccessPoint removedAP) { }
