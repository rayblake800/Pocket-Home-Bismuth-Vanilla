#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file Wifi_Resource.h
 *
 * @brief  Manages shared Wifi data and controls.
 */

#include "SharedResource_Modular_Resource.h"

namespace Wifi { class Resource; }

class Wifi::Resource : public SharedResource::Modular::Resource<>
{
public:
    /* The SharedResource::Resource object instance key: */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Initializes all Wifi resource modules and starts the Wifi thread.
     */
    Resource();

    /**
     * @brief  Shuts down all Wifi resource modules and stops the Wifi thread.
     */
    virtual ~Resource();
};
