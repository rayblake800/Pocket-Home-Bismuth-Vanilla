/**
 * @file WifiSettingsPage.h
 * 
 * TODO: documentation and cleanup
 */

#pragma once
#include "../../Wifi/WifiStatus.h"
#include "../../Basic Components/ScalingLabel.h"
#include "../../Basic Components/SwitchComponent.h"
#include "../../Basic Components/DrawableImageButton.h"
#include "../../PageComponent.h"
#include "../../PageStackComponent.h"
#include "ConnectionPage.h"

class WifiSettingsPage : public ConnectionPage<WifiAccessPoint>, 
public WifiStatus::Listener{
public:
    WifiSettingsPage();
    ~WifiSettingsPage();
private:
	/**
     *Returns the list of all access points that should be listed.
     */
    virtual Array<WifiAccessPoint> loadConnectionList() override;
    
    
    /**
     * Attempts to open a connection, if possible.
     */
    virtual void connect(const WifiAccessPoint& connection) override;

    /**
     * Attempts to close a connection, if possible.
     */
    virtual void disconnect(const WifiAccessPoint& connection) override;

    /**
     * @return true iff the system is connected to WifiAccessPoint connection.
     */
    virtual bool isConnected(const WifiAccessPoint& connection) override;

    /**
     * This is called whenever a button other than the navigation buttons
     * is clicked.
     */
    virtual void connectionButtonClicked(Button* button)  override;

    /**
     * Get the layout for a single connection list item.
     */
    virtual std::vector<GridLayoutManager::RowLayoutParams>
    getConnectionLayout(WifiAccessPoint* connection) override;

    /**
     * Get the layout for a connection list item in expanded detail mode.
     * This should include whatever controls are needed to manage the
     * connection.
     */
    virtual std::vector<GridLayoutManager::RowLayoutParams>
    getConnectionDetailLayout(WifiAccessPoint* connection) override;
    
    void setCurrentlyConnecting(bool currentlyConnecting);
	
    void handleWifiEnabled() override;
    
    void handleWifiDisabled() override;
    
    void handleWifiConnected() override;
    
    void handleWifiDisconnected() override;
    
    void handleWifiBusy() override;
    
    void handleWifiFailedConnect() override;

    void textEditorReturnKeyPressed(TextEditor &) override;
    
    static const Array<String> wifiImageFiles;
    
    static String getWifiAssetName(const WifiAccessPoint& accessPoint);
    
	struct ListItemComponents{
		ListItemComponents(const WifiAccessPoint& accessPoint);
		const WifiAccessPoint& accessPoint;
		ScalingLabel connectionLabel;
		DrawableImageComponent wifiIcon;
		DrawableImageComponent lockIcon; 
	}
	OwnedArray<ListItemComponents> listItemComponents;
	TextEditor passwordEditor;
	TextButton connectionButton;
	ScalingLabel errorLabel;
	Spinner spinner;
	ListItemComponents* getAPComponents(const WifiAccessPoint& accessPoint);
	ListItemComponents* getSelectedAPComponents();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsPage)
};
