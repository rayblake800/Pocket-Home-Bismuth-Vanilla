/**
 * @file WifiAPPage.h
 * 
 * TODO: documentation
 */
#pragma once
#include "../../Wifi/WifiStatus.h"
#include "../../Basic Components/Spinner.h"
#include "../../Basic Components/ScalingLabel.h"
#include "../../PageComponent.h"
#include "../../../JuceLibraryCode/JuceHeader.h"
class WifiAPPage : public PageComponent, public WifiStatus::Listener,
private TextEditor::Listener {
public:
    WifiAPPage(WifiAccessPoint ap);
    ~WifiAPPage();
    
    void setAccessPoint(WifiAccessPoint ap);
    
private:
    void setCurrentlyConnecting(bool currentlyConnecting);
    void updateConnectionLabelAndButton();
    void startConnection();
    void startDisconnect();
    
    void handleWifiEnabled() override;
    void handleWifiDisabled() override;
    void handleWifiConnected() override;
    void handleWifiDisconnected() override;
    void handleWifiFailedConnect() override;
    void handleWifiBusy() override;
    void textEditorReturnKeyPressed(TextEditor &) override;
    void pageButtonClicked(Button* button) override;
    void visibilityChanged();
    
    WifiAccessPoint wifiAP;
    ScalingLabel connectionLabel;
    TextEditor passwordEditor;
    TextButton connectionButton;
    ScalingLabel errorLabel;
    Spinner spinner;
};
