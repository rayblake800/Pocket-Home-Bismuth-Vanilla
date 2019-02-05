#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Connection_Record_Writer.h
 *
 * @brief  Updates the Record::Module's Wifi event records.
 */
#include "SharedResource_Modular_Handler.h"

namespace Wifi { namespace Connection {  namespace Record { class Writer; } } }
namespace Wifi { namespace Connection {  namespace Record { class Module; } } }
namespace Wifi { class Resource; }
namespace Wifi { namespace Connection { class Event; }  }
namespace Wifi { namespace Connection { enum class EventType; }  }
namespace Wifi { class AccessPoint; }


class Wifi::Connection::Record::Writer : 
        public SharedResource::Modular::Handler<Resource, Module>
{
public:
    Writer();

    virtual ~Writer() { }

    /**
     * @brief  Connects to NetworkManager to rebuild the list of connection 
     *         records.
     */
    void updateRecords() const;

    /**
     * @brief  Adds a new event to the list of saved events.
     *
     * @param newEvent  A valid connection event to add to the list. If this
     *                  parameter is null, no action will be taken.
     */
    void addConnectionEvent(const Event newEvent) const;

    /**
     * @brief  Adds a new event to the list of saved events.
     *
     * @param eventAP    A non-null access point associated with the new event.
     *
     * @param eventType  A valid event type to save.
     *
     * @param eventTime  An optional event timestamp. If not provided, the
     *                   current time will be used.
     */
    void addConnectionEvent(
            const AccessPoint eventAP, 
            const EventType eventType, 
            const juce::Time eventTime = juce::Time::getCurrentTime());

    /**
     * @brief  Adds a new event to the list of saved events if the most recent 
     *         saved event doesn't have the same access point and event type.
     *
     * @param newEvent  A valid connection event to add to the list. If this
     *                  parameter is null, no action will be taken.
     */
    void addEventIfNotDuplicate(const Event newEvent);

    /**
     * @brief  Adds a new event to the list of saved events if the most recent 
     *         saved event doesn't have the same access point and event type.
     *
     * @param eventAP    A non-null access point associated with the new event.
     *
     * @param eventType  A valid event type to save.
     *
     * @param eventTime  An optional event timestamp. If not provided, the
     *                   current time will be used.
     */
    void addEventIfNotDuplicate(
            const AccessPoint eventAP, 
            const EventType eventType, 
            const juce::Time eventTime = juce::Time::getCurrentTime());
};
