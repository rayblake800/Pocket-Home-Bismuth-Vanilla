#pragma once
#include "WifiStateManager.h"
#include "Localized.h"
#include "Spinner.h"
#include "ScalingLabel.h"
#include "SwitchComponent.h"
#include "DrawableImageComponent.h"
#include "DrawableImageButton.h"
#include "PageComponent.h"
#include "PageStackComponent.h"
#include "ConnectionPage.h"

/**
 * @file WifiSettingsPage.h
 * 
 * @brief Shows information on all visible Wifi access points, and provides 
 *        controls for connecting to or disconnecting from those access points.
 */


class WifiSettingsPage : public ConnectionPage<WifiAccessPoint>,
public WifiStateManager::Listener, public TextEditor::Listener,
private Localized
{
public:
    WifiSettingsPage();

    virtual ~WifiSettingsPage() { }

private:
    /**
     * Finds all wifi access points within range of the wifi device.
     * 
     * @return the list of all visible Wifi access points.
     */
    Array<WifiAccessPoint> loadConnectionPoints() override;


    /**
     * Attempts to connect to a Wifi access point.  This will close any
     * connections to other access points.
     *
     *  @param accessPoint  The wifi device will attempt to find and connect
     *                      to this access point.
     */
    void connect(WifiAccessPoint accessPoint);

    /**
     * Tries to disconnect from a specific wifi access point.
     * 
     * @param accessPoint   If the system is currently connected to this
     *                      access point, this method closes that connection.
     */
    void disconnect(WifiAccessPoint accessPoint);

    /**
     * Checks if wifi is connected to a specific access point.
     * 
     * @param accessPoint  The access point to check.
     * 
     * @return  true iff an active network connection is using the given access
     *          point.
     */
    bool isConnected(WifiAccessPoint accessPoint) override;

    /**
     * Attempts to connect or disconnect from the current selected access point
     * when the connection button is clicked.
     * 
     * @param button  This should always be the connection button.
     */
    void connectionButtonClicked(Button* button) override;

    /**
     * Constructs a button component to represent a wifi access point.
     * This button will display the access point name, along with a signal
     * strength indicator and a lock icon if the connection requires a password.
     * 
     * @param connection  The access point represented by the new button
     *                    component.
     */
    Button* getConnectionButton(WifiAccessPoint accessPoint) override;

    /**
     * Gets the layout for the Wifi access point controls.
     * 
     * @param accessPoint   The control components will be updated to control
     *                      this access point.
     */
    RelativeLayoutManager::Layout getConnectionControlsLayout
    (WifiAccessPoint accessPoint) override;

    /**
     * Updates connection control components to match the current Wifi 
     * connection state.
     */
    virtual void updateConnectionControls() override;

    /**
     * Keeps the page updated when wifi state changes.
     * 
     * @param state  The new wifi connection state.
     */
    void wifiStateChanged(WifiStateManager::WifiState state) override;


    /**
     * Attempts to connect if return is pressed after entering a password.
     * 
     * @param editor  This should always be 
     */
    void textEditorReturnKeyPressed(TextEditor& editor) override;

    /**
     * Sets the spinner's bounds within the connection button
     */
    void connectionPageResized() override;


    /**
     * Gets the asset name for the icon that best represents accessPoint's 
     * signal strength.
     * 
     * @param accessPoint
     */
    static String getWifiAssetName(WifiAccessPoint accessPoint);

    /**
     * Reloads the access point list, re-selects the selected connection, 
     * updates and enables connection controls.
     */
    void reloadPage();

    /**
     * The custom button type to use for access point list buttons.
     */
    class WifiAPButton : public Button, private Localized
    {
    public:
        /**
         * @param accessPoint   The access point represented by this button.
         * 
         * @param isConnected   Indicates if an active connection exists using
         *                      this access point.
         */
        WifiAPButton(WifiAccessPoint accessPoint, bool isConnected);
    private:

        /**
         * Updates icon and label bounds to fit button bounds.
         */
        void resized() override;

        /**
         * This method must be implemented in all Button classes, but child
         * components and the LookAndFeel handle all WifiAPButton drawing.
         */
        void paintButton(
                Graphics& g,
                bool isMouseOverButton,
                bool isButtonDown) { }
        
        //shows the access point name
        ScalingLabel apLabel;
        //shows the access point signal strength
        DrawableImageComponent wifiIcon;
        //shown iff the access point is secured
        ScopedPointer<DrawableImageComponent> lockIcon;
    };

    //Wifi icons for all signal strengths
    static const StringArray wifiImageFiles;
    
    WifiAccessPoint lastConnecting;
    WifiAccessPoint lastDisconnecting;

    //Used for entering a password for a secured access point.
    ScalingLabel passwordLabel;
    TextEditor passwordEditor;
    //clicked to connect or disconnect
    TextButton connectionButton;
    //prints an error if the connection fails
    ScalingLabel errorLabel;
    //displays over the connection button while connecting to indicate that
    //wifi is busy
    Spinner spinner;
    
    
    //localized text keys;
    static const constexpr char * password_field = "password_field";
    static const constexpr char * btn_connect = "btn_connect";
    static const constexpr char * btn_disconnect = "btn_disconnect";
    static const constexpr char * wrong_password = "wrong_password";
    static const constexpr char * invalid_key_format = "invalid_key_format";
    static const constexpr char * connection_failed = "connection_failed";
    static const constexpr char * lost_ap = "lost_ap";
    
    static const constexpr char * connected_ap = "connected_ap";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsPage)
};
