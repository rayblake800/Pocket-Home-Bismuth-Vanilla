
#pragma once
/**
 * @file  Settings_WifiControl.h
 *
 * @brief  Shows Wifi state, lets the user enable or disable Wifi, and opens
 *         the Wifi settings page.
 */

#include "Settings_ConnectionComponent.h"
#include "Wifi_Device_Listener.h"
#include "Wifi_Connection_Record_Listener.h"
#include "Locale_TextUser.h"

namespace Settings { class WifiControl; }
namespace Wifi { class AccessPoint; }

/**
 * @brief  A UI Component that displays the current Wifi connection and provides
 *         compact Wifi controls.
 */
class Settings::WifiControl : public ConnectionComponent,
    public Wifi::Device::Listener, public Wifi::Connection::Record::Listener,
    public Locale::TextUser
{
public:
    /**
     * @brief  Sets the function used to open the Wifi settings page on
     *         construction.
     *
     * @param openWifiPage   A callback function that opens the Wifi page.
     */
    WifiControl(std::function<void()> openWifiPage);

    virtual ~WifiControl() { }

private:
    /**
     * @brief  Checks if Wifi is currently turned on.
     *
     * @return  True if Wifi is enabled, false if disabled.
     */
    bool connectionEnabled() override;

    /**
     * @brief  Determines if the loading spinner should be shown.
     *
     * @return  True whenever Wifi is in a transition state(connecting,
     *          disconnecting, etc.), and the spinner should be visible.
     */
    bool shouldShowSpinner() override;

    /**
     * @brief  Determines if the connection switch should be enabled.
     *
     * @return  True if and only if the Wifi device is present and isn't being
     *          turned on or off.
     */
    bool allowConnectionToggle() override;

    /**
     * @brief  Determines if the connection page should be accessible.
     *
     * @return  True whenever Wifi is enabled and not being disabled.
     */
    bool connectionPageAvailable() override;

    /**
     * @brief  Gets the Wifi icon path.
     *
     * @return  The Wifi icon path.
     */
    juce::String getIconAsset() override;

    /**
     * @brief  Enables or disables the Wifi radio.
     *
     * @param enabled   Whether Wifi should be enabled or disabled.
     */
    void enabledStateChanged(bool enabled) override;

    /**
     * @brief  Sets the Wifi button text based on the current Wifi state.
     *
     * @return  The text that should be printed on the Wifi button.
     */
    juce::String updateButtonText() override;

    /**
     * @brief  Updates the widget whenever wireless networking is enabled.
     */
    virtual void wirelessEnabled() final override;

    /**
     * @brief  Updates the widget whenever wireless networking is disabled.
     */
    virtual void wirelessDisabled() final override;

    /**
     * @brief  Shows the loading spinner and updates connection text when a new
     *         connection starts.
     *
     * @param connectingAP  The access point used by the activating connection.
     */
    virtual void startedConnecting(const Wifi::AccessPoint connectingAP)
        final override;

    /**
     * @brief  Signals that a new Wifi connection was opened successfully.
     *
     * @param connectedAP  The access point used to open the connection.
     */
    virtual void connected(const Wifi::AccessPoint connectedAP) override;

    /**
     * @brief  Updates the widget when a connection disconnects, stopping the
     *         loading spinner and updating the connection text.
     *
     * @param disconnectedAP  The access point that was being used by the
     *                        closed connection.
     */
    virtual void disconnected(const Wifi::AccessPoint disconnectedAP) override;

    /**
     * @brief  Updates the widget when a connection fails, stopping the loading
     *         spinner and updating the connection text.
     *
     * @param connectingAP  The access point the system was attempting to use
     *                      to open the new connection.
     */
    virtual void connectionAuthFailed(const Wifi::AccessPoint connectingAP)
        override;

    // If true, show the loading spinner and disable controls.
    bool wifiBusy = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiControl)
};
