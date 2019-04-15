#pragma once
/**
 * @file  Wifi_Connection_Record_Handler.h
 *
 * @brief  Reads saved Wifi connection event records.
 */

#include "SharedResource_Modular_Handler.h"

namespace Wifi
{
    class Resource;
    class AccessPoint;
    namespace Connection
    {
        class Event;
        enum class EventType;
        namespace Record
        {
            class Handler;
            class Module;
        }
    }
}

/**
 * @brief  Connects to the Connection::Record::Module to access the list of
 *         saved Wifi connection events.
 */
class Wifi::Connection::Record::Handler :
    public SharedResource::Modular::Handler<Resource, Module>
{
public:
    /**
     * @brief  Initializes the Wifi::Resource object if necessary.
     */
    Handler();

    virtual ~Handler() { }

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
     * @brief  Gets the most recent connection event in the connection history.
     *
     * @return  The stored Connection::Event with the most recent time value,
     *          or a null Connection::Event if no events were found.
     */
    Event getLatestEvent() const;

    /**
     * @brief  Gets the most recent connection event that involves a particular
     *         access point.
     *
     * @param eventAP  The access point to find in the connection history.
     *
     * @return         The most recent stored Connection::Event that has
     *                 eventAP as its access point, or a null Connection::Event
     *                 if no stored event has the given access point.
     */
    Event getLatestEvent(const AccessPoint eventAP) const;

    /**
     * @brief  Gets the most recent connection event with a specific event type.
     *
     * @param eventType  The requested event type.
     *
     * @return           The most recent stored Connection::Event with that
     *                   type, or a null Event if no stored event is of type
     *                   eventType.
     */
    Event getLatestEvent(const EventType eventType) const;

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
    Event getLatestEvent(const AccessPoint eventAP,
            const EventType eventType) const;

    /**
     * @brief  Connects with NetworkManager to initialize or update the most
     *         recent connection record.
     */
    void updateRecords();
};
