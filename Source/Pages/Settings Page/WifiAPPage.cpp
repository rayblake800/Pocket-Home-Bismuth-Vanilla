#include "../../PocketHomeApplication.h"
#include "WifiAPPage.h"

WifiAPPage::WifiAPPage(WifiAccessPoint ap) :
PageComponent("WifiAPPage",{
    {1,
        {
            {&connectionLabel, 1}
        }},
    {1,
        {
            {&passwordEditor, 1}
        }},
    {1,
        {
            {&connectionButton, 1}
        }},
    {1,
        {
            {&errorLabel, 1}
        }}
}, true),
wifiAP(ap),
connectionLabel("Connection Label", "Connected"),
passwordEditor("Password", (juce_wchar) 0x2022),
connectionButton("Connection Button"),
errorLabel("Error Text", "Bad password ...")
{
    connectionLabel.setJustificationType(juce::Justification::centred);
    errorLabel.setJustificationType(juce::Justification::centred);
    passwordEditor.addListener(this);
    connectionButton.addListener(this);
    // add the spinner image to our page
    connectionButton.addChildComponent(spinner);
    addAndShowLayoutComponents();
    WifiStatus& wifiStatus
            = PocketHomeApplication::getInstance()->getWifiStatus();
    wifiStatus.addListener(this);
    if (wifiStatus.isConnected())
    {
        handleWifiConnected();
    }
    else
    {
        handleWifiDisconnected();
    }
}

WifiAPPage::~WifiAPPage() { }

void WifiAPPage::setAccessPoint(WifiAccessPoint ap)
{
    if (wifiAP.ssid != ap.ssid)
    {
        wifiAP = ap;
        setCurrentlyConnecting(false);
        updateConnectionLabelAndButton();
    }
}

void WifiAPPage::setCurrentlyConnecting(bool currentlyConnecting)
{
    bool wifiEnabled = PocketHomeApplication::getInstance()
            ->getWifiStatus().isEnabled();
    spinner.setVisible(currentlyConnecting);
    if (currentlyConnecting)
    {
        connectionButton.setButtonText("");
    }
    else
    {
        updateConnectionLabelAndButton();
    }
    connectionButton.setEnabled(wifiEnabled && !currentlyConnecting);
    passwordEditor.setEnabled(wifiAP.requiresAuth && wifiEnabled
            && !currentlyConnecting);
    passwordEditor.setVisible(wifiAP.requiresAuth);
}

void WifiAPPage::updateConnectionLabelAndButton()
{
    const WifiStatus& status = PocketHomeApplication::getInstance()
            ->getWifiStatus();
    String ssidText = wifiAP.ssid;
    String buttonText = "Connect";
    errorLabel.setVisible(false);

    if (status.isConnected())
    {
        try
        {
            WifiAccessPoint connectedAp = status.connectedAccessPoint();
            if (connectedAp.hash != wifiAP.hash)
            {
                wifiAP = connectedAp;
                ssidText = wifiAP.ssid;
            }
            passwordEditor.setText("");
            ssidText += " (connected)";
            buttonText = "Disconnect";
        }
        catch (WifiStatus::MissingAccessPointException e)
        {
            DBG("WifiAPPage::updateConnectionLabelAndButton: connected, but no AP found");
        }
    }
    passwordEditor.setVisible(wifiAP.requiresAuth && !status.isConnected());
    connectionLabel.setText(ssidText,
            juce::NotificationType::dontSendNotification);
    connectionButton.setButtonText(buttonText);
}

void WifiAPPage::startConnection()
{
    WifiStatus& status = PocketHomeApplication::getInstance()->getWifiStatus();
    if (!status.isConnected())
    {
        errorLabel.setVisible(false);
        if (wifiAP.requiresAuth)
        {
            const auto& psk = passwordEditor.getTextValue().toString();
            status.setConnectedAccessPoint(wifiAP, psk);
        }
        else
        {
            status.setConnectedAccessPoint(wifiAP);
        }
    }
}

void WifiAPPage::startDisconnect()
{
    PocketHomeApplication::getInstance()->getWifiStatus().setDisconnected();
}

void WifiAPPage::handleWifiEnabled()
{
    DBG("WifiAPPage::wifiEnabled");
    setCurrentlyConnecting(false);
}

void WifiAPPage::handleWifiDisabled()
{
    setCurrentlyConnecting(false);
    PageStackComponent& mainStack = PocketHomeApplication::getInstance()
            ->getMainStack();
    //close wifi settings pages if wifi is disabled
    if (mainStack.getCurrentPage() == this)
    {
        mainStack.popPage(PageStackComponent::kTransitionTranslateHorizontal);
    }
}

void WifiAPPage::handleWifiConnected()
{
    setCurrentlyConnecting(false);
    passwordEditor.setVisible(false);
    errorLabel.setVisible(false);
    updateConnectionLabelAndButton();
}

void WifiAPPage::handleWifiDisconnected()
{
    setCurrentlyConnecting(false);
    if (wifiAP.requiresAuth)
    {
        passwordEditor.setVisible(true);
        passwordEditor.grabKeyboardFocus();
        errorLabel.setVisible(false);
    }
    updateConnectionLabelAndButton();
}

void WifiAPPage::handleWifiFailedConnect()
{
    setCurrentlyConnecting(false);
    if (wifiAP.requiresAuth)
    {
        passwordEditor.setText("");
        passwordEditor.grabKeyboardFocus();
        errorLabel.setVisible(true);
    }
}

void WifiAPPage::handleWifiBusy()
{
    setCurrentlyConnecting(true);
}

void WifiAPPage::textEditorReturnKeyPressed(TextEditor &)
{
    startConnection();
}

void WifiAPPage::pageButtonClicked(Button* button)
{
    if (button == &connectionButton)
    {
        WifiStatus &status = PocketHomeApplication::getInstance()
                ->getWifiStatus();
        if (status.isConnected())
        {
            startDisconnect();
        }
        else
        {
            startConnection();
        }
    }
}

void WifiAPPage::visibilityChanged()
{
    updateConnectionLabelAndButton();
}
