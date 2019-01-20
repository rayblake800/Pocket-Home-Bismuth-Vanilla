#define WIFI_IMPLEMENTATION
#include "Wifi_Connection_Event.h"

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
 * Makes this event into a copy of another event. 
 */
WifiConnect::Event& WifiConnect::Event::operator=(const Event& rhs)
{
    eventAP = rhs.eventAP;
    eventType = rhs.eventType;
    eventTime = rhs.eventTime;
    return *this;
}

/*
 * Checks if two connection events are equivalent.
 */
bool WifiConnect::Event::operator==(const Event& rhs) const
{
    return eventAP == rhs.eventAP
        && eventType == rhs.eventType
        && eventTime == rhs.eventTime;
}


/*
 * Checks if two connection events are not equivalent.
 */
bool WifiConnect::Event::operator!=(const Event& rhs) const
{
    return !(*this == rhs);
}

/*
 * Compares two connection Event objects using their event times.
 */
bool WifiConnect::Event::operator<(const Event& rhs) const
{
    return eventTime.toMilliseconds() < rhs.eventTime.toMilliseconds();
}

/*
 * Checks if this Event object is null.
 */
bool WifiConnect::Event::isNull() const
{
    return eventType == WifiConnect::EventType::invalid
        || eventAP.isNull() || eventTime.toMilliseconds() == 0;
}

/*
 * Represents this Event object as a string, usable for debugging.
 */
juce::String WifiConnect::Event::toString() const
{
    if(isNull())
    {
        return "Null Event";
    }

    juce::String eventString = "AP:[";
    if(eventAP.isNull())
    {
        eventString += "Null AP";
    }
    else
    {
        eventString += eventAP.getSSID().toString();
    }
    eventString += "], Type:[";
    switch(eventType)
    {
        case WifiConnect::EventType::connected:
            eventString += "connected";
            break;
        case WifiConnect::EventType::connectionRequested:
            eventString += "connectionRequested";
            break;
        case WifiConnect::EventType::connectionFailed:
            eventString += "connectionFailed";
            break;
        case WifiConnect::EventType::connectionAuthFailed:
            eventString += "connectionAuthFailed";
            break;
        case WifiConnect::EventType::disconnected:
            eventString += "disconnected";
            break;
        case WifiConnect::EventType::startedConnecting:
            eventString += "startedConnecting";
            break;
        case WifiConnect::EventType::invalid:
            eventString += "invalid";
    }
    eventString += "], Time:[";
    eventString += eventTime.toString(true, true, true, true);
    eventString += "]";
    return eventString;
}
