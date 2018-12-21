#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi/Connection/RecordWriter.h
 *
 * @brief  Updates the RecordResource's Wifi event records.
 */
#include "SharedResource/Handler.h"

namespace Wifi { namespace Connection { class RecordWriter; } }
namespace Wifi { namespace Connection { class RecordResource; } }
namespace Wifi { namespace Connection { class Event; }  }
namespace Wifi { class AccessPoint; }

namespace WifiConnect = Wifi::Connection;

class WifiConnect::RecordWriter : public SharedResource::Handler<RecordResource>
{
public:
    RecordWriter();

    virtual ~RecordWriter();

    /**
     * @brief  Connects to NetworkManager to rebuild the list of connection 
     *         records.
     */
    void updateRecords() const;

    /**
     * @brief  Adds a new event to the list of saved events.
     *
     * @param newEvent  A valid connection event to add to the list. If this
     *                  parameter is null, no action will be taken.
     */
    void addConnectionEvent(const Event newEvent) const;

    /**
     * @brief  Removes all saved network connections that match a particular
     *         access point.
     *
     * @param toRemove  The access point used to select saved connections to
     *                  delete.
     */
    void removeSavedConnection(const AccessPoint toRemove) const;
};
