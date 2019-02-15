#define WIFI_IMPLEMENTATION
#include "Wifi_TestUtils_Waiting.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_APHash.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_APList_Reader.h"
#include "Wifi_Connection_Record_Reader.h"
#include "DelayUtils.h"

/* Number of milliseconds to wait for access points to load: */
static const constexpr int apWaitPeriod = 12000;

/* Frequency in milliseconds to check access points when waiting for 
 * them to load: */
static const constexpr int apCheckFrequency = 1000;

/* Number of milliseconds to wait for connection events to occur: */
static const constexpr int connectionWaitPeriod = 6500;

/* Frequency in milliseconds to check if an expected connection 
 * event has occurred: */
static const constexpr int connectionCheckFrequency = 100;

/*
 * Waits until multiple visible access points have been registered to the 
 * APList.
 */
bool Wifi::TestUtils::Waiting::waitForAccessPoints()
{
    APList::Reader listReader;
    return DelayUtils::idleUntil([&listReader]()
    {
        return listReader.getAccessPoints().size() > 1;
    }, apCheckFrequency, apWaitPeriod);
}

/*
 * Waits for the wifi device to detect a specific access point.
 */
Wifi::AccessPoint Wifi::TestUtils::Waiting::waitForAccessPoint
(const LibNM::APHash apHash)
{
    APList::Reader listReader;
    AccessPoint requestedAP;
    DelayUtils::idleUntil([&listReader, &apHash, &requestedAP]()
    {
        requestedAP = listReader.getAccessPoint(apHash);
        return !requestedAP.isNull();
    }, apCheckFrequency, apWaitPeriod);
    return requestedAP;
}

/*
 * Waits for the next Wifi connection event to be registered.
 */
Wifi::Connection::Event Wifi::TestUtils::Waiting::waitForNextConnectionEvent
(const juce::Time startTime)
{
    using namespace Wifi::Connection;
    const juce::int64 startTimeMS = startTime.toMilliseconds();
    Record::Reader recordReader;
    Event lastEvent;
    if(DelayUtils::idleUntil([&lastEvent, &recordReader, startTimeMS]()
    {
        lastEvent = recordReader.getLatestEvent();
        return lastEvent.getEventTime().toMilliseconds() > startTimeMS;
    }, connectionCheckFrequency, connectionWaitPeriod))
    {
        return lastEvent;
    }
    return Event();
}
