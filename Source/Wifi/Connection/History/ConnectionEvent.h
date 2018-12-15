#pragma once
/**
 * @file  Wifi/ConnectionHistory/ConnectionEvent.h
 *
 * @brief  Stores the wifi access point, time, and type of a Wifi connection
 *         event.
 */
#include "Wifi/AccessPoint/AccessPoint.h"

namespace Wifi { class ConnectionEvent; }
namespace Wifi { enum class EventType; }

/**
 * @brief  All types of event tracked using ConnectionEvent.
 */
enum class Wifi::EventType
{
    /* A new connection was requested through the Wifi::ConnectionManager. */
    connectionRequested,
    /* NetworkManager is attempting to open a connection. */ 
    startedConnecting,
    /* A connection attempt was stopped due to an incorrect or missing network
       key. */
    connectionAuthFailed,
    /* A new connection opened successfully. */
    connected,
    /* The active connection was closed or lost. */
    disconnected,
    /* The ConnectionEvent does not represent a connection event. */
    invalid
};

class Wifi::ConnectionEvent
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
    ConnectionEvent(const AccessPoint eventAP, const EventType eventType,
            const juce::Time eventTime = juce::Time::getCurrentTime());

    /**
     * @brief  Creates the connection event as a copy of another event.
     *
     * @param rhs  The other connection event to copy.
     */
    ConnectionEvent(const ConnectionEvent& rhs);

    /**
     * @brief  Creates a null ConnectionEvent object that does not represent
     *         a real connection event.
     */
    ConnectionEvent();

    virtual ~ConnectionEvent() { }

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
    ConnectionEvent& operator=(const ConnectionEvent& rhs);

    /**
     * @brief  Checks if two connection events are equivalent.
     *
     * @param rhs  Another saved ConnectionEvent.
     *
     * @return     Whether both ConnectionEvents have identical AccessPoint
     *             objects, event times, and event types.
     */
    bool operator==(const ConnectionEvent& rhs) const;


    /**
     * @brief  Checks if two connection events are not equivalent.
     *
     * @param rhs  Another saved ConnectionEvent.
     *
     * @return     Whether the ConnectionEvents have different AccessPoint
     *             objects, event times, or event types.
     */
    bool operator!=(const ConnectionEvent& rhs) const;

    /**
     * @brief  Compares two ConnectionEvent objects using their event times.
     *
     * @param rhs  Another saved ConnectionEvent.
     *
     * @return     Whether this connection event happened before the rhs
     *             ConnectionEvent.
     */
    bool operator<(const ConnectionEvent& rhs) const;

    /**
     * @brief  Checks if this ConnectionEvent object is null.
     *
     * @return  Whether this ConnectionEvent is a null object that does not
     *          represent an event.
     */
    bool isNull() const;

private:

    /* The access point used in the connection event. */
    AccessPoint eventAP;

    /* The type of event this object records. */
    EventType eventType;

    /* The time the event occurred. */
    juce::Time eventTime;
};
