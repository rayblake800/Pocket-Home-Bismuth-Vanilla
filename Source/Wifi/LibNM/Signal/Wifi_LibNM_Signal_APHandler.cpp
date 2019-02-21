#define WIFI_IMPLEMENTATION
#include "Wifi_LibNM_Signal_APHandler.h"
#include "Wifi_APList_Writer.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_ContextTest.h"

namespace NMSignal = Wifi::LibNM::Signal;

/*
 * Disconnects the signal handler from all access points.
 */
void NMSignal::APHandler::disconnect()
{
    disconnectAll();
}

/*
 * Subscribes to signal strength signals from a single AccessPoint.
 */
void NMSignal::APHandler::connectAllSignals(const AccessPoint source)
{
    ASSERT_NM_CONTEXT;
    createPropertyConnection(NM_ACCESS_POINT_STRENGTH, source);
}

/*
 * Notifies the signal handler of a change in access point signal strength.
 */
void NMSignal::APHandler::signalStrengthChanged
(const AccessPoint updatedAP, unsigned int newStrength) { }

/*
 * Builds signalStrengthChanged() calls from generic property change 
 * notifications.
 */
void NMSignal::APHandler::propertyChanged
(const AccessPoint source, juce::String property) 
{ 
    ASSERT_NM_CONTEXT;
    if(property == NM_ACCESS_POINT_STRENGTH)
    {
        if(!source.isNull())
        {
            unsigned int strength = source.getSignalStrength();
            signalStrengthChanged(source, strength);
        }
    }
}
