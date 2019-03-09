#pragma once
/**
 * @file  Wifi_LibNM_Thread_Handler.h
 *
 * @brief  Provides access to the LibNM thread module.
 */

#include "Wifi_LibNM_Client.h"
#include "Wifi_LibNM_DeviceWifi.h" 
#include "Wifi_LibNM_ActiveConnection.h"
#include "GLib_ThreadHandler.h"
#include "SharedResource_Modular_Handler.h"
#include "JuceHeader.h"

namespace Wifi 
{ 
    class Resource;
    namespace LibNM { namespace Thread 
    {
        class Handler; 
        class Module;
    } } 
}

class Wifi::LibNM::Thread::Handler : public GLib::ThreadHandler
        <SharedResource::Modular::Handler<Resource, Module>>
{
public:
    /**
     * @brief  Creates the shared Wifi::Resource if it doesn't already exist.
     */
    Handler();

    virtual ~Handler() { }

    /*
     * @brief  Gets the shared NetworkManager client object.
     *
     * @return  The Client object, or a null Client if called outside the event
     *          loop.
     */
    Client getClient() const;

    /**
     * @brief  Gets the shared DeviceWifi object used to control the 
     *         LibNM-managed Wifi device if called within the LibNM event loop.
     *
     * @return  The DeviceWifi object used to control the LibNM-managed Wifi
     *          device, or a null DeviceWifi object if called outside the event
     *          loop.
     */
    DeviceWifi getWifiDevice() const;

    /**
     * @brief  Schedules a function to run asynchronously within the LibNM 
     *         thread module's event loop.
     *
     * @param toCall  A function to call within the thread's event loop.
     */
    void callAsync(std::function<void()> toCall) const;

    /**
     * @brief  Calls a function within the LibNM thread module's event loop,
     *         waiting until the function finishes executing.
     *
     * @param toCall  A function to call within the thread's event loop.
     */
    void call(std::function<void()> toCall) const;
};
