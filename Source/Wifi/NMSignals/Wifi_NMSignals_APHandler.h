#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_NMSignals_APHandler.h
 *
 * @brief  Sends Wifi access point signal strength changes to the access point
 *         list.
 */

#include "SharedResource_Handler.h"
#include "LibNM/BorrowedObjects/AccessPoint.h"

namespace Wifi { namespace NMSignals { class APHandler; } }
namespace Wifi { namespace APList { class ListResource; } }

class Wifi::NMSignals::APHandler : public LibNM::AccessPoint::Listener,
    public SharedResource::Handler<APList::ListResource>
{
public:
    APHandler();

    virtual ~APHandler() { }

private:
    /**
     * @brief  Passes signal strength updates to the access point list.
     *
     * @param updatedAP    The LibNM::AccessPoint that had its signal strength
     *                     change.
     *
     * @param newStrength  The new strength of the access point, as a value
     *                     between zero and one hundred, inclusive.
     */
    virtual void signalStrengthChanged(LibNM::AccessPoint& updatedAP,
            unsigned int newStrength) final override;
};
