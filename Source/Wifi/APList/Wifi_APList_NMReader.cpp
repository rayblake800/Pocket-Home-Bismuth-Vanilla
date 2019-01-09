#define WIFI_IMPLEMENTATION
#include "Wifi_APList_NMReader.h"
#include "Wifi_APList_ListResource.h"
#include "Wifi_AccessPoint.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"

Wifi::APList::NMReader::NMReader() { }

/*
 * Gets the strongest visible LibNM::AccessPoint that matches a
 * Wifi::AccessPoint.
 */
LibNM::AccessPoint Wifi::APList::NMReader::getStrongestNMAccessPoint
(const AccessPoint accessPoint) const
{
    SharedResource::LockedPtr<ListResource> apList = getReadLockedResource();
    return apList->getStrongestNMAccessPoint(accessPoint);

}

/*
 * Gets all LibNM::AccessPoint objects described by a Wifi::AccessPoint.
 */
juce::Array<LibNM::AccessPoint> Wifi::APList::NMReader::getNMAccessPoints
(const AccessPoint accessPoint) const
{
    SharedResource::LockedPtr<ListResource> apList = getReadLockedResource();
    return apList->getNMAccessPoints();
}
