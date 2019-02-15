#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Connection_Event.h
 *
 * @brief  Stores the wifi access point, time, and type of a Wifi connection
 *         event.
 */
#include "Wifi_AccessPoint.h"

namespace Wifi 
{ 
    namespace Connection 
    { 
        class Event; 
        enum class EventType;

        /**
         * @brief  Gets a string representation of a Wifi connection event type.
         *
         * @param type  The event type to convert.
         *
         * @return      The event type's string representation.
         */
        juce::String eventTypeString(const EventType type);
    } 
}

/**
 * @brief  All types of event tracked using Connection::Event.
 */
enum class Wifi::Connection::EventType
{
    /* A new connection was requested. */
    connectionRequested,
    /* NetworkManager is attempting to open a connection. */ 
    startedConnecting,
    /* A connection attempt was stopped due to an incorrect or missing network
       key. */
    connectionAuthFailed,
    /* A connection event failed for some other reason. */
    connectionFailed,
    /* A new connection opened successfully. */
    connected,
    /* The active connection was closed or lost. */
    disconnected,
    /* The ConnectionEvent does not represent a connection event. */
    invalid
};


class Wifi::Connection::Event
{
public:
    /**
     * @brief  Creates a new connection event with initial connection data.
     *
     * @param eventAP    The access point involved in the event.
     *
     * @param eventType  The specific type of connection event.
     *
     * @param eventTime  The time when the event occurred. If omitted, this
     *                   uses the current time by default.
     */
    Event(const AccessPoint eventAP, const EventType eventType,
            const juce::Time eventTime = juce::Time::getCurrentTime());

    /**
     * @brief  Creates the connection event as a copy of another event.
     *
     * @param rhs  The other connection event to copy.
     */
    Event(const Event& rhs);

    /**
     * @brief  Creates a null Event object that does not represent a real 
     *         connection event.
     */
    Event();

    virtual ~Event() { }

    /**
     * @brief  Gets the access point involved in the event.
     *
     * @return  The connected, connecting, or disconnecting access point 
     *          relevant to the event.
     */
    AccessPoint getEventAP() const;
    
    /**
     * @brief  Gets the type of event that occurred.
     *
     * @return  The type of event this object is recording.
     */
    EventType getEventType() const;

    /**
     * @brief  Gets the event's time.
     *
     * @return   The approximate time when the event happened.
     */
    juce::Time getEventTime() const;

    /**
     * @brief  Makes this event into a copy of another event. 
     *
     * @param rhs  The other event to copy.
     *
     * @return     This ConnectionEvent object.
     */
    Event& operator=(const Event& rhs);

    /**
     * @brief  Checks if two connection events are equivalent.
     *
     * @param rhs  Another saved Event.
     *
     * @return     Whether both Event objectss have identical AccessPoint
     *             objects, event times, and event types.
     */
    bool operator==(const Event& rhs) const;


    /**
     * @brief  Checks if two connection events are not equivalent.
     *
     * @param rhs  Another saved Event object.
     *
     * @return     Whether the Event objectss have different AccessPoint
     *             objects, event times, or event types.
     */
    bool operator!=(const Event& rhs) const;

    /**
     * @brief  Compares two connection Event objects using their event times.
     *
     * @param rhs  Another saved Event.
     *
     * @return     Whether this connection event happened before the rhs
     *             Event.
     */
    bool operator<(const Event& rhs) const;

    /**
     * @brief  Checks if this Event object is null.
     *
     * @return  Whether this Event is a null object that does not represent an 
     *          actual connection event.
     */
    bool isNull() const;

    /**
     * @brief  Represents this Event object as a string, usable for debugging.
     *
     * @return  All event data in string format.
     */
    juce::String toString() const;

private:

    /* The access point used in the connection event. */
    AccessPoint eventAP;

    /* The type of event this object records. */
    EventType eventType;

    /* The time the event occurred. */
    juce::Time eventTime;
};
