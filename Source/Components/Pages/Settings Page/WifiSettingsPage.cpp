#include "Utils.h"
#include "LocalizedTime.h"
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
 * Compares wifi access points, in order to sort the access point list.
 * The connected access point will always come before all others, and
 * saved access points will come before new ones.  Otherwise, access points
 * are sorted by signal strength.
 */
int WifiSettingsPage::compareConnectionPoints
(const WifiAccessPoint& first, const WifiAccessPoint& second)
{
    jassert(!first.isNull() && !second.isNull());
    WifiStateManager wifiManager;
    WifiAccessPoint activeAP = wifiManager.getActiveAP();
    if (!activeAP.isNull())
    {
        if (first == activeAP)
        {
            return -1;
        }
        else if (second == activeAP)
        {
            return 1;
        }
    }
    bool firstSaved = first.getSavedConnectionPath().isNotEmpty();
    bool secondSaved = second.getSavedConnectionPath().isNotEmpty();
    if (firstSaved && !secondSaved)
    {
        return -1;
    }
    else if (secondSaved && !firstSaved)
    {
        return 1;
    }
    return second.getSignalStrength() - first.getSignalStrength();
}

/*
 * Attempts to connect to a Wifi access point.  This will close any
 * connections to other access points.
 */
void WifiSettingsPage::connect(const WifiAccessPoint& accessPoint)
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
void WifiSettingsPage::disconnect(const WifiAccessPoint& accessPoint)
{
    WifiStateManager wifiManager;
    if (accessPoint == wifiManager.getActiveAP())
    {
        wifiManager.disconnect();
    }
    else
    {
        DBG("WifiSettingsPage::" << __func__
                << ": ap is not connected/connecting!");
    }
}

/*
 * Attempts to connect or disconnect from the current selected access point
 * when the connection button is clicked.
 */
void WifiSettingsPage::connectionButtonClicked(Button* button)
{
    if (&connectionButton == button)
    {
        WifiStateManager wifiManager;
        const WifiAccessPoint& selectedAP = getSelectedConnection();
        switch (wifiManager.getAPState(selectedAP))
        {
            case WifiStateManager::connectedAP:
                DBG("WifiSettingsPage::" << __func__ << ": Disconnecting from "
                        << selectedAP.getSSID());
                disconnect(selectedAP);
                return;
            case WifiStateManager::disconnectedAP:
            case WifiStateManager::invalidSecurityAP:
                DBG("WifiSettingsPage::" << __func__ << ": Connecting to "
                        << selectedAP.getSSID());
                connect(selectedAP);
                return;
            default:
                DBG("WifiSettingsPage::" << __func__
                        << ": The connection button should have been disabled "
                        << "or hidden!");
        }
    }
}

/*
 * Constructs a button component to represent a wifi access point.
 */
Button* WifiSettingsPage::getConnectionButton
(const WifiAccessPoint& accessPoint)
{
    if (!accessPoint.isNull())
    {
        WifiStateManager wifiManager;
        WifiStateManager::AccessPointState apState 
                = wifiManager.getAPState(accessPoint);
        return new WifiAPButton(accessPoint,
                apState == WifiStateManager::connectedAP ||
                apState == WifiStateManager::disconnectingAP);
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
(const WifiAccessPoint& accessPoint)
{
    jassert(accessPoint == getSelectedConnection());
    RelativeLayoutManager::Layout controlLayout = {
	{1,
	    {
	        {nullptr,1}
	    }},
	{2,
            {
                {accessPoint.getRequiresAuth() ? &passwordLabel : nullptr, 4},
                {accessPoint.getRequiresAuth() ? &passwordEditor : nullptr, 5}
            }},
        {1,
	    {
	        {nullptr,1}
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
            }}
    };
    if (accessPoint.getSavedConnectionPath().isNotEmpty())
    {
        RelativeLayoutManager::RowLayout row = 
        {2,
            {
                {nullptr, 1},
                {&lastConnectionLabel, 4},
                {nullptr, 1}
            }};
        controlLayout.insert(controlLayout.begin(), row);
    }
    return controlLayout;
}

/*
 * Updates connection control components to match the current Wifi connection 
 * state.
 */
void WifiSettingsPage::updateConnectionControls()
{
    const WifiAccessPoint& selectedAP = getSelectedConnection();
    if (selectedAP.isNull())
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
    if(selectedAP.getSavedConnectionPath().isNotEmpty())
    {
        LocalizedTime connTime(wifiManager.lastConnectionTime(selectedAP));
        lastConnectionLabel.setText(localeText(last_connected)
		+ connTime.approxTimePassed(),
                NotificationType::dontSendNotification);
    }
    DBG("WifiSettingsPage::" << __func__ << ": Updating connection controls for"
            " AP " << selectedAP.getSSID() << " with state "
            << WifiStateManager::apStateString
            (wifiManager.getAPState(selectedAP)));
    switch (wifiManager.getAPState(selectedAP))
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
    passwordEditor.clear();
    passwordEditor.setEnabled(showPasswordEntry);
    passwordEditor.setVisible(showPasswordEntry);
    passwordLabel.setVisible(showPasswordEntry);
    if (hideConnectionButton)
    {
        connectionButton.setVisible(false);
    }
    else
    {
        connectionButton.setVisible(true);
        if (showButtonSpinner)
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
    WifiStateManager wifiManager;
    WifiAccessPoint activeAP = wifiManager.getActiveAP();
    switch (state)
    {
        case WifiStateManager::missingNetworkDevice:
        case WifiStateManager::turningOn:
        case WifiStateManager::turningOff:
        case WifiStateManager::disabled:
            removeFromStack();
            break;
        case WifiStateManager::connected:
            lastConnected = activeAP;
        case WifiStateManager::enabled:
        case WifiStateManager::disconnecting:
            if (!activeAP.isNull())
            {
                updateConnectionPoint(activeAP);
            }
            else if (!lastConnected.isNull())
            {
                updateConnectionPoint(lastConnected);
            }
            else
            {
                layoutConnectionPage();
            }
            break;
        default:
            layoutConnectionPage();
    }
}

/*
 * Adds all newly detected access points to the access point list.
 */
void WifiSettingsPage::accessPointAdded(const WifiAccessPoint& addedAP)
{
    addConnectionPoint(addedAP);
}

/*
 * Removes all missing access points from the access point list.
 */
void WifiSettingsPage::accessPointRemoved(const WifiAccessPoint& removedAP)
{
    removeConnectionPoint(removedAP);
}

/*
 * Attempts to connect if return is pressed after entering a password.
 */
void WifiSettingsPage::textEditorReturnKeyPressed(TextEditor& editor)
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
String WifiSettingsPage::getWifiAssetName(const WifiAccessPoint& accessPoint)
{
    float sigStrength = accessPoint.getSignalStrength();
    int maxIndex = wifiImageFiles.size() - 1;
    return wifiImageFiles[round((maxIndex * (sigStrength) / 100.0f))];
}

WifiSettingsPage::WifiAPButton::WifiAPButton(
        const WifiAccessPoint& accessPoint,
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
