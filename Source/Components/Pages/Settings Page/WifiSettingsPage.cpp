#include "Utils.h"
#include "WifiSettingsPage.h"

const StringArray WifiSettingsPage::wifiImageFiles
        = {
           "wifiStrength0.svg",
           "wifiStrength1.svg",
           "wifiStrength2.svg",
           "wifiStrength3.svg"
};

WifiSettingsPage::WifiSettingsPage() :
ConnectionPage<WifiAccessPoint>(),
passwordLabel("passwordLabel", localeText(password_field)),
Localized("WifiSettingsPage")
{
#    if JUCE_DEBUG
    setName("WifiSettingsPage");
#    endif
    passwordEditor.addListener(this);
    connectionButton.addChildComponent(spinner);
    connectionButton.addListener(this);
    errorLabel.setJustificationType(Justification::centred);
    WifiStateManager wifiManager;
    wifiManager.addListener(this);
    updateConnectionList();
}

/*
 * Finds all wifi access points within range of the wifi device.
 */
Array<WifiAccessPoint> WifiSettingsPage::loadConnectionPoints()
{
    WifiStateManager wifiManager;
    return wifiManager.getVisibleAPs();
}

/*
 * Attempts to connect to a Wifi access point.  This will close any
 * connections to other access points.
 */
void WifiSettingsPage::connect(WifiAccessPoint accessPoint)
{
    if (accessPoint.isNull())
    {
        DBG("WifiSettingsPage::" << __func__ << ": ap is null!");
        return;
    }
    const String& psk = passwordEditor.getText();
    if (passwordEditor.isVisible() && !accessPoint.isValidKeyFormat(psk))
    {
        errorLabel.setText(localeText(invalid_key_format),
                NotificationType::dontSendNotification);
        errorLabel.setVisible(true);
        return;
    }
    WifiStateManager wifiManager;
    lastConnecting = accessPoint;
    if (accessPoint.getRequiresAuth())
    {
        DBG("WifiSettingsPage::" << __func__ << ": connecting to "
                << accessPoint.getSSID() << " with psk of length "
                << psk.length());
        wifiManager.connectToAccessPoint(accessPoint, psk);
    }
    else
    {
        DBG("WifiSettingsPage::" << __func__ << ": connecting to "
                << accessPoint.getSSID() << " with no psk required.");
        wifiManager.connectToAccessPoint(accessPoint);
    }
    errorLabel.setVisible(false);
}

/*
 * Tries to disconnect from a specific wifi access point.
 */
void WifiSettingsPage::disconnect(WifiAccessPoint accessPoint)
{
    WifiStateManager wifiManager;
    if(accessPoint == wifiManager.getActiveAP())
    {
        lastDisconnecting = accessPoint;
        wifiManager.disconnect();
    }
    else
    {
        DBG("WifiSettingsPage::" << __func__ 
                << ": ap is not connected/connecting!");
    }
}

/*
 * Checks if wifi is connected to a specific access point.
 */
bool WifiSettingsPage::isConnected(WifiAccessPoint accessPoint)
{
    if (accessPoint.isNull())
    {
        return false;
    }
    WifiStateManager wifiManager;
    return wifiManager.getAPState(accessPoint) == 
            WifiStateManager::connectedAP;
}

/*
 * Attempts to connect or disconnect from the current selected access point
 * when the connection button is clicked.
 */
void WifiSettingsPage::connectionButtonClicked(Button* button)
{
    if (&connectionButton == button)
    {
        WifiAccessPoint selectedAP = getSelectedConnection();
        if (isConnected(selectedAP))
        {
            DBG("WifiSettingsPage::" << __func__ << ": Disconnecting from "
                    << selectedAP.getSSID());
            disconnect(selectedAP);
        }
        else
        {
            DBG("WifiSettingsPage::" << __func__ << ": Connecting to "
                    << selectedAP.getSSID());
            connect(selectedAP);
        }
    }
}

/*
 * Constructs a button component to represent a wifi access point.
 */
Button* WifiSettingsPage::getConnectionButton
(WifiAccessPoint accessPoint)
{
    if(!accessPoint.isNull())
    {
        return new WifiAPButton(accessPoint, isConnected(accessPoint));
    }
    else
    {
        DBG("WifiSettingsPage::" << __func__ << ": connection is null!");
        return nullptr;
    }
}

/*
 * Gets the layout for the Wifi access point controls.
 */
RelativeLayoutManager::Layout WifiSettingsPage::getConnectionControlsLayout
(WifiAccessPoint accessPoint)
{
    jassert(accessPoint == getSelectedConnection());
    updateConnectionControls();
    return {
        {1,
            {
                {nullptr, 1}
            }},
        {2,
            {
                {nullptr, 1},
                {accessPoint.getRequiresAuth() ? &passwordLabel : nullptr, 2},
                {accessPoint.getRequiresAuth() ? &passwordEditor : nullptr, 4},
                {nullptr, 1}
            }},
        {2,
            {
                {nullptr, 1},
                {&connectionButton, 1},
                {nullptr, 1}
            }},
        {2,
            {
                {&errorLabel, 1}
            }}};
}

/*
 * Updates connection control components to match the current Wifi connection 
 * state.
 */
void WifiSettingsPage::updateConnectionControls()
{
    WifiAccessPoint selectedAP = getSelectedConnection();
    if(selectedAP.isNull())
    {
        DBG("WifiSettingsPage::" << __func__ << ": null AP selected");
        return;
    }
    bool showPasswordEntry = false;
    String connectionBtnText = localeText(btn_connect);
    bool showButtonSpinner = false;
    bool hideConnectionButton = false;
    String errorMessage = "";
    WifiStateManager wifiManager;
    switch(wifiManager.getAPState(selectedAP))
    {
        case WifiStateManager::nullAP:
            DBG("WifiSettingsPage::" << __func__ 
                    << ": AP is suddenly null!");
            return;
        case WifiStateManager::connectingAP:
        case WifiStateManager::disconnectingAP:
            connectionBtnText = "";
            showButtonSpinner = true;
            break;
        case WifiStateManager::connectedAP:
            connectionBtnText = localeText(btn_disconnect);
            break;
        case WifiStateManager::disconnectedAP:
            showPasswordEntry = selectedAP.getRequiresAuth() &&
                    selectedAP.getSavedConnectionPath().isEmpty();
            break;
        case WifiStateManager::invalidSecurityAP:
            showPasswordEntry = selectedAP.getRequiresAuth();
            errorMessage = localeText(wrong_password);
            break;
        case WifiStateManager::missingAP:
            errorMessage = localeText(lost_ap);
            hideConnectionButton = true;
            
    }
    if(showPasswordEntry != passwordEditor.isVisible())
    {
        passwordEditor.clear();
        passwordEditor.setEnabled(showPasswordEntry);
        passwordEditor.setVisible(showPasswordEntry);
        passwordLabel.setVisible(showPasswordEntry);
    }
    if(hideConnectionButton)
    {
        connectionButton.setVisible(false);
    }
    else
    {
        connectionButton.setVisible(true);
        if(showButtonSpinner)
        {
            connectionBtnText = String();
        }
        connectionButton.setButtonText(connectionBtnText);
        connectionButton.setEnabled(!showButtonSpinner);
        spinner.setVisible(showButtonSpinner);
    }
    errorLabel.setText(errorMessage, NotificationType::dontSendNotification);
}


/*
 * Keeps the page updated when wifi state changes.
 */
void WifiSettingsPage::wifiStateChanged(WifiStateManager::WifiState state)
{
    MessageManager::callAsync([this, state]()
    {
        switch (state)
        {
            case WifiStateManager::missingNetworkDevice:
            case WifiStateManager::turningOn:
            case WifiStateManager::turningOff:
            case WifiStateManager::disabled:
                removeFromStack();
                break;
            case WifiStateManager::connected:
                lastConnecting = WifiAccessPoint();
            default:
                reloadPage();
        }
    });
}

/*
 * Attempts to connect if return is pressed after entering a password.
 */
void WifiSettingsPage::textEditorReturnKeyPressed(TextEditor & editor)
{
    connect(getSelectedConnection());
}

/*
 * Sets the spinner's bounds within the connection button
 */
void WifiSettingsPage::connectionPageResized()
{
    int spinnerSize = connectionButton.getHeight();
    spinner.setBounds(connectionButton.getLocalBounds().
            withSizeKeepingCentre(spinnerSize, spinnerSize));
}

/*
 * Gets the asset name for the icon that best represents accessPoint's 
 * signal strength.
 */
String WifiSettingsPage::getWifiAssetName(WifiAccessPoint accessPoint)
{
    float sigStrength = accessPoint.getSignalStrength();
    int maxIndex = wifiImageFiles.size() - 1;
    return wifiImageFiles[round((maxIndex * (sigStrength) / 100.0f))];
}

/**
 * Reloads the access point list, re-selects the selected connection, 
 * updates and enables connection controls.
 */
void WifiSettingsPage::reloadPage()
{
    WifiAccessPoint selected = getSelectedConnection();
    updateConnectionList();
    setSelectedConnection(selected);
    updateConnectionControls();
}

WifiSettingsPage::WifiAPButton::WifiAPButton(
        WifiAccessPoint accessPoint,
        bool isConnected) :
Button(accessPoint.getSSID() + "Button"),
Localized("WifiAPButton"),
apLabel("apLabel", accessPoint.getSSID() 
+ (isConnected ? localeText(connected_ap) : String())),
wifiIcon(getWifiAssetName(accessPoint))
{
    Colour iconColour = findColour(Label::textColourId);
    wifiIcon.setColour(DrawableImageComponent::imageColour0Id, iconColour);
    addAndMakeVisible(apLabel);
    addAndMakeVisible(wifiIcon);
    apLabel.setJustificationType(Justification::centred);
    apLabel.setInterceptsMouseClicks(false, false);
    if (accessPoint.getRequiresAuth())
    {
        lockIcon = new DrawableImageComponent("lock.svg");
        lockIcon->setColour(DrawableImageComponent::imageColour0Id, iconColour);
        addAndMakeVisible(lockIcon);
    }
}

/*
 * Updates icon and label bounds to fit button bounds.
 */
void WifiSettingsPage::WifiAPButton::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    Rectangle<int> iconBounds = bounds.withLeft
            (bounds.getRight() - bounds.getHeight());
    wifiIcon.setBounds(iconBounds.reduced(bounds.getHeight() / 12));
    if (lockIcon != nullptr)
    {
        iconBounds.setX(iconBounds.getX() - iconBounds.getWidth());
        lockIcon->setBounds(iconBounds.reduced(bounds.getHeight() / 12));
    }
    apLabel.setBounds(bounds.withRight(iconBounds.getX()));
}
