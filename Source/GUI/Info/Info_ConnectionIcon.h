#pragma once
/**
 * @file  Info_ConnectionIcon.h
 * 
 * @brief  Displays an icon indicating Wifi connection signal strength.
 */

#include "Info_SignalIcon.h"
#include "Wifi_Device_Listener.h"
#include "Wifi_AP_StrengthListener.h"
#include "Wifi_Connection_Record_Listener.h"

namespace Info { class ConnectionIcon; }


class Info::ConnectionIcon : 
    public SignalIcon,
    public Wifi::AP::StrengthListener, 
    public Wifi::Connection::Record::Listener,
    public Wifi::Device::Listener
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
    virtual void signalStrengthUpdate(const Wifi::AccessPoint updatedAP) 
        final override;

    /**
     * @brief  Changes the tracked AccessPoint and selected status icon when a
     *         new Wifi connection is activated.
     *
     * @param connectedAP  The new connection's access point object.
     */
    virtual void connected(const Wifi::AccessPoint connectedAP) final override; 

    /**
     * @brief  Stops tracking signal strength changes and updates the status
     *         icon when the Wifi connection is lost.
     *
     * @param disconnectedAP  The disconnected access point object.
     */
    virtual void disconnected(const Wifi::AccessPoint disconnectedAP) 
        final override;

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
