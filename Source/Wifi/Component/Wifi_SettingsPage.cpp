#define WIFI_IMPLEMENTATION
#include "Wifi_SettingsPage.h"
#include "Wifi_Connection_Record_Reader.h"
#include "Wifi_Connection_Saved_Reader.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_APList_Reader.h"
#include "Layout_Component_ConfigFile.h"
#include "Wifi_LibNM_SecurityType.h"
#include "Locale/Time.h"

#ifdef JUCE_DEBUG
/* Print the full class name before debug output: */
static const constexpr char* dbgPrefix = "Wifi::SettingsPage::";
#endif

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
static const juce::Identifier localeClassKey = "Wifi_SettingsPage";

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
        const Connection::Record::Reader recordReader;
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
        bool firstSaved = first.hasSavedConnection();
        bool secondSaved = second.hasSavedConnection();
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
    listUpdatePending.set(false);
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
    jassert(lockIcon != nullptr || wifiAP.getSecurityType() 
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
    String lastConnectionText;
    bool showButtonSpinner = false;
    bool showPasswordEntry = false;
    String errorMessage = "";

    const Connection::Record::Reader connectionRecord;

    DBG(dbgPrefix << __func__ << ": Updating connection controls for AP " 
            << selectedAP.getSSID().toString());
    const bool requiresAuth = selectedAP.getSecurityType() 
            != LibNM::SecurityType::unsecured;
    const bool hasSavedConnection = selectedAP.hasSavedConnection();
    const bool isActiveAP = connectionRecord.getActiveAP() == selectedAP;


    juce::int64 lastConnectionTime = 0;
    if(isActiveAP && connectionRecord.isConnecting())
    {
        connectionBtnText = "";
        showButtonSpinner = true;
    }
    else if(isActiveAP && connectionRecord.isConnected())
    {
        connectionBtnText = localeText(disconnectTextKey);
        lastConnectionTime = juce::Time::currentTimeMillis();
    }
    else if(hasSavedConnection)
    {
        lastConnectionTime = selectedAP.getLastConnectionTime();
    }
    else
    {
        const Connection::EventType lastEventType 
                 = connectionRecord.getLatestEvent(selectedAP).getEventType();
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
    if(lastConnectionTime > 0)
    {
        Locale::Time localizedTime((juce::Time(lastConnectionTime)));

        lastConnectionText = localeText(lastConnectedTextKey)
                + localizedTime.approxTimePassed();
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
    lastConnectionLabel.setText(lastConnectionText,
            juce::NotificationType::dontSendNotification);
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
    /* Check for pending updates and schedule an update if none is pending, all
       as one atomic action. */
    if(listUpdatePending.compareAndSetBool(true, false))
    {
        return;
    }
    juce::Component::SafePointer<SettingsPage> safePagePtr(this);
    juce::MessageManager::callAsync([safePagePtr]()
    {
        SettingsPage* settingsPage = safePagePtr.getComponent();
        if(settingsPage != nullptr)
        {
            for (int i = 0; i < settingsPage->visibleAPs.size(); i++)
            {
                if (settingsPage->visibleAPs[i].isNull() || settingsPage
                        ->visibleAPs[i].getSSID().toString().isEmpty())
                {
                    settingsPage->visibleAPs.remove(i);
                    i--;
                }
            }
            if(settingsPage->getSelectedIndex() == -1)
            {
                settingsPage->visibleAPs.sort(apComparator, false);
            }
            settingsPage->updateList();
            settingsPage->listUpdatePending.set(false);
        }
    });
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
        DBG(dbgPrefix << __func__ << ": ap is null!");
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

    if (accessPoint.getSecurityType() != LibNM::SecurityType::unsecured)
    {
        DBG(dbgPrefix << __func__ << ": connecting to "
                << accessPoint.getSSID().toString() << " with psk of length "
                << psk.length());
        connectionControl.connectToAccessPoint(accessPoint, psk);
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": connecting to "
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

    const Connection::Record::Reader connectionRecords;
    if (accessPoint == connectionRecords.getActiveAP())
    {
        connectionControl.disconnect();
    }
    else
    {
        DBG(dbgPrefix << __func__
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
    const Connection::Record::Reader connectionRecords;
    if(connectionRecords.getActiveAP() == selectedAP)
    {
        if(connectionRecords.isConnecting())
        {
            DBG(dbgPrefix << __func__ 
                    << ": Currently connecting, the connection button "
                    << "should have been disabled or hidden!");
            return;
        }
        DBG(dbgPrefix << __func__ << ": Disconnecting from "
                << selectedAP.getSSID().toString());
        disconnect(selectedAP);
        return;
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Connecting to "
                << selectedAP.getSSID().toString());
        connect(selectedAP);
    }
}
    
/*
 * Updates the list when access point signal strength changes.
 */
void Wifi::SettingsPage::signalStrengthUpdate(const AccessPoint updatedAP) 
{
    juce::MessageManager::callAsync([this]()
    {
        updateAPList();
    });
}

/*
 * Adds a newly discovered access point to the list.
 */
void Wifi::SettingsPage::accessPointAdded(const AccessPoint addedAP)
{
    juce::MessageManager::callAsync([this, addedAP]()
    {
        visibleAPs.addIfNotAlreadyThere(addedAP);
        updateAPList();
    });
}

/*
 * Removes an access point from the list when it is no longer visible.
 */
void Wifi::SettingsPage::accessPointRemoved(const AccessPoint removedAP)
{
    juce::MessageManager::callAsync([this, removedAP]()
    {
        visibleAPs.removeAllInstancesOf(removedAP);
        updateAPList();
    });
}

/*
 * Updates access point connection controls when a connection starts to 
 * activate.
 */
void Wifi::SettingsPage::startedConnecting(const AccessPoint connectingAP)
{
    juce::MessageManager::callAsync([this]()
    {
        updateAPList();
    });
}

/*
 * Updates access point connection controls and prints an error message when 
 * connection authentication fails.
 */
void Wifi::SettingsPage::connectionAuthFailed(const AccessPoint connectingAP)
{
    juce::MessageManager::callAsync([this]()
    {
        updateAPList();
    });
}

/*
 * Updates the access point list and connection controls when an access point 
 * connects.
 */
void Wifi::SettingsPage::connected(const AccessPoint connectedAP)
{
    juce::MessageManager::callAsync([this]()
    {
        updateAPList();
    });
}

/*
 * Updates the access point list and connection controls when an access point 
 * disconnects.
 */
void Wifi::SettingsPage::disconnected(const AccessPoint disconnectedAP)
{
    juce::MessageManager::callAsync([this]()
    {
        updateAPList();
    });
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
