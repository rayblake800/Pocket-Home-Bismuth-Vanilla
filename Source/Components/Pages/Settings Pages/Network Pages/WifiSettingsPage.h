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
#include "FocusingListPage.h"
#include "WindowFocusedTimer.h"

/**
 * @file WifiSettingsPage.h
 * 
 * @brief Shows information on all visible Wifi access points, and provides 
 *        controls for connecting to or disconnecting from those access points.
 */


class WifiSettingsPage : public FocusingListPage,
        public WifiStateManager::Listener, public juce::TextEditor::Listener,
        private Localized, private WindowFocusedTimer
{
public:
    WifiSettingsPage();

    virtual ~WifiSettingsPage() { }
    
protected:
    /**
     * Sets the number of items in the list to match the number of visible
     * Wifi access points.
     * 
     * @return  The number of WifiAccessPoint objects available. 
     */
    virtual unsigned int getListSize() override;
    
    /**
     * Creates or updates the layout of one access point on the list.
     * 
     * @param layout  A layout object to update.  Components will be added or
     *                updated in this layout to match a particular Wifi access
     *                point.
     * 
     * @param index   An index in the list.  The layout will be updated to match
     *                the WifiAccessPoint with this index in the the sorted
     *                list of access points.
     */
    virtual void updateListItemLayout(LayoutManager::Layout& layout,
            const unsigned int index) override;
    
    /**
     * Creates or updates the access point control/information panel that
     * appears when an access point in the list is selected.
     * 
     * @param layout   The control layout object, to be updated to match the 
     *                 current selected access point.
     */
    virtual void updateSelectedItemLayout(LayoutManager::Layout& layout) 
    override;

private:
    /**
     * @brief  Periodically triggers a new scan for Wifi AP updates.
     */
    void timerCallback() override;

    /**
     * Reloads the list of wifi access points within range of the wifi device,
     * and updates the access point list.
     */
    void loadAccessPoints();
    
    /**
     * Removes any lost access points, sorts the access point list, and
     * refreshes the access point list component.
     */
    void updateAPList();

    /**
     * Attempts to connect to a Wifi access point.  This will close any
     * connections to other access points.
     *
     *  @param accessPoint  The wifi device will attempt to find and connect
     *                      to this access point.
     */
    void connect(const WifiAccessPoint& accessPoint);

    /**
     * Tries to disconnect from a specific wifi access point.
     * 
     * @param accessPoint   If the system is currently connected to this
     *                      access point, this method closes that connection.
     */
    void disconnect(const WifiAccessPoint& accessPoint);

    /**
     * Attempts to connect or disconnect from the current selected access point
     * when the connection button is clicked.
     * 
     * @param button  This should always be the connection button.
     */
    virtual void listPageButtonClicked(juce::Button* button) override;

    /**
     * Keeps the page updated when wifi state changes.
     * 
     * @param state  The new wifi connection state.
     */
    void wifiStateChanged(WifiState state) override;
   
    /**
     * Adds all newly detected access points to the access point list.
     *
     * @param addedAP  The newly detected access point object.
     */
    void accessPointAdded(const WifiAccessPoint& addedAP) override;

    /**
     * Removes all missing access points from the access point list.
     *
     * @param removedAP  The access point that is no longer visible.
     */
    void accessPointRemoved(const WifiAccessPoint& removedAP) override;

    /**
     * Attempts to connect if return is pressed after entering a password.
     * 
     * @param editor  This should always be the password field.
     */
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;

    /**
     * Gets the asset name for the icon that best represents accessPoint's 
     * signal strength.
     * 
     * @param accessPoint
     */
    static juce::String getWifiAssetName(const WifiAccessPoint& accessPoint);
    
    //All visible access points.
    juce::Array<WifiAccessPoint> visibleAPs;

    //Holds recycled list item components
    juce::OwnedArray<ScalingLabel> apLabels;
    juce::OwnedArray<DrawableImageComponent> apIcons;
    juce::OwnedArray<DrawableImageComponent> lockIcons;
    
    //Wifi icon paths for all signal strengths
    static const juce::StringArray wifiImageFiles;
    
    /**
     * ConnectionButton is a TextButton with text that can be replaced by a
     * Spinner to indicate that wifi is busy.
     */
    class ConnectionButton : public juce::TextButton
    {
    public:
        ConnectionButton();
        
        virtual ~ConnectionButton() { }
      
        /**
         * Show or hide the spinner, ensuring the button is enabled when
         * text is visible and disabled when the spinner is visible.
         * 
         * @param showSpinner  True to show the spinner, false to show the
         *                     button text.
         */
        void setSpinnerVisible(bool showSpinner);
    private:
        /**
         * Ensures spinner bounds are updated with connection button bounds.
         */
        virtual void resized() override;
        
        Spinner spinner;
        //Holds the button text while the spinner is enabled.
        juce::String savedText;
    };
    
    WifiAccessPoint lastConnecting;
    //Displays the last connection time of access points with saved connections.
    ScalingLabel lastConnectionLabel;
    //Used for entering a password for a secured access point.
    ScalingLabel passwordLabel;
    juce::TextEditor passwordEditor;
    //clicked to connect or disconnect
    ConnectionButton connectionButton;
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
    static const constexpr char * last_connected = "last_connected";
    static const constexpr char * connected_ap = "connected_ap";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsPage)
};
