#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file Wifi/ConnectionHistory/ConnectionHistory.h
 *
 * @brief  Tracks all major Wifi connection events.
 */

#include "Wifi/Connection/History/ConnectionEvent.h"

namespace Wifi { class ConnectionHistory; }
namespace Wifi { class AccessPoint; }

/**
 *  ConnectionHistory records all Wifi connection events relevant to Pocket-Home
 * that occur after the program is launched. ConnectionHistory also reads saved
 * connections, storing the last connection times of all Wifi connections known
 * to NetworkManager. ConnectionHistory finds and shares events with the most
 * recent stored time, optionally sorting by access point or event type.
 */
class Wifi::ConnectionHistory
{
public:
    ConnectionHistory();

    virtual ~ConnectionHistory() { }

    /**
     * @brief  Adds a new event to the list of saved events.
     *
     * @param newEvent  A valid connection event to add to the list. If this
     *                  parameter is null, no action will be taken.
     */
    void addConnectionEvent(const ConnectionEvent newEvent);

    /**
     * @brief  Gets the most recent connection event in the connection history.
     *
     * @return  The stored ConnectionEvent with the most recent time value, or
     *          a null ConnectionEvent if no events were found.
     */
    ConnectionEvent getLastEvent() const;

    /**
     * @brief  Gets the most recent connection event that involves a particular
     *         access point.
     *
     * @param eventAP  The access point to find in the connection history.
     *
     * @return         The most recent stored ConnectionEvent that has eventAP 
     *                 as its access point, or a null ConnectionEvent if no
     *                 stored event has the given access point.
     */
    ConnectionEvent getLastEvent(const AccessPoint eventAP) const;

    /**
     * @brief  Gets the most recent connection event with a specific event type.
     *
     * @param eventType  The requested event type.
     *
     * @return           The most recent stored ConnectionEvent with that type,
     *                   or a null ConnectionEvent if no stored event is of
     *                   type eventType.
     */
    ConnectionEvent getLastEvent(const EventType eventType) const;

    /**
     * @brief  Gets the most recent connection event with a specific event type
     *         and access point.
     *
     * @param eventAP    The access point associated with the requested event.
     *
     * @param eventType  The type of event to find.
     *
     * @return           The most recent ConnectionEvent with that type and
     *                   access point, or a null ConnectionEvent if no matching
     *                   event is found.
     */
    ConnectionEvent getLastEvent(const AccessPoint eventAP,
            const EventType eventType) const;

private:
    /* Holds all recorded connection events. */
    juce::Array<ConnectionEvent> connectionEvents;
};
