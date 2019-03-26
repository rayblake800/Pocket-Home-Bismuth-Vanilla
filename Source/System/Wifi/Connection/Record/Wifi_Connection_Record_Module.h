#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file Wifi_Connection_Record_Module.h
 *
 * @brief  Tracks all major Wifi connection events.
 */
#include "Wifi_Module.h"

namespace Wifi 
{ 
    class Resource;
    class AccessPoint;
    namespace Connection 
    { 
        class Event;
        enum class EventType;
        namespace Record { class Module; } 
    } 
}

/**
 * @brief  Records all Wifi connection events relevant to this application that 
 *         occur after the program is launched. 
 *
 *  RecordModule finds and shares the most recent connection events, optionally 
 * filtering by access point or event type.
 */
class Wifi::Connection::Record::Module final : public Wifi::Module
{
public:
    /**
     * @brief  Connects the module to its Resource.
     *
     * @param parentResource  The Wifi::Resource object instance.
     */
    Module(Resource& parentResource);

    virtual ~Module() { }

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
     * @brief  Adds a new event to the list of saved events.
     *
     * @param newEvent  A valid connection event to add to the list. If this
     *                  parameter is null, no action will be taken.
     */
    void addConnectionEvent(const Event newEvent);

    /**
     * @brief  Adds a new event to the list of saved events if the most recent 
     *         saved event doesn't have the same access point and event type.
     *
     *  For the purposes of this function, disconnect, connection failure, and
     * authentication failure are treated as equivalent types.
     *
     * @param newEvent  A valid connection event to add to the list. If this
     *                  parameter is null, no action will be taken.
     */
    void addEventIfNotDuplicate(const Event newEvent);

    /**
     * @brief  Connects with NetworkManager to initialize or update the most
     *         recent connection record.
     */
    void updateRecords();

    /**
     * @brief  Gets the most recent connection event in the connection history.
     *
     * @return  The stored Connection::Event with the most recent time value, or
     *          a null Connection::Event if no events were found.
     */
    Event getLatestEvent() const;

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
};
