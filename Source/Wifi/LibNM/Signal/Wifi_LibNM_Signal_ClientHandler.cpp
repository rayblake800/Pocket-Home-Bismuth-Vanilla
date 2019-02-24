#define WIFI_IMPLEMENTATION
#include "Wifi_LibNM_Signal_ClientHandler.h"
#include "Wifi_LibNM_Client.h"
#include "Wifi_Device_Module.h"
#include "Wifi_Resource.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_Thread_Handler.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_LibNM_ContextTest.h"
#include "SharedResource_Modular_Handler.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Wifi::LibNM::Signal::ClientHandler::";
#endif

namespace NMSignal = Wifi::LibNM::Signal;

/*
 * Notifies the ClientHandler when wireless networking is enabled or disabled.
 */
void NMSignal::ClientHandler::wirelessStateChange(bool wifiEnabled) { }

/*
 * Subscribes to all relevant signals from a single Client signal source.
 */
void NMSignal::ClientHandler::connectAllSignals(const Client source)
{
    ASSERT_NM_CONTEXT;
    if(!source.isNull())
    {
        createPropertyConnection(NM_CLIENT_WIRELESS_ENABLED, source);
    }
}

/*
 * Converts generic propertyChanged calls to class-specific wirelessStateChange 
 * calls.
 */
void NMSignal::ClientHandler::propertyChanged
(const Client source, juce::String property)
{ 
    ASSERT_NM_CONTEXT;
    if(!source.isNull() && property == NM_CLIENT_WIRELESS_ENABLED)
    {
        const bool enabled = source.wirelessEnabled();
        wirelessStateChange(enabled);
    }
}
