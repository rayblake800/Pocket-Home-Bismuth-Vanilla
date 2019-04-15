/**
 * @file  Wifi_Connection_Saved_Deleter.h
 *
 * @brief  Deletes saved network connections from the system.
 */
#include "Wifi_Module.h"

namespace Wifi
{
    class Resource;
    class AccessPoint;
    namespace Connection
    {
        namespace Saved
        {
            class Deleter;
            class Module;
        }
    }
}

/**
 * @brief  Connects to the Connection::Saved::Module to delete saved
 *         connections.
 */
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
