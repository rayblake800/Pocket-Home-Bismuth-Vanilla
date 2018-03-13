#include "../../PokeLookAndFeel.h"
#include "../../Utils.h"
#include "WifiSettingsPage.h"

const Array<String>
        WifiSettingsPage::wifiImageFiles = {
                                            "wifiStrength0.svg",
                                            "wifiStrength1.svg",
                                            "wifiStrength2.svg",
                                            "wifiStrength3.svg"
};

WifiSettingsPage::WifiSettingsPage() :
ConnectionPage<WifiAccessPoint>(),
passwordLabel("passwordLabel", "Password:")
{

#if JUCE_DEBUG
    setName("WifiSettingsPage");
#endif
    passwordEditor.addListener(this);
    connectionButton.addChildComponent(spinner);
    connectionButton.addListener(this);
    errorLabel.setJustificationType(Justification::centred);
    WifiStatus* wifiStatus = WifiStatus::getInstance();
    if (wifiStatus != nullptr)
    {
        wifiStatus->addListener(this);
        if (wifiStatus->isConnected())
        {
            handleWifiEvent(WifiStatus::wifiConnected);
        }
    }
    else
    {
        DBG("WifiSettingsPage::" << __func__ << ": WifiStatus is null!");
    }
}

WifiSettingsPage::~WifiSettingsPage() { }

/**
 * @return the list of all visible Wifi access points.
 */
Array<WifiAccessPoint> WifiSettingsPage::loadConnectionList()
{
    WifiStatus* wifiStatus = WifiStatus::getInstance();
    if (wifiStatus == nullptr)
    {
        DBG("WifiSettingsPage::" << __func__ << ": WifiStatus is null!");
        return {};
    }
    return wifiStatus->nearbyAccessPoints();
}

/**
 * Attempts to connect to a Wifi access point.  This will close any
 * connections to other access points.
 */
void WifiSettingsPage::connect(const WifiAccessPoint& connection)
{
    WifiStatus* wifiStatus = WifiStatus::getInstance();
    if (wifiStatus == nullptr)
    {
        DBG("WifiSettingsPage::" << __func__ << ": WifiStatus is null!");
        return;
    }
    if (wifiStatus->isConnected())
    {
        wifiStatus->disconnect();
    }
    setCurrentlyConnecting(true);
    errorLabel.setVisible(false);
    if (connection.getRequiresAuth())
    {
        const auto& psk = passwordEditor.getTextValue().toString();
        wifiStatus->setConnectedAccessPoint(connection, psk);
    }
    else
    {
        wifiStatus->setConnectedAccessPoint(connection);
    }
}

/**
 * @param connection if the system is currently connected to this
 * connection, this method closes that connection.
 */
void WifiSettingsPage::disconnect(const WifiAccessPoint& connection)
{
    WifiStatus* wifiStatus = WifiStatus::getInstance();
    if (wifiStatus != nullptr && isConnected(connection))
    {
        wifiStatus->disconnect();
    }
}

/**
 * @return true iff the system is connected to WifiAccessPoint connection.
 */
bool WifiSettingsPage::isConnected(const WifiAccessPoint& connection)
{
    WifiStatus* wifiStatus = WifiStatus::getInstance();
    if (connection.isNull() || wifiStatus == nullptr)
    {
        return false;
    }
    return wifiStatus->connectedAccessPoint() == connection;
}

/**
 * This is called whenever a button other than the navigation buttons
 * is clicked.
 */
void WifiSettingsPage::connectionButtonClicked(Button* button)
{
    if (&connectionButton == button)
    {
        const WifiAccessPoint& selectedAP = getSelectedConnection();
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
(const WifiAccessPoint& connection)
{
    return new WifiAPButton(connection, isConnected(connection));
}

/**
 * Get the layout for the Wifi access point controls.
 */
GridLayoutManager::Layout WifiSettingsPage::getConnectionControlsLayout
(const WifiAccessPoint& connection)
{
    updateConnectionControls(connection);
    return {
        {1,
            {
                {nullptr, 1}
            }},
        {2,
            {
                {nullptr, 1},
                {connection.getRequiresAuth() ? &passwordLabel : nullptr, 2},
                {connection.getRequiresAuth() ? &passwordEditor : nullptr, 4},
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
(const WifiAccessPoint& accessPoint)
{
    passwordEditor.clear();
    bool connected = isConnected(accessPoint);
    bool passwordNeeded = accessPoint.getRequiresAuth() && !connected;
    passwordEditor.setVisible(passwordNeeded);
    passwordEditor.setEnabled(passwordNeeded && !wifiBusy);
    passwordLabel.setVisible(passwordNeeded);
    String connectionText;
    if (!wifiBusy)
    {
        connectionText = connected ? "Disconnect" : "Connect";
    }
    connectionButton.setButtonText(connectionText);
    connectionButton.setEnabled(!wifiBusy);
    errorLabel.setText("", NotificationType::dontSendNotification);
    spinner.setVisible(wifiBusy);
}

/**
 * When currentlyConnecting, disable Wifi controls and show a loading
 * spinner.  Otherwise, enable controls and hide the loading spinner.
 */
void WifiSettingsPage::setCurrentlyConnecting(bool currentlyConnecting)
{
    if (currentlyConnecting != wifiBusy)
    {
        wifiBusy = currentlyConnecting;
        updateConnectionControls(getSelectedConnection());
    }
}

/**
 * When wifi is enabled, connects, or disconnects, reload page contents.
 * When wifi is disabled, close this page.
 * When wifi is busy, disable connection controls.
 * When wifi fails to connect, show the error label and enable connection
 * controls.
 */
void WifiSettingsPage::handleWifiEvent(WifiStatus::WifiEvent event)
{
    DBG("WifiSettingsPage::" << __func__ << ": handling wifi event");
    switch (event)
    {
        case WifiStatus::wifiEnabled:
        case WifiStatus::wifiConnected:
        case WifiStatus::wifiDisconnected:
            reloadPage();
            break;
        case WifiStatus::wifiDisabled:
            removeFromStack(PageStackComponent::Transition
                    ::kTransitionTranslateHorizontal);
            break;
        case WifiStatus::wifiBusy:
            setCurrentlyConnecting(true);
            break;
        case WifiStatus::wifiConnectionFailed:
            setCurrentlyConnecting(false);
            updateConnectionControls(getSelectedConnection());
            errorLabel.setText(getSelectedConnection().getRequiresAuth() ?
                    "Incorrect password." : "Connection failed.",
                    NotificationType::dontSendNotification);
            errorLabel.setVisible(true);
    }
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
String WifiSettingsPage::getWifiAssetName(const WifiAccessPoint & accessPoint)
{
    float sigStrength = accessPoint.getSignalStrength();
    int maxIndex = wifiImageFiles.size() - 1;

    return wifiImageFiles[round((maxIndex * (sigStrength) / 100.0f))];
}

/**
 * Reload the access point list, re-select the selected connection, 
 * update and enable connection controls.
 */
void WifiSettingsPage::reloadPage()
{
    WifiAccessPoint selected = getSelectedConnection();
    updateConnectionList();
    setSelectedConnection(selected);
    setCurrentlyConnecting(false);
    updateConnectionControls(getSelectedConnection());
}

WifiSettingsPage::WifiAPButton::WifiAPButton
(const WifiAccessPoint& connection, bool isConnected) :
Button(connection.getSSID() + "Button"),
apLabel("apLabel", connection.getSSID() + (isConnected ? " (Connected)" : "")),
wifiIcon(getWifiAssetName(connection))
{
    addAndMakeVisible(apLabel);
    addAndMakeVisible(wifiIcon);
    apLabel.setJustificationType(Justification::centred);
    apLabel.setInterceptsMouseClicks(false, false);
    if (connection.getRequiresAuth())
    {

        lockIcon = new DrawableImageComponent("lock.svg");
        addAndMakeVisible(lockIcon);
    }
}

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
