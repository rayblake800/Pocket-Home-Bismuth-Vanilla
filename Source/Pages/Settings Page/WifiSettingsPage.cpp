#include "Utils.h"
#include "PocketHomeApplication.h"
#include "WifiSettingsPage.h"

const StringArray WifiSettingsPage::wifiImageFiles = {
                                                      "wifiStrength0.svg",
                                                      "wifiStrength1.svg",
                                                      "wifiStrength2.svg",
                                                      "wifiStrength3.svg"
};

WifiSettingsPage::WifiSettingsPage() :
ConnectionPage<WifiAccessPoint>(),
passwordLabel("passwordLabel", "Password:")
{

#    if JUCE_DEBUG
    setName("WifiSettingsPage");
#    endif
    passwordEditor.addListener(this);
    connectionButton.addChildComponent(spinner);
    connectionButton.addListener(this);
    errorLabel.setJustificationType(Justification::centred);
    WifiStateManager& wifiManager = PocketHomeApplication::getInstance()
            ->getWifiManager();
    wifiManager.addListener(this);
    updateConnectionList();
}

WifiSettingsPage::~WifiSettingsPage() { }

/**
 * @return the list of all visible Wifi access points.
 */
Array<WifiAccessPoint> WifiSettingsPage::loadConnectionList()
{
    WifiStateManager& wifiManager = PocketHomeApplication::getInstance()
            ->getWifiManager();
    return wifiManager.getVisibleAPs();
}

/**
 * Attempts to connect to a Wifi access point.  This will close any
 * connections to other access points.
 */
void WifiSettingsPage::connect(const WifiAccessPoint& connection)
{
    WifiStateManager& wifiManager = PocketHomeApplication::getInstance()
            ->getWifiManager();
    if (connection.getRequiresAuth())
    {
        const String& psk = passwordEditor.getText();
        DBG("WifiSettingsPage::" << __func__ << ": connecting to "
                << connection.getSSID() << " with psk of length "
                << psk.length());
        wifiManager.connectToAccessPoint(connection, psk);
    }
    else
    {
        DBG("WifiSettingsPage::" << __func__ << ": connecting to "
                << connection.getSSID() << " with no psk required.");
        wifiManager.connectToAccessPoint(connection);
    }
    setCurrentlyConnecting(true);
    errorLabel.setVisible(false);
}

/**
 * @param connection if the system is currently connected to this
 * connection, this method closes that connection.
 */
void WifiSettingsPage::disconnect(const WifiAccessPoint& connection)
{
    WifiStateManager& wifiManager = PocketHomeApplication::getInstance()
            ->getWifiManager();
    wifiManager.disconnect(connection);
}

/**
 * @return true iff the system is connected to WifiAccessPoint connection.
 */
bool WifiSettingsPage::isConnected(const WifiAccessPoint& connection)
{
    WifiStateManager& wifiManager = PocketHomeApplication::getInstance()
            ->getWifiManager();
    if (connection.isNull())
    {
        return false;
    }
    return wifiManager.getConnectedAP() == connection;
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
RelativeLayoutManager::Layout WifiSettingsPage::getConnectionControlsLayout
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
    passwordEditor.setEnabled(passwordNeeded && !connectionChanging);
    passwordLabel.setVisible(passwordNeeded);
    String connectionText;
    if (!connectionChanging)
    {
        connectionText = connected ? "Disconnect" : "Connect";
    }
    connectionButton.setButtonText(connectionText);
    connectionButton.setEnabled(!connectionChanging);
    errorLabel.setText("", NotificationType::dontSendNotification);
    spinner.setVisible(connectionChanging);
}

/**
 * When currentlyConnecting, disable Wifi controls and show a loading
 * spinner.  Otherwise, enable controls and hide the loading spinner.
 */
void WifiSettingsPage::setCurrentlyConnecting(bool currentlyConnecting)
{
    if (currentlyConnecting != connectionChanging)
    {
        connectionChanging = currentlyConnecting;
        updateConnectionControls(getSelectedConnection());
    }
}

/**
 */
void WifiSettingsPage::wifiStateChanged(WifiStateManager::WifiState state)
{
    MessageManager::callAsync([this,state]()
    {
        switch (state)
        {
            case WifiStateManager::missingNetworkInterface:
            case WifiStateManager::noStateManager:
            case WifiStateManager::turningOn:
            case WifiStateManager::turningOff:
            case WifiStateManager::disabled:
                removeFromStack(PageStackComponent::Transition
                        ::kTransitionTranslateHorizontal);
                break;
            case WifiStateManager::connected:
                reloadPage();
                break;
            case WifiStateManager::enabled:
                if (connectionChanging)
                {
                    setCurrentlyConnecting(false);
                    updateConnectionControls(getSelectedConnection());
                    errorLabel.setText(getSelectedConnection().getRequiresAuth() ?
                            "Incorrect password." : "Connection failed.",
                            NotificationType::dontSendNotification);
                    errorLabel.setVisible(true);
                }
                else
                {
                    reloadPage();
                }
                break;
            case WifiStateManager::connecting:
            case WifiStateManager::disconnecting:
            case WifiStateManager::switchingConnection:
                setCurrentlyConnecting(true);
                break;
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
