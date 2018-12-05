#pragma once
#include "Wifi/SavedState/ConnectionEvent.h"
#include "SharedResource/Resource.h"

namespace Wifi { class ConnectionTracker; }
namespace Wifi { class AccessPoint; }

class Wifi::ConnectionTracker : public SharedResource::Resource
{
public:
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
