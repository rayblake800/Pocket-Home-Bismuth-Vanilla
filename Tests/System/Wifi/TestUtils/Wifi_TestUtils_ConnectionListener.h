#pragma once
/**
 * @file  Wifi_TestUtils_ConnectionListener.h
 *
 * @brief  Listens for all new Wifi connection events that are added to the 
 *         Wifi::Connection::Record module.
 */

#include "Wifi_Connection_Record_Listener.h"
#include "Wifi_Connection_Event.h"
#include "JuceHeader.h"

namespace Wifi { namespace TestUtils { class ConnectionListener; } }

/**
 * @brief  Listens for recorded Wifi connection events.
 */
class Wifi::TestUtils::ConnectionListener : 
    public Wifi::Connection::Record::Listener
{
public:
    ConnectionListener() { }

    virtual ~ConnectionListener() { }

    /**
     * @brief  Handles the addition of a new event to the 
     *         Connection::Record::Module.
     *
     * @param newEvent  The new Wifi event registered to the connection record.
     */
    virtual void eventAdded(const Wifi::Connection::Event newEvent) = 0;

protected:
    /**
     * @brief  Updates the last recorded event when a new Wifi connection starts 
     *         opening.
     *
     * @param connectingAP  The access point being used by the attempted
     *                      connection.
     */
    virtual void startedConnecting
    (const Wifi::AccessPoint connectingAP) override;

    /**
     * @brief  Updates the last recorded event when connection Wifi 
     *         authentication fails. 
     *
     * @param failedAP  The access point used in the connection event that
     *                  failed to authenticate.
     */
    virtual void connectionAuthFailed
    (const Wifi::AccessPoint failedAP) override;

    /**
     * @brief  Updates the last recorded event when a Wifi connection is fully 
     *         opened. 
     * 
     * @param connectedAP  The access point used to establish the new 
     *                     connection.
     */
    virtual void connected(const Wifi::AccessPoint connectedAP) override;

    /**
     * @brief  Updates the last recorded event when a Wifi connection is closed.
     *
     * @param disconnectedAP  The access point used by the connection that was
     *                        closed.
     */
    virtual void disconnected(const Wifi::AccessPoint disconnectedAP) override;

    /**
     * @brief  Gets the last event recorded by this listener.
     *
     * @return  The last recorded Wifi connection event.
     */
    Wifi::Connection::Event getLastEvent() const
    {
        const juce::ScopedLock eventLock(eventControl);
        return lastEvent;
    }

private:
    /**
     * @brief  Updates the last recorded connection event when updates are
     *         received.
     */
    void updateLastRecordedEvent();

    /* Allows threads to safely share access to the listener: */
    juce::CriticalSection eventControl;
    /* Stores the last Wifi connection event the listener received: */
    Wifi::Connection::Event lastEvent;
};
