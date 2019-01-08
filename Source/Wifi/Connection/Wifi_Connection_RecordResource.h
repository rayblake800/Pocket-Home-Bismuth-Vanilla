#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file Wifi_Connection_RecordResource.h
 *
 * @brief  Tracks all major Wifi connection events.
 */
#include "SharedResource_Resource.h"
#include "LibNM/DBus/SavedConnectionLoader.h"
#include "JuceHeader.h"

namespace Wifi { namespace Connection { class RecordResource; }  }
namespace Wifi { namespace Connection { class Event; }  }
namespace Wifi { namespace Connection { enum class EventType; }  }
namespace Wifi { class AccessPoint; }

/**
 *  RecordResource records all Wifi connection events relevant to Pocket-Home
 * that occur after the program is launched. RecordResource also reads saved
 * connections, storing the last connection times of all Wifi connections known
 * to NetworkManager. RecordResource finds and shares events with the most
 * recent stored time, optionally sorting by access point or event type.
 */
class Wifi::Connection::RecordResource : public SharedResource::Resource
{
public:
    /* SharedResource object instance key: */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Reads NetworkManager data to build the initial set of connection 
     *         records.
     */
    RecordResource();

    virtual ~RecordResource() { }

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
     * @param newEvent  A valid connection event to add to the list. If this
     *                  parameter is null, no action will be taken.
     */
    void addEventIfNotDuplicate(const Event newEvent);

    /**
     * @brief  Removes all saved network connections that match a particular
     *         access point.
     *
     * @param toRemove  The access point used to select saved connections to
     *                  delete.
     */
    void removeSavedConnection(const AccessPoint toRemove);

    /**
     * @brief  Connects with NetworkManager to build the initial set of
     *         connection records.
     */
    void updateRecords();

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

private:
    /**
     * @brief  Gets all saved connections compatible with a particular 
     *         AccessPoint object.
     *
     * This method must be called within the LibNM::ThreadResource.
     *
     * @param toMatch  The access point to search for.
     *
     * @return         All saved connections that could have used the provided
     *                 AccessPoint parameter.
     */
    juce::Array<LibNM::SavedConnection> getMatchingConnections
    (const Wifi::AccessPoint toMatch) const;

    /* Reads and removes saved network connections: */
    LibNM::SavedConnectionLoader savedConnections;
};
