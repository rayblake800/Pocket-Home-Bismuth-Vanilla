#pragma once
/**
 * @file  Wifi_APList_Listener.h
 *
 * @brief  Receives notifications whenever the list of visible Wifi access
 *         points changes.
 */

#ifndef WIFI_IMPLEMENTATION
  #define APLIST_LISTENER_IMPLEMENTATION
  #define WIFI_IMPLEMENTATION
#endif
#include "Wifi_APList_UpdateInterface.h"
#ifdef APLIST_LISTENER_IMPLEMENTATION
  #undef APLIST_LISTENER_IMPLEMENTATION
  #undef WIFI_IMPLEMENTATION
#endif
#include "SharedResource_Modular_Handler.h"

namespace Wifi { namespace APList { class Listener; } }
namespace Wifi { namespace APList { class Module; } }
namespace Wifi { class Resource; }

class Wifi::APList::Listener : public UpdateInterface,
    public SharedResource::Modular::Handler<Resource, Module>
{
public:
    Listener();

    virtual ~Listener() { }

    /**
     * @brief  Gets the list of all visible access points from the access point 
     *         list resource.
     *
     * @return  All visible access points.
     */
    juce::Array<AccessPoint> getVisibleAPs() const;

private:
    /**
     * @brief  Signals that a new access point was discovered.
     *
     * This takes no action by default, override to handle new AP events.
     *
     * @param newAP  The new Wifi::AccessPoint object.
     */
    virtual void accessPointAdded(const AccessPoint newAP) override;

    /**
     * @brief  Signals that a tracked access point was lost.
     *
     * This takes no action by default, override to handle removed AP events.
     *
     * @param removedAP  The Wifi::AccessPoint that is no longer visible.
     */
    virtual void accessPointRemoved(const AccessPoint removedAP) override;
};
