#define WIFI_IMPLEMENTATION
#include "Wifi_ConnectionList_ControlComponent.h"
#include "Wifi_Connection_Record_Reader.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_Connection_Control_Handler.h"
#include "Wifi_Connection_Saved_Deleter.h"
#include "Layout_Component_ConfigFile.h"
#include "Locale_Time.h"
#include "Utils.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix 
        = "Wifi::ConnectionList::ControlComponent::";
#endif

/* Localized object class key: */
static const juce::Identifier localeClassKey = "Wifi_ConnectionList";

/* Localized text value keys: */
namespace TextKey
{
    static const juce::Identifier password         = "password";
    static const juce::Identifier connect          = "connect";
    static const juce::Identifier disconnect       = "disconnect";
    static const juce::Identifier wrongPassword    = "wrongPassword";
    static const juce::Identifier invalidFormat    = "invalidFormat";
    static const juce::Identifier connectionFailed = "connectionFailed";
    static const juce::Identifier lostAP           = "lostAP";
    static const juce::Identifier lastConnected    = "lastConnected";
    static const juce::Identifier deleteSaved      = "deleteSaved";
    static const juce::Identifier confirmDelete    = "confirmDelete";
}

//===================== List item Layout values ================================
/* Vertical weight values: */
static const constexpr unsigned int baseRowWeight = 3;
static const constexpr unsigned int errorMsgRowWeight = 2;
/* Margin and Padding fractions: */
static const constexpr float xMarginFraction = 0.03;
static const constexpr float yMarginFraction = 0.05;
static const constexpr float xPaddingFraction = 0.04;
static const constexpr float yPaddingFraction = 0.02;

/*
 * Initializes all child components.
 */
Wifi::ConnectionList::ControlComponent::ControlComponent() :
    Locale::TextUser(localeClassKey),
    controlListener(*this)
{ 
    savedConnectionDeleteButton.setButtonText(localeText(TextKey::deleteSaved));
    passwordLabel.setText(localeText(TextKey::password),
            juce::NotificationType::dontSendNotification);

    savedConnectionDeleteButton.addListener(&controlListener);
    connectionButton.addListener(&controlListener);
    passwordEditor.addListener(&controlListener);

    using namespace Layout::Group;
    RelativeLayout newLayout(
    {
        Row(baseRowWeight,
        {
            RowItem(),
            RowItem(&lastConnectionLabel, 4),
            RowItem()
        }),
        Row(baseRowWeight,
        {
            RowItem(),
            RowItem(&savedConnectionDeleteButton, 4),
            RowItem()
        }),
        Row(baseRowWeight,
        {
            RowItem(&passwordLabel, 1),
            RowItem(&passwordEditor, 2)
        }),
        Row(errorMsgRowWeight,{RowItem(&errorLabel)}),
        Row(baseRowWeight,
        {
            RowItem(),
            RowItem(&connectionButton),
            RowItem()
        })
    });
    newLayout.setXPaddingFraction(xPaddingFraction);
    newLayout.setYPaddingFraction(yPaddingFraction);
    newLayout.setXMarginFraction(xMarginFraction);
    newLayout.setYMarginFraction(yMarginFraction);
    layoutControl.setLayout(newLayout, this);
}

/*
 * Updates all components to represent a specific Wifi access point.
 */
void Wifi::ConnectionList::ControlComponent::updateComponentsForAP
(AccessPoint newAP)
{

    using juce::String;
    using Connection::EventType;

    const Connection::Record::Reader connectionRecord;
    DBG(dbgPrefix << __func__ << ": Updating connection controls for AP " 
            << newAP.getSSID().toString());
    const bool requiresAuth = newAP.getSecurityType() 
            != LibNM::SecurityType::unsecured;
    const bool hasSavedConnection = newAP.hasSavedConnection();
    const bool isActiveAP = connectionRecord.getActiveAP() == newAP;
    const bool isConnected = isActiveAP && connectionRecord.isConnected();
    const bool isConnecting = isActiveAP && connectionRecord.isConnecting();
    const EventType lastEventType 
             = connectionRecord.getLatestEvent(newAP).getEventType();

    // Update last connection time label:
    String lastConnectionText;
    juce::int64 lastConnectionTime = 0;
    if(isConnected)
    {
        lastConnectionTime = juce::Time::currentTimeMillis();
    }
    else if(hasSavedConnection)
    {
        lastConnectionTime = newAP.getLastConnectionTime();
    }
    if(lastConnectionTime > 0)
    {
        Locale::Time localizedTime((juce::Time(lastConnectionTime)));

        lastConnectionText = localeText(TextKey::lastConnected)
                + localizedTime.approxTimePassed();
    }
    lastConnectionLabel.setText(lastConnectionText,
            juce::NotificationType::dontSendNotification);

    // Update saved connection delete button:
    const bool showDeleteButton = !isConnected && !isConnecting 
            && hasSavedConnection;
    savedConnectionDeleteButton.setVisible(showDeleteButton);
    savedConnectionDeleteButton.setEnabled(showDeleteButton);
    

    // Update password label and entry field:
    const bool showPasswordEntry = !isConnected && !isConnecting && requiresAuth
        && (!hasSavedConnection || lastEventType 
                == EventType::connectionAuthFailed);
    passwordLabel.setVisible(showPasswordEntry);
    passwordEditor.setVisible(showPasswordEntry);
    passwordEditor.setEnabled(showPasswordEntry);
    if(selectedAP != newAP)
    {
        passwordEditor.clear();
    }

    // Update error label:
    String errorText;
    if(selectedAP == newAP)
    {
        errorText = errorLabel.getText();
    }
    if(!isConnected && !isConnecting)
    {
        if(lastEventType == Connection::EventType::connectionFailed)
        {
            errorText = localeText(TextKey::connectionFailed);
        }
        else if(lastEventType == Connection::EventType::connectionAuthFailed)
        {
            errorText = localeText(TextKey::wrongPassword);
        }
    }
    errorLabel.setText(errorText, juce::NotificationType::dontSendNotification);

    // Update connection button:
    connectionButton.setSpinnerVisible(isConnecting);
    String connectionBtnText;
    if(isConnected)
    {
        connectionBtnText = localeText(TextKey::disconnect);
    }
    else if(!isConnecting)
    {
        connectionBtnText = localeText(TextKey::connect);
    }
    connectionButton.setButtonText(connectionBtnText);

    selectedAP = newAP;
}

/*
 * Updates the layout of all child components when the main component is 
 * resized.
 */
void Wifi::ConnectionList::ControlComponent::resized()
{
    layoutControl.layoutComponents(getLocalBounds());
}

/*
 * Attempts to open a connection using the selected access point.
 */
void Wifi::ConnectionList::ControlComponent::connect()
{
    using juce::String;
    if (selectedAP.isNull())
    {
        DBG(dbgPrefix << __func__ << ": ap is null!");
        return;
    }
    const String& psk = passwordEditor.getText();
    if (passwordEditor.isShowing() && !selectedAP.isValidKeyFormat(psk))
    {
        errorLabel.setText(localeText(TextKey::invalidFormat),
                juce::NotificationType::dontSendNotification);
        passwordEditor.clear();
        return;
    }

    if (selectedAP.getSecurityType() != LibNM::SecurityType::unsecured)
    {
        DBG(dbgPrefix << __func__ << ": connecting to "
                << selectedAP.getSSID().toString() << " with psk of length "
                << psk.length());
        connectionController.connectToAccessPoint(selectedAP, psk);
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": connecting to "
                << selectedAP.getSSID().toString() 
                << " with no psk required.");
        connectionController.connectToAccessPoint(selectedAP);
    }
    passwordEditor.clear();
    errorLabel.setText(String(), 
            juce::NotificationType::dontSendNotification);
}

using Control = Wifi::ConnectionList::ControlComponent;

Control::ConnectionButton::ConnectionButton() 
{ 
    addChildComponent(spinner);
}

/*
 * Shows or hide the spinner, ensuring the button is enabled when text is
 * visible and disabled when the spinner is visible.
 */
void Control::ConnectionButton::setSpinnerVisible(bool showSpinner)
{
    spinner.setVisible(showSpinner);
    setEnabled(!showSpinner);
    if(showSpinner)
    {
        savedText = getButtonText();
        setButtonText(juce::String());
    }
    else
    {
        setButtonText(savedText);
        savedText = juce::String();
    }
}

/*
 * Ensures the spinner bounds are updated with connection button bounds.
 */
void Control::ConnectionButton::resized()
{
    int spinnerSize = getHeight();
    spinner.setBounds(getLocalBounds().withSizeKeepingCentre(spinnerSize,
                spinnerSize));
}

/*
 * Connects this listener to its ControlComponent.
 */
Control::ControlListener::ControlListener
(ControlComponent& controlComponent) : controlComponent(controlComponent) { }

/*
 * Attempts to connect or disconnect from the current selected access point when
 * the connection button is clicked.
 */
void Control::ControlListener::buttonClicked(juce::Button* button)
{
    AccessPoint selectedAP = controlComponent.selectedAP;
    if(selectedAP.isNull())
    {
        DBG(dbgPrefix << __func__ << ": ap is null!");
        return;
    }
    if(button == &controlComponent.connectionButton)
    {
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
            else if(connectionRecords.isConnected())
            {
                DBG(dbgPrefix << __func__ << ": Disconnecting from "
                        << selectedAP.getSSID().toString());
                controlComponent.connectionController.disconnect();
                return;
            }
        }
        else
        DBG(dbgPrefix << __func__ << ": Connecting to "
                << selectedAP.getSSID().toString());
        controlComponent.connect();
    }
    else if(button == &controlComponent.savedConnectionDeleteButton)
    {
        DBG(dbgPrefix << __func__ 
                << ": Getting confirmation before deleting saved connection:");
        confirmAction(controlComponent.localeText(TextKey::deleteSaved),
                controlComponent.localeText(TextKey::confirmDelete),
                [this, selectedAP]()
        {
            Connection::Saved::Deleter connectionDeleter;
            connectionDeleter.removeSavedConnection(selectedAP);
            controlComponent.updateComponentsForAP(selectedAP);
        });
    }
}

/*
 * Attempts to connect if return is pressed after entering a password.
 */
void Control::ControlListener::textEditorReturnKeyPressed
(juce::TextEditor& editor)
{
    controlComponent.connect();
}
