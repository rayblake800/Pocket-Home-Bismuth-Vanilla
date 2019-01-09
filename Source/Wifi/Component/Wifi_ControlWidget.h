#pragma once
/**
 * @file  Wifi_ControlWidget.h
 * 
 * @brief  Shows wifi state, lets the user enable or disable wifi, and opens the
 *         wifi settings page.
 */

#include "Settings/QuickSettingsPage/ConnectionSettingsComponent.h"
#include "Wifi_Device_Listener.h"
#include "Wifi_Connection_Listener.h"
#include "Locale/TextUser.h"

namespace Wifi { class ControlWidget; }
namespace Wifi { class AccessPoint; }

class Wifi::ControlWidget : public ConnectionSettingsComponent,
    public Wifi::Device::Listener, public Wifi::Connection::Listener,  
    public Locale::TextUser
{
public:
    /**
     * @param openWifiPage   A callback function that opens the wifi page.
     */
    ControlWidget(std::function<void() > openWifiPage);

    virtual ~ControlWidget() { }

private:
    /**
     * @brief  Checks if wifi is currently turned on.
     * 
     * @return  True if wifi is enabled, false if disabled.
     */
    bool connectionEnabled() override;
       
    /**
     * @brief  Determines if the loading spinner should be shown.
     * 
     * @return  True whenever wifi is in a transition state (connecting,
     *          disconnecting, etc.), and the spinner should be visible.
     */
    bool shouldShowSpinner() override;

    /**
     * @brief  Determines if the connection switch should be enabled.
     * 
     * @return  True if and only if the wifi device is present and isn't being 
     *          turned on or off.
     */
    bool allowConnectionToggle() override;
    
    /**
     * @brief  Determines if the connection page should be accessible.
     * 
     * @return  True whenever wifi is enabled and not being disabled.
     */
    bool connectionPageAvailable() override;

    /**
     * @brief  Gets the wifi icon path.
     * 
     * @return  The wifi icon path.
     */
    juce::String getIconAsset() override;

    /**
     * Enable or disable the wifi radio.
     * 
     * @param enabled   This sets which action to take.
     */
    void enabledStateChanged(bool enabled) override;

    /**
     * Sets the wifi button text based on the current wifi state.
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
     * @brief  Shows the loading spinner and updates connection text when a
     *         new connection starts.
     */
    virtual void startedConnecting(const AccessPoint connectingAP)
        final override;

    /**
     * @brief  Signals that a new Wifi connection was opened successfully.
     *
     * @param connectedAP  The access point used to open the connection.
     */
    virtual void connected(const AccessPoint connectedAP) override; 

    /**
     * @brief  Updates the widget when a connection disconnects, stopping the 
     *         loading spinner and updating the connection text.
     *
     * @param disconnectedAP  The access point that was being used by the closed
     *                        connection.
     */
    virtual void disconnected(const AccessPoint disconnectedAP) override; 

    /**
     * @brief  Updates the widget when a connection fails, stopping the loading
     *         spinner and updating the connection text.
     *
     * @param connectingAP  The access point the system was attempting to use
     *                      to open the new connection.
     */
    virtual void connectionAuthFailed(const AccessPoint connectingAP) override;


    //if true, show the loading spinner and disable controls.
    bool wifiBusy = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlWidget)
};
