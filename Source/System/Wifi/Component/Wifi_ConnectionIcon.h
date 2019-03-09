#pragma once
/**
 * @file  Wifi_ConnectionIcon.h
 * 
 * @brief  Displays an icon indicating Wifi connection signal strength.
 */

#include "Wifi_SignalIcon.h"
#include "Wifi_Device_Listener.h"
#include "Wifi_AP_StrengthListener.h"
#include "Wifi_Connection_Record_Listener.h"

namespace Wifi { class ConnectionIcon; }


class Wifi::ConnectionIcon : 
    public SignalIcon,
    public AP::StrengthListener, 
    public Connection::Record::Listener,
    public Device::Listener
{
public:
    ConnectionIcon();

    virtual ~ConnectionIcon() { }

private:
    /**
     * @brief  Updates the selected icon when the active connection's signal
     *         strength changes.
     *
     * @param updatedAP  The active connection's AccessPoint.
     */
    virtual void signalStrengthUpdate(const AccessPoint updatedAP) 
        final override;

    /**
     * @brief  Changes the tracked AccessPoint and selected status icon when a
     *         new Wifi connection is activated.
     *
     * @param connectedAP  The new connection's access point object.
     */
    virtual void connected(const AccessPoint connectedAP) final override; 

    /**
     * @brief  Stops tracking signal strength changes and updates the status
     *         icon when the Wifi connection is lost.
     *
     * @param disconnectedAP  The disconnected access point object.
     */
    virtual void disconnected(const AccessPoint disconnectedAP) final override;

    /**
     * @brief  Updates the selected icon when Wifi is turned on.
     */
    virtual void wirelessEnabled() final override;

    /**
     * @brief  Updates the selected icon and stops tracking signal strength when
     *         Wifi is turned off.
     */
    virtual void wirelessDisabled() final override;
};
