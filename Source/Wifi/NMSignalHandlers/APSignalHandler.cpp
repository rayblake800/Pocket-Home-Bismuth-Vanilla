#define WIFI_IMPLEMENTATION
#include "Wifi/NMSignalHandlers/APSignalHandler.h"
#include "Wifi/AccessPoint/AccessPoint.h"
#include "Wifi/AccessPointList/APList.h"
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
    SharedResource::LockedPtr<APList> apList = getWriteLockedResource();
    AccessPoint wifiAP = apList->getAccessPoint(updatedAP.generateHash());
    apList->updateSignalStrength(wifiAP);
}
