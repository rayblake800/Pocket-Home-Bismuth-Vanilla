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
ConnectionPage<WifiAccessPoint::Ptr>(),
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

/**
 * Finds all wifi access points within range of the wifi device.
 */
Array<WifiAccessPoint::Ptr> WifiSettingsPage::loadConnectionList()
{
    WifiStateManager wifiManager;
    return wifiManager.getVisibleAPs();
}

/**
 * Attempts to connect to a Wifi access point.  This will close any
 * connections to other access points.
 */
void WifiSettingsPage::connect(WifiAccessPoint::Ptr connection)
{
    if (connection == nullptr)
    {
        DBG("WifiSettingsPage::" << __func__ << ": ap is null!");
        return;
    }
    WifiStateManager wifiManager;
    lastConnecting = connection;
    if (connection->getRequiresAuth())
    {
        const String& psk = passwordEditor.getText();
        DBG("WifiSettingsPage::" << __func__ << ": connecting to "
                << connection->getSSID() << " with psk of length "
                << psk.length());
        wifiManager.connectToAccessPoint(connection, psk);
    }
    else
    {
        DBG("WifiSettingsPage::" << __func__ << ": connecting to "
                << connection->getSSID() << " with no psk required.");
        wifiManager.connectToAccessPoint(connection);
    }
    errorLabel.setVisible(false);
}

/**
 * Tries to disconnect from a specific wifi access point.
 */
void WifiSettingsPage::disconnect(WifiAccessPoint::Ptr connection)
{
    WifiStateManager wifiManager;
    if(connection == wifiManager.getConnectingAP())
    {
        lastDisconnecting = connection;
        wifiManager.stopConnecting();
    }
    else if(connection == wifiManager.getConnectedAP())
    {
        lastDisconnecting = connection;
        wifiManager.disconnect();
    }
    else
    {
        DBG("WifiSettingsPage::" << __func__ 
                << ": ap is not connected/connecting!");
    }
}

/**
 * @return true iff the system is connected to WifiAccessPoint connection.
 */
bool WifiSettingsPage::isConnected(WifiAccessPoint::Ptr connection)
{
    WifiStateManager wifiManager;
    if (connection == nullptr)
    {
        return false;
    }
    return wifiManager.getConnectedAP() == connection;
}

/**
 * Attempt to connect or disconnect from the current selected access point
 * when the connection button is clicked.
 */
void WifiSettingsPage::connectionButtonClicked(Button* button)
{
    if (&connectionButton == button)
    {
        WifiAccessPoint::Ptr selectedAP = getSelectedConnection();
        if (isConnected(selectedAP))
        {
            disconnect(selectedAP);
        }
        else
        {
            connect(selectedAP);
        }
    }
}

/**
 * Construct a button component to represent a wifi access point.
 */
Button* WifiSettingsPage::getConnectionButton
(WifiAccessPoint::Ptr connection)
{
    if(connection != nullptr)
    {
        return new WifiAPButton(connection, isConnected(connection));
    }
    else
    {
        DBG("WifiSettingsPage::" << __func__ << ": connection is null!");
        return nullptr;
    }
}

/**
 * Get the layout for the Wifi access point controls.
 */
RelativeLayoutManager::Layout WifiSettingsPage::getConnectionControlsLayout
(WifiAccessPoint::Ptr connection)
{
    updateConnectionControls(connection);
    return {
        {1,
            {
                {nullptr, 1}
            }},
        {1,
            {
                {&debugLabel, 1}
            }},
        {2,
            {
                {nullptr, 1},
                {connection->getRequiresAuth() ? &passwordLabel : nullptr, 2},
                {connection->getRequiresAuth() ? &passwordEditor : nullptr, 4},
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

/**
 * Update connection control components to match the current Wifi connection
 * state and the provided Wifi access point.
 */
void WifiSettingsPage::updateConnectionControls
(WifiAccessPoint::Ptr accessPoint)
{
    if(accessPoint == nullptr)
    {
        DBG("WifiSettingsPage::" << __func__ << ": null AP selected");
        return;
    }
    bool showPasswordEntry = false;
    String connectionBtnText = localeText(btn_connect);
    bool showButtonSpinner = false;
    String errorMessage = "";
    bool apConnected = isConnected(accessPoint);
    //debug text
    String debug = "Saved=";
    debug += (accessPoint->hasSavedConnection() ? "yes " : "no ");
    debug += "   SavedPsk="
    debug += (accessPoint->hasSavedPsk() ? "yes " : "no ");
    debugLabel.setText(debug);
    if(!apConnected && lastConnecting != accessPoint 
            && lastDisconnecting != accessPoint)
    {
        showPasswordEntry = accessPoint->getRequiresAuth()
                && !accessPoint->hasSavedPsk();
    }
    else
    {
        WifiStateManager wifiManager;
        switch(wifiManager.getWifiState())
        {
            case WifiStateManager::connecting:
                if(lastConnecting == accessPoint)
                {
                    showButtonSpinner = true;
                }
                break;
            case WifiStateManager::missingPassword:
                showPasswordEntry = accessPoint->getRequiresAuth();
                if(accessPoint == lastConnecting)
                {
                    errorMessage = localeText(wrong_password);
                }
                break;
            case WifiStateManager::connected:
                if(apConnected)
                {
                    connectionBtnText = localeText(btn_disconnect);
                    break;
                }    
            case WifiStateManager::enabled:
                if(lastConnecting == accessPoint 
                   && lastDisconnecting != accessPoint)
                {
                    errorMessage = localeText(connection_failed);
                }
                showPasswordEntry = accessPoint->getRequiresAuth()
                        && !accessPoint->hasSavedPsk();
                break;
            case WifiStateManager::disconnecting:
                if(lastDisconnecting == accessPoint)
                {
                    showButtonSpinner = true;
                }
            default:
                DBG("WifiSettingsPage::" << __func__ << ": page should be closed!");
        };
    }
    if(showPasswordEntry != passwordEditor.isVisible())
    {
        passwordEditor.clear();
        passwordEditor.setEnabled(showPasswordEntry);
        passwordEditor.setVisible(showPasswordEntry);
        passwordLabel.setVisible(showPasswordEntry);
    }
    if(showButtonSpinner)
    {
        connectionBtnText = String();
    }
    connectionButton.setButtonText(connectionBtnText);
    connectionButton.setEnabled(!showButtonSpinner);
    spinner.setVisible(showButtonSpinner);
    errorLabel.setText(errorMessage, NotificationType::dontSendNotification);
}


/**
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
                lastConnecting = nullptr;
            default:
                reloadPage();
        }
    });
}

/**
 * Attempt to connect if return is pressed after entering a password.
 */
void WifiSettingsPage::textEditorReturnKeyPressed(TextEditor & editor)
{
    connect(getSelectedConnection());
}

/**
 * Set the spinner's bounds within the connection button
 */
void WifiSettingsPage::connectionPageResized()
{
    int spinnerSize = connectionButton.getHeight();

    spinner.setBounds(connectionButton.getLocalBounds().
            withSizeKeepingCentre(spinnerSize, spinnerSize));
}

/**
 * Get the asset name for the icon that best represents accessPoint's 
 * signal strength.
 */
String WifiSettingsPage::getWifiAssetName(WifiAccessPoint::Ptr accessPoint)
{
    float sigStrength = accessPoint->getSignalStrength();
    int maxIndex = wifiImageFiles.size() - 1;

    return wifiImageFiles[round((maxIndex * (sigStrength) / 100.0f))];
}

/**
 * Reload the access point list, re-select the selected connection, 
 * update and enable connection controls.
 */
void WifiSettingsPage::reloadPage()
{
    WifiAccessPoint::Ptr selected = getSelectedConnection();
    updateConnectionList();
    setSelectedConnection(selected);
    updateConnectionControls(getSelectedConnection());
}

WifiSettingsPage::WifiAPButton::WifiAPButton(
        WifiAccessPoint::Ptr connection,
        bool isConnected) :
Button(connection->getSSID() + "Button"),
Localized("WifiAPButton"),
apLabel("apLabel", connection->getSSID() 
+ (isConnected ? localeText(connected_ap) : String())),
wifiIcon(getWifiAssetName(connection))
{
    Colour iconColour = findColour(Label::textColourId);
    wifiIcon.setColour(DrawableImageComponent::imageColour0Id, iconColour);
    addAndMakeVisible(apLabel);
    addAndMakeVisible(wifiIcon);
    apLabel.setJustificationType(Justification::centred);
    apLabel.setInterceptsMouseClicks(false, false);
    if (connection->getRequiresAuth())
    {

        lockIcon = new DrawableImageComponent("lock.svg");
        lockIcon->setColour(DrawableImageComponent::imageColour0Id, iconColour);
        addAndMakeVisible(lockIcon);
    }
}

/**
 * Update icon and label bounds to fit button bounds.
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
