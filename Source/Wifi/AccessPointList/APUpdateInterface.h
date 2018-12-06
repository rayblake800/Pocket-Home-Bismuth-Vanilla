#pragma once
/**
 * @file Wifi/Listeners/UpdateInterfaces/APUpdateInterface.h
 *
 * @brief  The interface used by the Wifi::APList to update all
 *         Wifi::VisibleAPListener objects when access points are added or 
 *         removed.
 */

namespace Wifi { class APUpdateInterface; }
namespace Wifi { class AccessPoint; }

class Wifi::APUpdateInterface
{
public:
    APUpdateInterface() { }

    virtual ~APUpdateInterface() { }

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
