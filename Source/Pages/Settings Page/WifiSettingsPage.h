/**
 * @file WifiSettingsPage.h
 * 
 * WifiSettingsPage shows information on all visible Wifi access points, and
 * provides controls for connecting or disconnecting from those access points.
 */

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
    Array<WifiAccessPoint> loadConnectionList() override;

    /**
     * Checks if wifi is connected to a specific access point.
     * 
     * @param connection
     * 
     * @return true iff the system is connected to WifiAccessPoint connection.
     */
    bool isConnected(const WifiAccessPoint& connection) override;

    /**
     * Attempt to connect or disconnect from the current selected access point
     * when the connection button is clicked.
     * 
     * @param button
     */
    void connectionButtonClicked(Button* button);

    /**
     * Construct a button component to represent a wifi access point.
     * This button will display the access point name, along with a signal
     * strength indicator and a lock icon if the connection requires a password.
     * 
     * @param connection
     */
    Button* getConnectionButton(const WifiAccessPoint& connection);

    /**
     * Get the layout for the Wifi access point controls.
     * 
     * @param connection   The control components will be updated to suit
     *                      this access point.
     */
    RelativeLayoutManager::Layout getConnectionControlsLayout
    (const WifiAccessPoint& connection);
    
    /**
     * Update connection control components to match the current Wifi connection
     * state and the provided Wifi access point.
     * 
     * @param accessPoint
     */
    virtual void updateConnectionControls
    (const WifiAccessPoint& accessPoint) override;

    /**
     * Keeps the page updated when wifi state changes.
     * 
     * @param state
     */
    void wifiStateChanged(WifiStateManager::WifiState state) override;


    /**
     * Attempt to connect if return is pressed after entering a password.
     * 
     * @param editor
     */
    void textEditorReturnKeyPressed(TextEditor& editor) override;

    /**
     * Set the spinner's bounds within the connection button
     */
    void connectionPageResized() override;


    /**
     * Get the asset name for the icon that best represents accessPoint's 
     * signal strength.
     * 
     * @param accessPoint
     */
    static String getWifiAssetName(const WifiAccessPoint& accessPoint);

    /**
     * Reload the access point list, re-select the selected connection, 
     * update and enable connection controls.
     */
    void reloadPage();

    /**
     * Custom button type to use for getConnectionButton
     */
    class WifiAPButton : public Button, private Localized
    {
    public:
        /**
         * @param connection    The access point represented by this button.
         * 
         * @param isConnected   Should be true if wifi is connected to the 
         *                       connection access point parameter.
         */
        WifiAPButton(const WifiAccessPoint& connection, bool isConnected);
    private:

        /**
         * Update icon and label bounds to fit button bounds.
         */
        void resized() override;

        /**
         * This has to be implemented in all Button classes, but child
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
    //holds the last known wifi state
    WifiStateManager::WifiState lastState;
    
    
    
    //localized text keys;
    static const constexpr char * password_field = "password_field";
    static const constexpr char * btn_connect = "btn_connect";
    static const constexpr char * btn_disconnect = "btn_disconnect";
    static const constexpr char * wrong_password = "wrong_password";
    static const constexpr char * connection_failed = "connection_failed";
    static const constexpr char * connected_ap = "connected_ap";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsPage)
};
