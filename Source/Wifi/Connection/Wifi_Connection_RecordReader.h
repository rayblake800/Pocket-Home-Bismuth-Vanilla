#pragma once
/**
 * @file Wifi_Connection_RecordReader.h
 *
 * @brief  Reads Wifi connection events saved in the Connection::RecordResource.
 */

#include "SharedResource_Handler.h"

namespace Wifi { namespace Connection { class RecordReader; } }
namespace Wifi { namespace Connection { class RecordResource; } }
namespace Wifi { namespace Connection { class Event; } }
namespace Wifi { namespace Connection { enum class EventType; } }
namespace Wifi { class AccessPoint; }


class Wifi::Connection::RecordReader : 
    public SharedResource::Handler<RecordResource>
{
public:
    /**
     * @brief  Initializes the RecordResource if necessary.
     */
    RecordReader();

    virtual ~RecordReader() { }
    
    /**
     * @brief  Checks if the system has an active, established Wifi network
     *         connection.
     *
     * @return  Whether the system is connected to the internet through Wifi.
     */
    bool isConnected() const;

    /**
     * @brief  Checks if NetworkManager is currently opening a network 
     *         connection.
     *
     * @return  Whether a network connection is currently being established. 
     */
    bool isConnecting() const;

    /**
     * @brief  Gets the access point being used by the active or activating
     *         connection.
     *
     * @return  The access point used for the connection, or a null AccessPoint
     *          if no connection exists.
     */
    AccessPoint getActiveAP() const;
    
    /**
     * @brief  Checks if NetworkManager has a saved connection that is
     *         compatible with an access point.
     *
     * @param toCheck  The access point to check against the list of saved
     *                 connections.
     *
     * @return  Whether a saved connection exists that uses that access point. 
     */
    bool hasSavedConnection(const AccessPoint toCheck) const;

    /**
     * @brief  Finds the last time the system was fully connected to a 
     *         particular wifi access point's connection.
     *
     * @param connectionAP  A Wifi access point object to search for in
     *                      saved connection records.
     *
     * @return              The last time the system was connected using a 
     *                      connection compatible with connectionAP, or the 
     *                      Unix epoch if no saved connection was found.
     */
    juce::Time lastConnectionTime(const AccessPoint connectionAP) const;

    /**
     * @brief  Gets the most recent connection event in the connection history.
     *
     * @return  The stored Connection::Event with the most recent time value, or
     *          a null Connection::Event if no events were found.
     */
    Event getLastEvent() const;

    /**
     * @brief  Gets the most recent connection event that involves a particular
     *         access point.
     *
     * @param eventAP  The access point to find in the connection history.
     *
     * @return         The most recent stored Connection::Event that has eventAP 
     *                 as its access point, or a null Connection::Event if no
     *                 stored event has the given access point.
     */
    Event getLastEvent(const AccessPoint eventAP) const;

    /**
     * @brief  Gets the most recent connection event with a specific event type.
     *
     * @param eventType  The requested event type.
     *
     * @return           The most recent stored Connection::Event with that 
     *                   type, or a null Event if no stored event is of type 
     *                   eventType.
     */
    Event getLastEvent(const EventType eventType) const;

    /**
     * @brief  Gets the most recent connection event with a specific event type
     *         and access point.
     *
     * @param eventAP    The access point associated with the requested event.
     *
     * @param eventType  The type of event to find.
     *
     * @return           The most recent Event with that type and access point, 
     *                   or a null Event if no matching event is found.
     */
    Event getLastEvent(const AccessPoint eventAP,
            const EventType eventType) const;
};
