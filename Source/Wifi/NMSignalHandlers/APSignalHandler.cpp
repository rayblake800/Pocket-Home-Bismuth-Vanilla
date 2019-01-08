#define WIFI_IMPLEMENTATION
#include "Wifi/NMSignalHandlers/APSignalHandler.h"
#include "Wifi_AP_AccessPoint.h"
#include "Wifi_APList_ListResource.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"
#include "LibNM/ContextTest.h"

Wifi::APSignalHandler::APSignalHandler() { }

/*
 * Passes signal strength updates to the access point list.
 */
void Wifi::APSignalHandler::signalStrengthChanged
(LibNM::AccessPoint& updatedAP, unsigned int newStrength)
{
    ASSERT_NM_CONTEXT;
    SharedResource::LockedPtr<APList::ListResource> apList 
            = getWriteLockedResource();
    AccessPoint wifiAP = apList->getAccessPoint(updatedAP.generateHash());
    apList->updateSignalStrength(wifiAP);
}
