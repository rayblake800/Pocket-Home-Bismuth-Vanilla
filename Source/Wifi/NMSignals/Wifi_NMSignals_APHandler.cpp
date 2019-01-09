#define WIFI_IMPLEMENTATION
#include "Wifi_NMSignals_APHandler.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_APList_ListResource.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/ContextTest.h"

Wifi::NMSignals::APHandler::APHandler() { }

/*
 * Passes signal strength updates to the access point list.
 */
void Wifi::NMSignals::APHandler::signalStrengthChanged
(LibNM::AccessPoint& updatedAP, unsigned int newStrength)
{
    ASSERT_NM_CONTEXT;
    SharedResource::LockedPtr<APList::ListResource> apList 
            = getWriteLockedResource();
    AccessPoint wifiAP = apList->getAccessPoint(updatedAP.generateHash());
    apList->updateSignalStrength(wifiAP);
}
