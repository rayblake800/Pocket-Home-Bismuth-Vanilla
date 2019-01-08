#pragma once
/**
 * @file  Wifi/Manager.h
 *
 * @brief  Ensures Wifi resources and signal handlers remain initialized as
 *         long as an Initializer object exists.
 */

#include "WindowFocus/WindowFocus.h"
#include "LibNM/ThreadHandler.h"
#include "Wifi_APList_Reader.h"
#include "Wifi/Connection/RecordReader.h"

#ifndef WIFI_IMPLEMENTATION
  #define WIFI_MANAGER_TEMP
  #define WIFI_IMPLEMENTATION
#endif

#include "Wifi/Device/DeviceListener.h"
#include "Wifi/NMSignalHandlers/DeviceSignalHandler.h"
#include "Wifi/NMSignalHandlers/ClientSignalHandler.h"

#ifdef WIFI_MANAGER_TEMP
  #undef WIFI_MANAGER_TEMP
  #undef WIFI_IMPLEMENTATION
#endif

namespace Wifi { class Manager; }

class Wifi::Manager : public WindowFocus::Listener
{
public:
    /**
     * @brief  Initializes the LibNM thread resource, then creates and connects 
     *         all Wifi signal handlers.
     */
    Manager();

    /**
     * @brief  Destroys all Wifi signal handlers before the LibNM thread
     *         resource can be destroyed.
     */
    virtual ~Manager();

private:
    /**
     * @brief  Disables signal handlers while the application does not have
     *         window focus.
     */
    virtual void windowFocusLost() override;


    /**
     * @brief  Enables signal handlers when the application regains window
     *         focus.
     */
    virtual void windowFocusGained() override;

    /* Ensures the LibNM thread resource is initialized. */
    LibNM::ThreadHandler nmThread;

    /* Ensures the access point list resource is initialized. */
    std::unique_ptr<APList::Reader> apListReader;

    /* Ensures the connection record resource is initialized. */
    std::unique_ptr<Connection::RecordReader> recordReader;

    /* Handles all signals from the LibNM thread resource's Client object. */
    std::unique_ptr<ClientSignalHandler> clientSignalHandler;

    /* Handles all signals from the LibNM thread resource's DeviceWifi object.*/
    std::unique_ptr<DeviceSignalHandler> deviceSignalHandler;
};
