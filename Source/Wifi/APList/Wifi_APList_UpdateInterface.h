#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file Wifi_APList_UpdateInterface.h
 *
 * @brief  The interface used by the Wifi::APList::Module to update all
 *         Wifi::APList::Listener objects when access points are added or 
 *         removed.
 */

namespace Wifi { namespace APList { class UpdateInterface; } }
namespace Wifi { class AccessPoint; }

class Wifi::APList::UpdateInterface
{
public:
    UpdateInterface() { }

    virtual ~UpdateInterface() { }

    /**
     * @brief  Signals that a new access point was discovered.
     *
     * @param newAP  The new Wifi::AccessPoint object.
     */
    virtual void accessPointAdded(const AccessPoint newAP) = 0;

    /**
     * @brief  Signals that a tracked access point was lost.
     *
     * @param removedAP  The Wifi::AccessPoint that is no longer visible.
     */
    virtual void accessPointRemoved(const AccessPoint removedAP) = 0;
};
