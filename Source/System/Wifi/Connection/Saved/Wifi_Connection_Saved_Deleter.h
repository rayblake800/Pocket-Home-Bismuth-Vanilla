#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Connection_Saved_Deleter.h
 *
 * @brief  Deletes saved network connections from the system.
 */
#include "SharedResource_Modular_Handler.h"

namespace Wifi { namespace Connection {  namespace Saved { class Deleter; } } }
namespace Wifi { namespace Connection {  namespace Saved { class Module; } } }
namespace Wifi { class Resource; }
namespace Wifi { class AccessPoint; }


class Wifi::Connection::Saved::Deleter : 
        public SharedResource::Modular::Handler<Resource, Module>
{
public:
    Deleter();

    virtual ~Deleter() { }

    /**
     * @brief  Removes all saved network connections that match a particular
     *         access point.
     *
     * @param toRemove  The access point used to select saved connections to
     *                  delete.
     */
    void removeSavedConnection(const AccessPoint toRemove) const;
};
