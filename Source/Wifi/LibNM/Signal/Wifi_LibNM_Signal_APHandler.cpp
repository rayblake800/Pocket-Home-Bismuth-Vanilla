#define WIFI_IMPLEMENTATION
#include "Wifi_LibNM_Signal_APHandler.h"
#include "Wifi_APList_Writer.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_ContextTest.h"

namespace NMSignal = Wifi::LibNM::Signal;

/**
 * @brief  Disconnects the signal handler from all access points.
 */
void NMSignal::APHandler::disconnect()
{
    unsubscribeAll();
}

/*
 * Subscribes to signal strength signals from a single AccessPoint.
 */
void NMSignal::APHandler::connectAllSignals(AccessPoint& source)
{
    ASSERT_NM_CONTEXT;
    createPropertyConnection(NM_ACCESS_POINT_STRENGTH, source);
}

/*
 * Passes signal strength updates to the access point list.
 */
void NMSignal::APHandler::signalStrengthChanged
(LibNM::AccessPoint& updatedAP, unsigned int newStrength)
{
    ASSERT_NM_CONTEXT;
    DBG("Wifi::LibNM::Signal::APHandler::" << __func__ 
            << ": Signal strength change for " << updatedAP.getSSIDText()
            << " to strength " << (int) newStrength);
    APList::Writer apListWriter;
    apListWriter.updateSignalStrength(updatedAP);
}

/*
 * Builds signalStrengthChanged() calls from generic property change 
 * notifications.
 */
void NMSignal::APHandler::propertyChanged
(AccessPoint& source, juce::String property) 
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
