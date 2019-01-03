#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi/NMSignalHandlers/DeviceSignalHandler.h
 *
 * @brief  Handles signals from the Wifi device object, updating Wifi connection
 *         state, and visible access points. The DeviceSignalHandler is also
 *         responsible for 
 *         Wifi::APList resource, sends changes to the active Wifi connection 
 *         to the Wifi::Connection::RecordResource.
 */

#include "SharedResource_Handler.h"
#include "Wifi/AccessPointList/APListWriter.h"
#include "Wifi/Connection/RecordWriter.h"
#include "Wifi/NMSignalHandlers/APSignalHandler.h"
#include "LibNM/BorrowedObjects/DeviceWifi.h"

namespace Wifi { class DeviceSignalHandler; }
namespace Wifi { class APList; }
namespace Wifi { namespace Connection { class RecordWriter; } }

class Wifi::DeviceSignalHandler : public LibNM::DeviceWifi::Listener,
    public SharedResource::Handler<APList>
{
public:
    DeviceSignalHandler();

    virtual ~DeviceSignalHandler() { }

    /**
     * @brief  Starts tracking the LibNM::ThreadResource's DeviceWifi object.
     *
     * Signals will not be received until the signal handler is connected.
     */
    void connect();

    /**
     * @brief  Stops tracking the LibNM::ThreadResource's DeviceWifi object.
     * 
     * Signals will not be received after the signal handler is disconnected 
     * until it is connected again.
     */
    void disconnect();

private:
   /**
    * @brief  Handles Wifi device state changes.
    *
    * @param newState  The new device state value.
    *
    * @param oldState  The previous device state value.
    *
    * @param reason    The reason for the change in device state.
    */
    virtual void stateChanged(NMDeviceState newState,
            NMDeviceState oldState,
            NMDeviceStateReason reason) final override;

    /**
     * @brief  Updates the access point list whenever a new access point is
     *         detected.
     *
     * @param addedAP  The new access point's LibNM object.
     */
    virtual void accessPointAdded(LibNM::AccessPoint addedAP) final override;

    /**
     * @brief  Updates the access point list whenever a previously seen access
     *         point is lost.
     */
    virtual void accessPointRemoved() final override;

   /**
    * @brief  Updates the connection record when the active network connection
    *         changes.
    * 
    * @param activeConnection   The new active Wifi connection. If the device 
    *                           has disconnected, this will be a null object.
    */
    virtual void activeConnectionChanged
    (LibNM::ActiveConnection activeConnection) final override;

    /* Tracks all access point signal strengths. */
    APSignalHandler apSignalHandler;
};
