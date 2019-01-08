#pragma once
/**
 * @file Wifi/Component/SettingsPage.h
 * 
 * @brief Shows information on all visible Wifi access points, and provides 
 *        controls for connecting to or disconnecting from those access points.
 */

#include "FocusingListPage.h"
#include "Wifi_AP_StrengthListener.h"
#include "Wifi_APList_Listener.h"
#include "Wifi_Connection_Listener.h"
#include "Locale/TextUser.h"
#include "Spinner.h"
#include "ScalingLabel.h"
#include "DrawableImageComponent.h"

namespace Wifi { class SettingsPage; }

class Wifi::SettingsPage : public FocusingListPage,
        public Wifi::APList::Listener,
        public Wifi::SignalStrengthListener,
        public Wifi::Connection::Listener,
        public juce::TextEditor::Listener,
        public Locale::TextUser
{
public:
    SettingsPage();

    virtual ~SettingsPage() { }
    
protected:
    /**
     * @brief  Sets the number of items in the list to match the number of 
     *         visible Wifi access points.
     * 
     * @return  The number of WifiAccessPoint objects available. 
     */
    virtual unsigned int getListSize() final override;
    
    /**
     * @brief  Creates or updates the layout of one access point on the list.
     * 
     * @param layout  A layout object to update.  Components will be added or
     *                updated in this layout to match a particular Wifi access
     *                point.
     * 
     * @param index   An index in the list.  The layout will be updated to match
     *                the WifiAccessPoint with this index in the the sorted
     *                list of access points.
     */
    virtual void updateListItemLayout(Layout::Group::RelativeLayout& layout,
            const unsigned int index) final override;
    
    /**
     * @brief  Creates or updates the access point control/information panel 
     *         that appears when an access point in the list is selected.
     * 
     * @param layout   The control layout object, to be updated to match the 
     *                 current selected access point.
     */
    virtual void updateSelectedItemLayout(Layout::Group::RelativeLayout& layout) 
            final override;

private:
    /**
     * @brief  Reloads the list of wifi access points within range of the wifi 
     *         device, and updates the access point list.
     */
    void loadAccessPoints();
    
    /**
     * @brief  Removes any lost access points, sorts the access point list, and
     *         refreshes the access point list component.
     */
    void updateAPList();

    /**
     * @brief  Attempts to connect to a Wifi access point. This will close any
     *         connections to other access points.
     *
     *  @param accessPoint  The wifi device will attempt to find and connect
     *                      to this access point.
     */
    void connect(const AccessPoint accessPoint);

    /**
     * @brief  Tries to disconnect from a specific wifi access point.
     * 
     * @param accessPoint   If the system is currently connected to this
     *                      access point, this method closes that connection.
     */
    void disconnect(const AccessPoint accessPoint);

    /**
     * @brief  Attempts to connect or disconnect from the current selected 
     *         access point when the connection button is clicked.
     * 
     * @param button  This should always be the connection button.
     */
    virtual void listPageButtonClicked(juce::Button* button) final override;

    /**
     * @brief  Updates the list when access point signal strength changes.
     *
     * @param updatedAP  The AccessPoint with a new signal strength value.
     */
    virtual void signalStrengthUpdate(const AccessPoint updatedAP) 
        final override;

    /**
     * @brief  Adds a newly discovered access point to the list.
     *
     * @param newAP  The new Wifi::AccessPoint object.
     */
    virtual void accessPointAdded(const AccessPoint newAP) override;

    /**
     * @brief  Removes an access point from the list when it is no longer
     *         visible.
     *
     * @param removedAP  The access point that is no longer visible.
     */
    virtual void accessPointRemoved(const AccessPoint removedAP) override;

    /**
     * @brief  Updates access point connection controls when a connection starts
     *         to activate.
     *
     * @param connectingAP  The Wifi access point the system is trying to 
     *                      connect to.
     */
    virtual void startedConnecting(const AccessPoint connectingAP) override;

    /**
     * @brief  Updates access point connection controls and prints an error
     *         message when connection authentication fails.
     *
     * @param connectingAP  The Wifi access point that failed to connect.
     */
    virtual void connectionAuthFailed(const AccessPoint connectingAP) override;

    /**
     * @brief  Updates the access point list and connection controls when an 
     *         access point connects.
     *
     * @param connectedAP  The access point used to open the connection.
     */
    virtual void connected(const AccessPoint connectedAP) override; 

    /**
     * @brief  Updates the access point list and connection controls when an 
     *         access point disconnects.
     *
     * @param disconnectedAP  The access point that was being used by the closed
     *                        connection.
     */
    virtual void disconnected(const AccessPoint disconnectedAP) override; 

    /**
     * @brief  Attempts to connect if return is pressed after entering a 
     *         password.
     * 
     * @param editor  This should always be the password field.
     */
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;

    /**
     * @brief  Gets the asset name for the icon that best represents an access
     *         point's signal strength.
     * 
     * @param accessPoint  The access point used to select an icon.
     */
    static juce::String getWifiAssetName(const AccessPoint accessPoint);
    
    /* All visible access points. */
    juce::Array<AccessPoint> visibleAPs;

    /* Recycled list item components: */
    juce::OwnedArray<ScalingLabel> apLabels;
    juce::OwnedArray<DrawableImageComponent> apIcons;
    juce::OwnedArray<DrawableImageComponent> lockIcons;
    
    /* Wifi icon paths for all signal strengths: */
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
         * @brief  Shows or hide the spinner, ensuring the button is enabled 
         *         when text is visible and disabled when the spinner is 
         *         visible.
         * 
         * @param showSpinner  True to show the spinner, false to show the
         *                     button text.
         */
        void setSpinnerVisible(bool showSpinner);

    private:
        /**
         * @brief  Ensures the spinner bounds are updated with connection button 
         *         bounds.
         */
        virtual void resized() override;
        
        /* The loading spinner component: */
        Spinner spinner;

        /* Holds the button text while the spinner is enabled. */
        juce::String savedText;
    };
    
    /* Displays an access point's last connection time if applicable. */
    ScalingLabel lastConnectionLabel;

    /* Used for entering a password for a secured access point. */
    ScalingLabel passwordLabel;
    juce::TextEditor passwordEditor;

    /* Clicked to connect or disconnect: */
    ConnectionButton connectionButton;

    /* Shows an error message if the connection fails. */
    ScalingLabel errorLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPage)
};
