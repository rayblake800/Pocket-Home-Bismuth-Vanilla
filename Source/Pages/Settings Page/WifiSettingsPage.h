/**
 * @file WifiSettingsPage.h
 * 
 * WifiSettingsPage shows information on all visible Wifi access points, and
 * provides controls for connecting or disconnecting from those access points.
 */

#pragma once
#include "../../Wifi/WifiStatus.h"
#include "../../Basic Components/ScalingLabel.h"
#include "../../Basic Components/SwitchComponent.h"
#include "../../Basic Components/DrawableImageComponent.h"
#include "../../Basic Components/DrawableImageButton.h"
#include "../../PageComponent.h"
#include "../../PageStackComponent.h"
#include "ConnectionPage.h"

class WifiSettingsPage : public ConnectionPage<WifiAccessPoint>,
public WifiStatus::Listener, public TextEditor::Listener {
public:
    WifiSettingsPage();
    ~WifiSettingsPage();
private:
    /**
     * @return the list of all visible Wifi access points.
     */
    virtual Array<WifiAccessPoint> loadConnectionList();


    /**
     * Attempts to connect to a Wifi access point.  This will close any
     * connections to other access points.
     *
     *  @param connection
     */
    virtual void connect(const WifiAccessPoint& connection);

    /**
     * @param connection if the system is currently connected to this
     * connection, this method closes that connection.
     *  
     * @param connection
     */
    virtual void disconnect(const WifiAccessPoint& connection);

    /**
     * @param connection
     * @return true iff the system is connected to WifiAccessPoint connection.
     */
    virtual bool isConnected(const WifiAccessPoint& connection);

    /**
     * @param button
     * This is called whenever a button other than the navigation buttons
     * is clicked.
     */
    virtual void connectionButtonClicked(Button* button);

    /**
     * Construct a button component to represent a wifi access point.
     * This button will display the access point name, along with a signal
     * strength indicator and a lock icon if the connection requires a password.
     * 
     * @param connection
     * TODO: come up with some sort of indicator to mark the connected access
     * point.
     */
    virtual Button* getConnectionButton(const WifiAccessPoint& connection);

    /**
     * Get the layout for the Wifi access point controls.
     * @param connection the control components will be updated to suit
     * this access point.
     */
    virtual GridLayoutManager::Layout getConnectionControlsLayout
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
     * When currentlyConnecting, disable Wifi controls and show a loading
     * spinner.  Otherwise, enable controls and hide the loading spinner.
     * 
     * @param currentlyConnecting indicates if Wifi is trying to connect to
     * an access point, or is otherwise busy.
     */
    void setCurrentlyConnecting(bool currentlyConnecting);

    /**
     * When wifi is enabled, reload page contents.
     */
    void handleWifiEnabled() override;

    /**
     * When wifi is disabled, close this page.
     */
    void handleWifiDisabled() override;

    /**
     * When Wifi connects, reload page contents.
     */
    void handleWifiConnected() override;

    /**
     * When Wifi disconnects, reload page contents.
     */
    void handleWifiDisconnected() override;

    /**
     * Disable connection controls when Wifi is busy.
     */
    void handleWifiBusy() override;

    /**
     * When connecting fails, show the error label and enable connection
     * controls.
     */
    void handleWifiFailedConnect() override;

    /**
     * Attempt to connect if return is pressed after entering a password.
     * @param editor
     */
    void textEditorReturnKeyPressed(TextEditor& editor) override;

    /**
     * Set the spinner's bounds within the connection button
     */
    void connectionPageResized() override;

    //Wifi icons for all signal strengths
    static const Array<String> wifiImageFiles;

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
    class WifiAPButton : public Button {
    public:
        WifiAPButton(const WifiAccessPoint& connection, bool isConnected);
    private:
        void resized() override;

        void paintButton(Graphics& g, bool isMouseOverButton,
                bool isButtonDown) {
            //g.fillAll(findColour(backgroundColourId));
        }
        ScalingLabel apLabel;
        DrawableImageComponent wifiIcon;
        ScopedPointer<DrawableImageComponent> lockIcon;

    };

    bool wifiBusy = false;
    ScalingLabel passwordLabel;
    TextEditor passwordEditor;
    TextButton connectionButton;
    ScalingLabel errorLabel;
    Spinner spinner;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsPage)
};
