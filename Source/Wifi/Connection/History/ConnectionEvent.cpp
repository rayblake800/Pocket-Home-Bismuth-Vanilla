#include "Wifi/Connection/History/ConnectionEvent.h"

/*
 * Creates a new connection event with initial connection data.
 */
Wifi::ConnectionEvent::ConnectionEvent(
        const AccessPoint eventAP,
        const EventType eventType,
        const juce::Time eventTime) :
eventAP(eventAP),
eventType(eventType),
eventTime(eventTime) { }

/*
 * Creates the connection event as a copy of another event.
 */
Wifi::ConnectionEvent::ConnectionEvent(const ConnectionEvent& rhs) :
eventAP(rhs.eventAP),
eventType(rhs.eventType),
eventTime(rhs.eventTime) { }

/*
 * Creates a null ConnectionEvent object that does not represent a real
 * connection event.
 */
Wifi::ConnectionEvent::ConnectionEvent() : eventType(EventType::invalid) { }

/*
 * Gets the access point involved in the event.
 */
Wifi::AccessPoint Wifi::ConnectionEvent::getEventAP() const
{
    return eventAP;
}

/*
 * Gets the type of event that occurred.
 */
Wifi::EventType Wifi::ConnectionEvent::getEventType() const
{
    return eventType;
}

/*
 * Gets the event's time.
 */
juce::Time Wifi::ConnectionEvent::getEventTime() const
{
    return eventTime;
}

/*
 * Checks if this ConnectionEvent object is null.
 */
bool Wifi::ConnectionEvent::isNull() const
{
    return eventType == Wifi::EventType::invalid;
}
