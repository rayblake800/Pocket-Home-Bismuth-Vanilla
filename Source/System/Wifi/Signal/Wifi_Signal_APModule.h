#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Signal_APModule.h
 *
 * @brief  Sends Wifi access point signal strength changes to the access point
 *         list.
 */

#include "Wifi_LibNM_Signal_APHandler.h"
#include "Wifi_Module.h"

namespace Wifi 
{ 
    class Resource;
    namespace Signal { class APModule; }
}

class Wifi::Signal::APModule : public LibNM::Signal::APHandler,
    public Wifi::Module
{
public:
    /**
     * @brief  Connects the module to its Resource.
     *
     * @param parentResource  The Wifi::Resource object instance.
     */
    APModule(Resource& parentResource);

    virtual ~APModule() { }

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
    virtual void signalStrengthChanged(LibNM::AccessPoint updatedAP,
            unsigned int newStrength) override;
};
