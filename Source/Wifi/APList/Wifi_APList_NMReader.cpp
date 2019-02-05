#define WIFI_IMPLEMENTATION
#include "Wifi_APList_NMReader.h"
#include "Wifi_APList_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_AccessPoint.h"

Wifi::APList::NMReader::NMReader() { }

/*
 * Gets the strongest visible LibNM::AccessPoint that matches a
 * Wifi::AccessPoint.
 */
Wifi::LibNM::AccessPoint 
Wifi::APList::NMReader::getStrongestNMAccessPoint
(const AccessPoint accessPoint) const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> apList 
            = getReadLockedResource();
    return apList->getStrongestNMAccessPoint(accessPoint);
}

/*
 * Gets all LibNM::AccessPoint objects described by a Wifi::AccessPoint.
 */
juce::Array<Wifi::LibNM::AccessPoint> 
Wifi::APList::NMReader::getNMAccessPoints(const AccessPoint accessPoint) const
{
    SharedResource::Modular::LockedPtr<Resource, const Module> apList 
            = getReadLockedResource();
    return apList->getNMAccessPoints(accessPoint);
}
