#define WIFI_IMPLEMENTATION
#include "Wifi_TestUtils_ConnectionListener.h"
#include "Wifi_Connection_Record_Handler.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix 
        = "Wifi::TestUtils::ConnectionListener::";
#endif

/*
 * Updates the last recorded connection event when updates are received.
 */
void Wifi::TestUtils::ConnectionListener::updateLastRecordedEvent()
{
    const juce::ScopedLock eventLock(eventControl);
    Connection::Record::Handler recordHandler;
    lastEvent = recordHandler.getLatestEvent();
    DBG(dbgPrefix << __func__ << ": New Event:" << lastEvent.toString());
    eventAdded(lastEvent);
}

/*
 * Updates the last recorded event when a new Wifi connection starts opening.
 */
void Wifi::TestUtils::ConnectionListener::startedConnecting
(const Wifi::AccessPoint connectingAP)
{
    updateLastRecordedEvent();
}

/*
 * Updates the last recorded event when connection Wifi authentication fails.
 */
void Wifi::TestUtils::ConnectionListener::connectionAuthFailed
(const Wifi::AccessPoint failedAP)
{
    const juce::ScopedLock eventLock(eventControl);
    updateLastRecordedEvent();
}

/*
 * Updates the last recorded event when a Wifi connection is fully opened.
 */
void Wifi::TestUtils::ConnectionListener::connected
(const Wifi::AccessPoint connectedAP)
{
    updateLastRecordedEvent();
}

/*
 * Updates the last recorded event when a Wifi connection is closed.
 */
void Wifi::TestUtils::ConnectionListener::disconnected
(const Wifi::AccessPoint disconnectedAP)
{
    updateLastRecordedEvent();
}
