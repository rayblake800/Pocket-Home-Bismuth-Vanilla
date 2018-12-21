#define WIFI_IMPLEMENTATION
#include "Wifi/Connection/Event.h"

namespace WifiConnect = Wifi::Connection;

/*
 * Creates a new connection event with initial connection data.
 */
WifiConnect::Event::Event(
        const AccessPoint eventAP,
        const EventType eventType,
        const juce::Time eventTime) :
eventAP(eventAP),
eventType(eventType),
eventTime(eventTime) { }

/*
 * Creates the connection event as a copy of another event.
 */
WifiConnect::Event::Event(const Event& rhs) :
eventAP(rhs.eventAP),
eventType(rhs.eventType),
eventTime(rhs.eventTime) { }

/*
 * Creates a null Event object that does not represent a real connection event.
 */
WifiConnect::Event::Event() : eventType(EventType::invalid) { }

/*
 * Gets the access point involved in the event.
 */
Wifi::AccessPoint WifiConnect::Event::getEventAP() const
{
    return eventAP;
}

/*
 * Gets the type of event that occurred.
 */
WifiConnect::EventType WifiConnect::Event::getEventType() const
{
    return eventType;
}

/*
 * Gets the event's time.
 */
juce::Time WifiConnect::Event::getEventTime() const
{
    return eventTime;
}

/*
 * Checks if this Event object is null.
 */
bool WifiConnect::Event::isNull() const
{
    return eventType == WifiConnect::EventType::invalid;
}
