#ifndef WIFI_IMPLEMENTATION
  #error File included outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi/NMSignalHandlers/APSignalHandler.h
 *
 * @brief  Sends Wifi access point signal strength changes to the access point
 *         list.
 */

#include "SharedResource/Handler.h"
#include "LibNM/NMObjects/AccessPoint.h"

namespace Wifi { class APSignalHandler; }
namespace Wifi { class APList; }

class Wifi::APSignalHandler : public LibNM::AccessPoint::Listener,
    public SharedResource::Handler<APList>
{
public:
    APSignalHandler();

    virtual ~APSignalHandler() { }

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
