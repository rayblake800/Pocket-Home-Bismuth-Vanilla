#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file Wifi/ConnectionHistory/ConnectionHistory.h
 *
 * @brief  Tracks all major Wifi connection events.
 */

#include "SharedResource/Resource.h"
#include "Wifi/ConnectionHistory/ConnectionEvent.h"

namespace Wifi { class ConnectionTracker; }
namespace Wifi { class AccessPoint; }

class Wifi::ConnectionTracker : public SharedResource::Resource
{
public:
    /* SharedResource object instance key: */
    static const juce::Identifier resourceKey;

    ConnectionTracker();

    virtual ~ConnectionTracker() { }

    void addConnectionEvent(const ConnectionEvent newEvent);

    ConnectionEvent getLastEvent() const;

    ConnectionEvent getLastEvent(const AccessPoint eventAP) const;

    ConnectionEvent getLastEvent(const EventType eventType) const;

    ConnectionEvent getLastEvent(const AccessPoint eventAP,
            const EventType eventType) const;

private:
    juce::Array<ConnectionEvent> connectionEvents;
};
