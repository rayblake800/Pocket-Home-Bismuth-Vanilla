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

#include "SharedResource/Handler.h"
#include "Wifi/AccessPointList/APListWriter.h"
#include "Wifi/Connection/RecordWriter.h"
#include "Wifi/NMSignalHandlers/APSignalHandler.h"
#include "LibNM/NMObjects/DeviceWifi.h"

namespace Wifi { class DeviceSignalHandler; }
namespace Wifi { class APList; }
namespace Wifi { namespace Connection { class RecordWriter; } }

class Wifi::DeviceSignalHandler : public LibNM::DeviceWifi::Listener,
    public SharedResource::Handler<APList>
{
public:
    /**
     * @brief  Sets up Wifi access point signal strength tracking.
     */
    DeviceSignalHandler();

    virtual ~DeviceSignalHandler() { }

private:
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
     *
     * @param removedAP  The removed access point's LibNM object.
     */
    virtual void accessPointRemoved
    (LibNM::AccessPoint removedAP) final override;

   /**
    * @brief  Updates the connection record when the active network connection
    *         changes.
    * 
    * @param activeConnection   The new active Wifi connection. If the device 
    *                           has disconnected, this will be a null object.
    */
    virtual void activeConnectionChanged
    (LibNM::ActiveConnection activeConnection) final override;

    /* Used to update the access point list. */
    APListWriter apListWriter;

    /* Used to update the connection state. */
    Connection::RecordWriter connectionRecorder;

    /* Tracks all access point signal strengths. */
    APSignalHandler apSignalHandler;
};
