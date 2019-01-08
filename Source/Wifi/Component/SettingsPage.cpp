#define WIFI_IMPLEMENTATION
#include "Wifi/Component/SettingsPage.h"
#include "Wifi/Connection/RecordReader.h"
#include "Wifi/Connection/Event.h"
#include "Wifi/Connection/Controller.h"
#include "Wifi_APList_Reader.h"
#include "Layout_Component_ConfigFile.h"
#include "LibNM/Data/SecurityType.h"
#include "Locale/Time.h"

/* Wifi signal strength icon asset files: */
// TODO: load these assets from a JSON file!
const juce::StringArray Wifi::SettingsPage::wifiImageFiles = 
{
    "component assets/WifiIcon/wifiStrength0.svg",
    "component assets/WifiIcon/wifiStrength1.svg",
    "component assets/WifiIcon/wifiStrength2.svg",
    "component assets/WifiIcon/wifiStrength3.svg"
};

/* Access point lock icon file: */
// TODO: load this from a JSON file!
static const constexpr char* lockFile = "component assets/WifiIcon/lock.svg";

//===================== List item Layout values ================================
/* Horizontal weights for components in each access point list item: */
static const constexpr unsigned int labelWeight = 8;
static const constexpr unsigned int iconWeight = 1;
/* Vertical weight values: */
static const constexpr unsigned int baseRowWeight = 3;
static const constexpr unsigned int errorMsgRowWeight = 2;
/* Margin and Padding fractions: */
static const constexpr float xMarginFraction = 0.03;
static const constexpr float yMarginFraction = 0.1;
static const constexpr float xPaddingFraction = 0.04;
static const constexpr float yPaddingFraction = 0.04;

/* Localized object class key: */
static const juce::Identifier localeClassKey = "WifiSettingsPage";

/* Localized text value keys: */
static const juce::Identifier passwordTextKey         = "password";
static const juce::Identifier connectTextKey          = "connect";
static const juce::Identifier disconnectTextKey       = "disconnect";
static const juce::Identifier wrongPasswordTextKey    = "wrongPassword";
static const juce::Identifier invalidFormatTextKey    = "invalidFormat";
static const juce::Identifier connectionFailedTextKey = "connectionFailed";
static const juce::Identifier lostAPTextKey           = "lostAP";
static const juce::Identifier lastConnectedTextKey    = "lastConnected";

/* Anonymous comparator class object for Wifi access point array sorting. */
static class
{
public:
    /**
     * @brief  Compares wifi access points in order to sort the access point 
     *         list.
     *
     *  The connected access point will always come before all others, and
     * saved access points will come before new ones. Otherwise, access points
     * are sorted by signal strength.
     * 
     * @param first    Some access point in the list.
     * 
     * @param second   Some other access point in the list.
     * 
     * @return         A negative number if first should come before second, 
     *                 zero if the two connection points are equal, or a 
     *                 positive number if second should come before first.
     */
    static int compareElements(const Wifi::AccessPoint& first, 
            const Wifi::AccessPoint& second)
    {
        using namespace Wifi;
        jassert(!first.isNull() && !second.isNull());
        const Connection::RecordReader recordReader;
        AccessPoint activeAP = recordReader.getActiveAP();
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
        bool firstSaved = recordReader.hasSavedConnection(first);
        bool secondSaved = recordReader.hasSavedConnection(second);
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
} apComparator;

Wifi::SettingsPage::SettingsPage() :
passwordLabel("passwordLabel", localeText(passwordTextKey)),
Locale::TextUser(localeClassKey)
{
#    if JUCE_DEBUG
    setName("WifiSettingsPage");
#    endif
    passwordEditor.addListener(this);
    Layout::Component::ConfigFile config;
    passwordEditor.setFont(juce::Font(config.getFontHeight(
                    Layout::Component::TextSize::smallText)));
    
    connectionButton.addListener(this);
    errorLabel.setJustificationType(juce::Justification::centred);
    loadAccessPoints();
}

/*
 * Sets the number of items in the list to match the number of visible
 * Wifi access points. 
 */
unsigned int Wifi::SettingsPage::getListSize()
{
    return visibleAPs.size();
}

/**
 * @brief  Given a list of recycled components, return the first one on the list 
 *         that is currently unused. If no list items are unused, a new item 
 *         will be created, added to the list, and returned.
 * 
 * @tparam ComponentType  Any subclass of juce::Component.
 * 
 * @param   recycleList  A list of Component objects to re-use.
 * 
 * @return               The first list item found with no parent component, or 
 *                       a new list item if all old list items are in use.
 */
template<class ComponentType>
static ComponentType * getFirstUnused
(juce::OwnedArray<ComponentType>& recycleList)
{
    for (ComponentType * comp : recycleList)
    {
        if (comp->getParentComponent() == nullptr)
        {
            return comp;
        }
    }
    ComponentType * comp = new ComponentType();
    recycleList.add(comp);
    return comp;
}

/*
 * Creates or updates the layout of one access point on the list.
 */
void Wifi::SettingsPage::updateListItemLayout
(Layout::Group::RelativeLayout& layout, const unsigned int index)
{
    using namespace Layout::Group;
    jassert(index < visibleAPs.size());
    const AccessPoint& wifiAP = visibleAPs[index];
    ScalingLabel* apLabel = nullptr;
    DrawableImageComponent * apIcon = nullptr;
    DrawableImageComponent * lockIcon = nullptr;
    if (!layout.isEmpty())
    {
        Row labelRow = layout.getRow(0);
        jassert(labelRow.itemCount() == 3);
        apLabel = dynamic_cast<ScalingLabel*>
                (labelRow.getRowItem(0).getComponent());
        lockIcon = dynamic_cast<DrawableImageComponent*>
                (labelRow.getRowItem(1).getComponent());
        apIcon = dynamic_cast<DrawableImageComponent*>
                (labelRow.getRowItem(2).getComponent());
    }
    if(apLabel == nullptr)
    {
        apLabel = getFirstUnused(apLabels);
    }
    if(apIcon == nullptr)
    {
        apIcon = getFirstUnused(apIcons);
    }

    if (wifiAP.getSecurityType() != LibNM::SecurityType::unsecured)
    {
        if (lockIcon == nullptr)
        {
            lockIcon = getFirstUnused(lockIcons);
        }
        if(lockIcon->isEmpty())
        {
            lockIcon->setImage(lockFile);
            lockIcon->setColour(DrawableImageComponent::imageColour0Id,
                    findColour(juce::Label::ColourIds::textColourId));
        }
    }
    else
    {
        lockIcon = nullptr;
    }

    layout = RelativeLayout(
    {
        Row(baseRowWeight,
        {
            RowItem(apLabel, labelWeight),
            RowItem(lockIcon, iconWeight),
            RowItem(apIcon, iconWeight)
        })
    });
    layout.setXMarginFraction(xMarginFraction);
    layout.setYMarginFraction(yMarginFraction);
    layout.setXPaddingFraction(xPaddingFraction);

    jassert(apLabel != nullptr);
    jassert(apIcon != nullptr);
    jassert(lockIcon == nullptr || wifiAP.getSecurityType() 
            == LibNM::SecurityType::unsecured);
    jassert(!visibleAPs[index].isNull());
    jassert(visibleAPs[index].getSSID().toString().isNotEmpty());

    apLabel->setText(visibleAPs[index].getSSID().toString(),
            juce::NotificationType::dontSendNotification);
    apLabel->setJustificationType(juce::Justification::centred);
    apLabel->setInterceptsMouseClicks(false, false);
    apIcon->setImage(getWifiAssetName(visibleAPs[index]));
    apIcon->setColour(DrawableImageComponent::imageColour0Id,
                    findColour(juce::Label::ColourIds::textColourId));
}

/*
 * Creates or updates the access point control/information panel that
 * appears when an access point in the list is selected.
 */
void Wifi::SettingsPage::updateSelectedItemLayout
(Layout::Group::RelativeLayout& layout)
{
    using juce::String;
    jassert(getSelectedIndex() >= 0 && !layout.isEmpty());
    const AccessPoint& selectedAP = visibleAPs[getSelectedIndex()];

    using namespace Layout::Group;
    if (layout.rowCount() == 1)
    {
        layout.addRow(Row(baseRowWeight,{
            RowItem(),
            RowItem(&lastConnectionLabel, 4),
            RowItem()
        }));
        layout.addRow(Row(baseRowWeight));
        layout.addRow(Row(errorMsgRowWeight,{RowItem(&errorLabel)}));
        layout.addRow(Row(baseRowWeight,{
            RowItem(),
            RowItem(&connectionButton),
            RowItem()
        }));
        
        layout.setXPaddingFraction(xPaddingFraction);
        layout.setYPaddingFraction(yPaddingFraction);
        layout.setXMarginFraction(xMarginFraction);
        layout.setYMarginFraction(yMarginFraction);
    }
    String connectionBtnText = localeText(connectTextKey);
    bool showButtonSpinner = false;
    bool showPasswordEntry = false;
    String errorMessage = "";

    const Connection::RecordReader connectionRecord;
    if(connectionRecord.hasSavedConnection(selectedAP))
    {
        Locale::Time connTime(connectionRecord.lastConnectionTime(selectedAP));
        lastConnectionLabel.setText(localeText(lastConnectedTextKey)
                + connTime.approxTimePassed(),
                juce::NotificationType::dontSendNotification);
    }
    else
    {
        lastConnectionLabel.setText(String(),
                juce::NotificationType::dontSendNotification);
    }

    DBG("Wifi::SettingsPage::" << __func__ 
            << ": Updating connection controls for AP " 
            << selectedAP.getSSID().toString());
    const bool requiresAuth = selectedAP.getSecurityType() 
            != LibNM::SecurityType::unsecured;
    const bool hasSavedConnection 
            = connectionRecord.hasSavedConnection(selectedAP);
    const bool isActiveAP
            = connectionRecord.getActiveAP() == selectedAP;

    if(isActiveAP && connectionRecord.isConnecting())
    {
        connectionBtnText = "";
        showButtonSpinner = true;
    }
    else if(isActiveAP && connectionRecord.isConnected())
    {
        connectionBtnText = localeText(disconnectTextKey);
    }

    else
    {
        const Connection::EventType lastEventType 
                 = connectionRecord.getLastEvent(selectedAP).getEventType();
        showPasswordEntry = requiresAuth && !hasSavedConnection;
        if(lastEventType == Connection::EventType::connectionFailed)
        {
            errorMessage = localeText(connectionFailedTextKey);
        }
        else if(lastEventType == Connection::EventType::connectionAuthFailed)
        {
            jassert(requiresAuth);
            showPasswordEntry = true;
            errorMessage = localeText(wrongPasswordTextKey);
        }
    }
    if (showPasswordEntry)
    {
        passwordEditor.clear();
        layout.setRow(2 , Row(baseRowWeight,{
            RowItem(&passwordLabel, 4),
            RowItem(&passwordEditor, 5)
        }));
    }
    else
    {
        layout.setRow(2 , Row(baseRowWeight));
    }
    connectionButton.setButtonText(connectionBtnText);
    connectionButton.setSpinnerVisible(showButtonSpinner);
    errorLabel.setText(errorMessage, 
            juce::NotificationType::dontSendNotification);
}

/*
 * Reloads the list of wifi access points within range of the wifi device,
 * and updates the access point list.
 */
void Wifi::SettingsPage::loadAccessPoints()
{
    visibleAPs.clearQuick();
    const APList::Reader apListReader;
    visibleAPs = apListReader.getAccessPoints();
    visibleAPs.sort(apComparator, false);
    updateAPList();
}

/*
 * Removes any lost access points, sorts the access point list, and
 * refreshes the access point list component.
 */
void Wifi::SettingsPage::updateAPList()
{
    for (int i = 0; i < visibleAPs.size(); i++)
    {
        if (visibleAPs[i].isNull() 
                || visibleAPs[i].getSSID().toString().isEmpty())
        {
            visibleAPs.remove(i);
            i--;
        }
    }
    visibleAPs.sort(apComparator, false);
    updateList();
}

/*
 * Attempts to connect to a Wifi access point.  This will close any
 * connections to other access points.
 */
void Wifi::SettingsPage::connect(const AccessPoint accessPoint)
{
    using juce::String;
    if (accessPoint.isNull())
    {
        DBG("Wifi::SettingsPage::" << __func__ << ": ap is null!");
        return;
    }
    const String& psk = passwordEditor.getText();
    if (passwordEditor.isShowing() && !accessPoint.isValidKeyFormat(psk))
    {
        errorLabel.setText(localeText(invalidFormatTextKey),
                juce::NotificationType::dontSendNotification);
        passwordEditor.clear();
        return;
    }

    Connection::Controller connectionControl;
    if (accessPoint.getSecurityType() != LibNM::SecurityType::unsecured)
    {
        DBG("Wifi::SettingsPage::" << __func__ << ": connecting to "
                << accessPoint.getSSID().toString() << " with psk of length "
                << psk.length());
        connectionControl.connectToAccessPoint(accessPoint, psk);
    }
    else
    {
        DBG("Wifi::SettingsPage::" << __func__ << ": connecting to "
                << accessPoint.getSSID().toString() 
                << " with no psk required.");
        connectionControl.connectToAccessPoint(accessPoint);
    }
    passwordEditor.clear();
    errorLabel.setText(String(), 
            juce::NotificationType::dontSendNotification);
}

/*
 * Tries to disconnect from a specific wifi access point.
 */
void Wifi::SettingsPage::disconnect(const AccessPoint accessPoint)
{

    const Connection::RecordReader connectionRecords;
    if (accessPoint == connectionRecords.getActiveAP())
    {
        Connection::Controller connectionControl;
        connectionControl.disconnect();
    }
    else
    {
        DBG("Wifi::SettingsPage::" << __func__
                << ": ap is not connected/connecting!");
    }
}

/*
 * Attempts to connect or disconnect from the current selected access point
 * when the connection button is clicked.
 */
void Wifi::SettingsPage::listPageButtonClicked(juce::Button* button)
{
    // Only the connection button should trigger this method!
    jassert(&connectionButton == button);
    const AccessPoint& selectedAP = visibleAPs[getSelectedIndex()];
    const Connection::RecordReader connectionRecords;
    if(connectionRecords.getActiveAP() == selectedAP)
    {
        if(connectionRecords.isConnecting())
        {
            DBG("Wifi::SettingsPage::" << __func__ 
                    << ": Currently connecting, the connection button "
                    << "should have been disabled or hidden!");
            return;
        }
        DBG("Wifi::SettingsPage::" << __func__ << ": Disconnecting from "
                << selectedAP.getSSID().toString());
        disconnect(selectedAP);
        return;
    }
    else
    {
        DBG("Wifi::SettingsPage::" << __func__ << ": Connecting to "
                << selectedAP.getSSID().toString());
        connect(selectedAP);
    }
}
    
/*
 * Updates the list when access point signal strength changes.
 */
void Wifi::SettingsPage::signalStrengthUpdate(const AccessPoint updatedAP) 
{
    updateAPList();
}

/*
 * Adds a newly discovered access point to the list.
 */
void Wifi::SettingsPage::accessPointAdded(const AccessPoint addedAP)
{
    visibleAPs.add(addedAP);
    updateAPList();
}

/*
 * Removes an access point from the list when it is no longer visible.
 */
void Wifi::SettingsPage::accessPointRemoved(const AccessPoint removedAP)
{
    visibleAPs.removeAllInstancesOf(removedAP);
    updateAPList();
}

/*
 * Updates access point connection controls when a connection starts to 
 * activate.
 */
void Wifi::SettingsPage::startedConnecting(const AccessPoint connectingAP)
{
    updateAPList();
}

/*
 * Updates access point connection controls and prints an error message when 
 * connection authentication fails.
 */
void Wifi::SettingsPage::connectionAuthFailed(const AccessPoint connectingAP)
{
    updateAPList();
}

/*
 * Updates the access point list and connection controls when an access point 
 * connects.
 */
void Wifi::SettingsPage::connected(const AccessPoint connectedAP)
{
    updateAPList();
}

/*
 * Updates the access point list and connection controls when an access point 
 * disconnects.
 */
void Wifi::SettingsPage::disconnected(const AccessPoint disconnectedAP)
{
    updateAPList();
}

/*
 * Attempts to connect if return is pressed after entering a password.
 */
void Wifi::SettingsPage::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    connect(visibleAPs[getSelectedIndex()]);
}

/*
 * Gets the asset name for the icon that best represents accessPoint's 
 * signal strength.
 */
juce::String Wifi::SettingsPage::getWifiAssetName(const AccessPoint accessPoint)
{
    float sigStrength = accessPoint.getSignalStrength();
    int maxIndex = wifiImageFiles.size() - 1;
    return wifiImageFiles[round((maxIndex * (sigStrength) / 100.0f))];
}

Wifi::SettingsPage::ConnectionButton::ConnectionButton()
{
    addChildComponent(spinner);
}

/*
 * Show or hide the spinner, ensuring the button is enabled when
 * text is visible and disabled when the spinner is visible.
 */
void Wifi::SettingsPage::ConnectionButton::setSpinnerVisible(bool showSpinner)
{
    spinner.setVisible(showSpinner);
    setEnabled(!showSpinner);
    if(showSpinner)
    {
        savedText = getButtonText();
        setButtonText(juce::String());
    }
    else if(getButtonText().isEmpty())
    {
        setButtonText(savedText);
    }
}

/*
 * Ensures spinner bounds are updated with connection button bounds.
 */
void Wifi::SettingsPage::ConnectionButton::resized()
{
    int spinnerSize = getHeight();
    spinner.setBounds(getLocalBounds().
            withSizeKeepingCentre(spinnerSize, spinnerSize));
}
