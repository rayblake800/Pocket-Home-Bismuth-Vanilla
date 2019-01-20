#define WIFI_IMPLEMENTATION
#include "Wifi_APList_Reader.h"
#include "Wifi_APList_ListResource.h"
#include "Wifi_AccessPoint.h"
#include "LibNM/Data/APHash.h"

Wifi::APList::Reader::Reader() { }

/*
 * Gets Wifi::AccessPoint objects for all visible access points.
 */
juce::Array<Wifi::AccessPoint> Wifi::APList::Reader::getAccessPoints() const
{
    SharedResource::LockedPtr<const ListResource> apList
            = getReadLockedResource();
    return apList->getAccessPoints();
    
}

/*
 * Finds a single Wifi::AccessPoint
 */
Wifi::AccessPoint Wifi::APList::Reader::getAccessPoint
(LibNM::APHash apHash) const
{
    SharedResource::LockedPtr<const ListResource> apList 
            = getReadLockedResource();
    return apList->getAccessPoint(apHash);
}
