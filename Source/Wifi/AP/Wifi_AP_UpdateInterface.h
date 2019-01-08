#pragma once
/**
 * @file  Wifi/Listeners/UpdateInterfaces/SignalUpdateInterface.h
 *
 * @brief  The interface used by Wifi::APList to send updates to all
 *         Wifi::SignalStrengthListener objects.
 */

namespace Wifi { class SignalUpdateInterface; }
namespace Wifi { class AccessPoint; }

class Wifi::SignalUpdateInterface
{
public:
    SignalUpdateInterface() { }

    virtual ~SignalUpdateInterface() { }

    /**
     * @brief  Called whenever a Wifi::AccessPoint object's signal strength
     *         changes.
     *
     * @param updatedAP  The access point with a new signal strength value.
     */
    virtual void signalStrengthChanged(const Wifi::AccessPoint updatedAP) = 0;
};
