#pragma once
#include "Wifi/AccessPoint/AccessPoint.h"

namespace Wifi { class ConnectionEvent; }
namespace Wifi { enum class EventType; }

enum class Wifi::EventType
{
    connectionRequested,
    startedConnecting,
    connectionAuthFailed,
    connected,
    disconnected,
    connectionLost
};

class Wifi::ConnectionEvent
{
public:
    ConnectionEvent(const AccessPoint eventAP, const EventType eventType,
            const juce::Time eventTime = juce::Time::getCurrentTime());

    virtual ~ConnectionEvent() { }

    AccessPoint getEventAP() const;
    
    EventType getEventType() const;

    juce::Time getEventTime() const;

    ConnectionEvent& operator=(const ConnectionEvent& rhs);

    bool operator==(const ConnectionEvent& rhs) const;

    bool operator!=(const ConnectionEvent& rhs) const;

    bool operator<(const ConnectionEvent& rhs) const;

    bool isNull() const;

private:

    AccessPoint eventAP;

    EventType eventType;

    juce::Time eventTime;
};
