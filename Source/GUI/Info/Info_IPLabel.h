#pragma once
/**
 * @file  Info_IPLabel.h
 *
 * @brief  Shows the system's local and/or public IP addresses to the user.
 */

#include "Widgets_BoundedLabel.h"
#include "Locale_TextUser.h"
#include "Util_Commands.h"
#ifdef WIFI_SUPPORT
#include "Wifi_Connection_Record_Listener.h"
#endif

namespace Info { class IPLabel; }

/**
 * @brief  A configurable label component that prints the system's local and/or
 *         public IP addresses.
 *
 *  IPLabel is used to show the user their system's current IP address. The
 * label may show the system's IP address on the local network, the system's 
 * public IP address as seen outside the network, or both. The IPLabel will 
 * not print any text when the system is not connected to a network, or when
 * both local and public IP addresses are disabled. IPLabel's properties are
 * set in the config.json file, accessed through Config::MainFile.
 */
class Info::IPLabel : public Locale::TextUser,
#ifdef WIFI_SUPPORT
    public Wifi::Connection::Record::Listener,
#endif
    public Widgets::BoundedLabel
{
public:
    /**
     * @brief  Loads appropriate label text on construction.
     */
    IPLabel() noexcept;

    virtual ~IPLabel() { }

    /**
     * @brief  Updates the IP label text to match the IPLabel settings in the
     *         main configuration file and the current system IP address(es).
     */
    void updateLabelText() noexcept;

private:
    /**
     * @brief  Updates the label text when the IPLabel gains visibility.
     */
    virtual void visibilityChanged() override;

#ifdef WIFI_SUPPORT
    /**
     * @brief  Updates the label text whenever a new network connection becomes
     *         active.
     *
     * @param connectedAP  The access point used to create the new connection.
     */
    virtual void connected(const AccessPoint connectedAP) override;

    /**
     * @brief  Updates the label text whenever the active network connection is
     *         closed.
     *
     * @param connectedAP  The access point used by the former active 
     *                     connection.
     */
    virtual void disconnected(const AccessPoint connectedAP) override;
#endif

    /* Runs system commands and ensures the system command resource remains
     * loaded. */
    Util::Commands commandLoader;
};
