#pragma once
#include "Wifi/Listeners/UpdateInterfaces/APUpdateInterface.h"
#include "SharedResource/Handler.h"

namespace Wifi { class APList; }
namespace Wifi { class VisibleAPListener; }

class Wifi::VisibleAPListener : public APUpdateInterface,
    public SharedResource::Handler<APList>
{
public:
    VisibleAPListener() { }

    virtual ~VisibleAPListener() { }

    /**
     * @brief  Gets the list of all visible access points from the APList.
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
