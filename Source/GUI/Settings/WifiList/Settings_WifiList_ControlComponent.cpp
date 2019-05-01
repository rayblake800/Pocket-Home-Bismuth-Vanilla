#define WIFI_IMPLEMENTATION
#include "Settings_WifiList_ControlComponent.h"
#include "Wifi_Connection_Record_Handler.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_Connection_Control_Handler.h"
#include "Wifi_Connection_Saved_Deleter.h"
#include "Layout_Component_ConfigFile.h"
#include "Locale_Time.h"
#include "Windows_Alert.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix
        = "Settings::WifiList::ControlComponent::";
#endif

// Localized object class key:
static const juce::Identifier localeClassKey = "Settings::WifiList";

// Localized text value keys:
namespace TextKey
{
    static const juce::Identifier showPassword     = "showPassword";
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

static const constexpr juce::juce_wchar passwordChar = 0x2022;

//===================== List item Layout values ===============================
// Vertical weight values:
static const constexpr unsigned int baseRowWeight = 4;
static const constexpr unsigned int showPasswordRowWeight = 3;
static const constexpr unsigned int labelRowWeight = 3;
// If necessary, empty rows will be added to make sure the vertical weight sum
// reaches this value:
static const constexpr unsigned int rowWeightSum = 21;
// Margin and Padding fractions:
static const constexpr float xMarginFraction = 0.03;
static const constexpr float yMarginFraction = 0.05;
static const constexpr float xPaddingFraction = 0.04;
static const constexpr float yPaddingFraction = 0.02;

// Starts listening for input from child components.
Settings::WifiList::ControlComponent::ControlComponent() :
    Locale::TextUser(localeClassKey),
    controlListener(*this)
{
    savedConnectionDeleteButton.setButtonText(
            localeText(TextKey::deleteSaved));
    passwordLabel.setText(localeText(TextKey::password),
            juce::NotificationType::dontSendNotification);
    showPasswordLabel.setText(localeText(TextKey::showPassword),
            juce::NotificationType::dontSendNotification);
    passwordEditor.setPasswordCharacter(passwordChar);
    errorLabel.setJustificationType(juce::Justification::centred);

    savedConnectionDeleteButton.addListener(&controlListener);
    connectionButton.addListener(&controlListener);
    passwordEditor.addListener(&controlListener);
    showPasswordSwitch.addListener(&controlListener);
}


// Updates all components to represent a specific Wifi access point.
void Settings::WifiList::ControlComponent::updateComponentsForAP
(Wifi::AccessPoint newAP)
{
    using juce::String;
    using Wifi::Connection::EventType;

    const Wifi::Connection::Record::Handler connectionRecord;
    DBG(dbgPrefix << __func__ << ": Updating connection controls for AP "
            << newAP.getSSID().toString());
    const bool requiresAuth = newAP.getSecurityType()
            != Wifi::LibNM::SecurityType::unsecured;
    const bool hasSavedConnection = newAP.hasSavedConnection();
    const bool isActiveAP = connectionRecord.getActiveAP() == newAP;
    const bool isConnected = isActiveAP && connectionRecord.isConnected();
    const bool isConnecting = isActiveAP && connectionRecord.isConnecting();
    const EventType lastEventType
            = connectionRecord.getLatestEvent(newAP).getEventType();

    using namespace Layout::Group;
    RelativeLayout updatedLayout;

    // Update last connection time label:
    juce::int64 lastConnectionTime = 0;
    if (isConnected)
    {
        lastConnectionTime = juce::Time::currentTimeMillis();
    }
    else if (hasSavedConnection)
    {
        lastConnectionTime = newAP.getLastConnectionTime();
    }
    if (lastConnectionTime > 0)
    {
        Locale::Time localizedTime( (juce::Time(lastConnectionTime)));

        String lastConnectionText(localeText(TextKey::lastConnected)
                + localizedTime.approxTimePassed());
        updatedLayout.addRow(
                Row(labelRowWeight,
                {
                    RowItem(),
                    RowItem(&lastConnectionLabel, 4),
                    RowItem()
                }));
        lastConnectionLabel.setText(lastConnectionText,
                juce::NotificationType::dontSendNotification);
    }
    else if (lastConnectionLabel.getParentComponent() == this)
    {
        removeChildComponent(&lastConnectionLabel);
    }

    // Update saved connection delete button:
    if (! isConnected && ! isConnecting && hasSavedConnection)
    {
        updatedLayout.addRow(
                Row(baseRowWeight,
                {
                    RowItem(),
                    RowItem(&savedConnectionDeleteButton, 4),
                    RowItem()
                }));
    }
    else if (savedConnectionDeleteButton.getParentComponent() == this)
    {
        removeChildComponent(&savedConnectionDeleteButton);
    }

    // Save the index where an empty row may need to be inserted to keep layout
    // rows correctly sized:
    const int emptyRowIndex = updatedLayout.rowCount();

    // Update password label, entry field, and password visibility switch:
    if (! isConnected && ! isConnecting && requiresAuth && (! hasSavedConnection 
                || lastEventType == EventType::connectionAuthFailed))
    {
        updatedLayout.addRow(
                Row(baseRowWeight,
                {
                    RowItem(&passwordLabel, 1),
                    RowItem(&passwordEditor, 2)
                }));
        updatedLayout.addRow(
                Row(showPasswordRowWeight,
                {
                    RowItem(&showPasswordLabel, 8),
                    RowItem(&showPasswordSwitch, 1)
                }));
        if (selectedAP != newAP)
        {
            passwordEditor.clear();
            passwordEditor.setPasswordCharacter(passwordChar);
            showPasswordSwitch.setToggleState(false);
        }
    }
    else if (passwordEditor.getParentComponent() == this)
    {
        removeChildComponent(&passwordLabel);
        removeChildComponent(&passwordEditor);
        removeChildComponent(&showPasswordLabel);
        removeChildComponent(&showPasswordSwitch);
    }

    // Update connection button:
    connectionButton.setSpinnerVisible(isConnecting);
    String connectionBtnText;
    if (isConnected)
    {
        connectionBtnText = localeText(TextKey::disconnect);
    }
    else if (!isConnecting)
    {
        connectionBtnText = localeText(TextKey::connect);
    }
    connectionButton.setButtonText(connectionBtnText);
    updatedLayout.addRow(
            Row(baseRowWeight,
            {
                RowItem(),
                RowItem(&connectionButton, 4),
                RowItem()
            }));

    // Update error label:
    String errorText;
    if (selectedAP == newAP)
    {
        errorText = errorLabel.getText();
    }
    if (! isConnected && ! isConnecting)
    {
        if (lastEventType == Wifi::Connection::EventType::connectionFailed)
        {
            errorText = localeText(TextKey::connectionFailed);
        }
        else if (lastEventType
                == Wifi::Connection::EventType::connectionAuthFailed)
        {
            errorText = localeText(TextKey::wrongPassword);
        }
    }
    else
    {
        errorText = "";
    }
    errorLabel.setText(errorText, juce::NotificationType::dontSendNotification);
    if (errorText.isNotEmpty())
    {
        updatedLayout.addRow(Row(labelRowWeight, { RowItem(&errorLabel) }));
    }
    else if (errorLabel.getParentComponent() == this)
    {
        removeChildComponent(&errorLabel);
    }

    // Add an empty row if necessary:
    int currentWeightSum = 0;
    for (const Row& layoutRow : updatedLayout)
    {
        currentWeightSum += layoutRow.getWeight();
    }
    const int weightNeeded = rowWeightSum - currentWeightSum;
    if (weightNeeded > 0)
    {
        updatedLayout.insertRow(emptyRowIndex, Row(weightNeeded));
    }

    updatedLayout.setXPaddingFraction(xPaddingFraction);
    updatedLayout.setYPaddingFraction(yPaddingFraction);
    updatedLayout.setXMarginFraction(xMarginFraction);
    updatedLayout.setYMarginFraction(yMarginFraction);
    layoutControl.setLayout(updatedLayout, this);
    layoutControl.layoutComponents(getLocalBounds());

    selectedAP = newAP;
}


// Updates the layout of all child components when the main component is
// resized.
void Settings::WifiList::ControlComponent::resized()
{
    layoutControl.layoutComponents(getLocalBounds());
}


// Attempts to open a connection using the selected access point.
void Settings::WifiList::ControlComponent::connect()
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
        updateComponentsForAP(selectedAP);
        return;
    }

    if (selectedAP.getSecurityType() != Wifi::LibNM::SecurityType::unsecured)
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

using Control = Settings::WifiList::ControlComponent;

Control::ConnectionButton::ConnectionButton()
{
    addChildComponent(spinner);
}


// Shows or hide the spinner, ensuring the button is enabled when text is
// visible and disabled when the spinner is visible.
void Control::ConnectionButton::setSpinnerVisible(bool showSpinner)
{
    spinner.setVisible(showSpinner);
    setEnabled(!showSpinner);
    if (showSpinner)
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


// Ensures the spinner bounds are updated with connection button bounds.
void Control::ConnectionButton::resized()
{
    int spinnerSize = getHeight();
    spinner.setBounds(getLocalBounds().withSizeKeepingCentre(spinnerSize,
                spinnerSize));
}


// Connects this listener to its ControlComponent.
Control::ControlListener::ControlListener
(ControlComponent& controlComponent) : controlComponent(controlComponent) { }


// Either deletes a saved connection, attempts to open or close a connection, or
// toggles password visibility, depending on which button is clicked.
void Control::ControlListener::buttonClicked(juce::Button* button)
{
    Wifi::AccessPoint selectedAP = controlComponent.selectedAP;
    if (selectedAP.isNull())
    {
        DBG(dbgPrefix << __func__ << ": ap is null!");
        return;
    }
    if (button == &controlComponent.connectionButton)
    {
        const Wifi::Connection::Record::Handler connectionRecords;
        if (connectionRecords.getActiveAP() == selectedAP)
        {
            if (connectionRecords.isConnecting())
            {
                DBG(dbgPrefix << __func__
                        << ": Currently connecting, the connection button "
                        << "should have been disabled or hidden!");
                return;
            }
            else if (connectionRecords.isConnected())
            {
                DBG(dbgPrefix << __func__ << ": Disconnecting from "
                        << selectedAP.getSSID().toString());
                controlComponent.connectionController.disconnect();
                return;
            }
        }
        else
        {
            DBG(dbgPrefix << __func__ << ": Connecting to "
                    << selectedAP.getSSID().toString());
            controlComponent.connect();
        }
    }
    else if (button == &controlComponent.savedConnectionDeleteButton)
    {
        DBG(dbgPrefix << __func__
                << ": Getting confirmation before deleting saved connection:");
        Windows::Alert::confirmAction(
                controlComponent.localeText(TextKey::deleteSaved),
                controlComponent.localeText(TextKey::confirmDelete),
                [this, selectedAP]()
        {
            Wifi::Connection::Saved::Deleter connectionDeleter;
            connectionDeleter.removeSavedConnection(selectedAP);
            controlComponent.updateComponentsForAP(selectedAP);
        });
    }
    else if (button == &controlComponent.showPasswordSwitch)
    {
        controlComponent.passwordEditor.setPasswordCharacter(
                (button->getToggleState() ? 0 : passwordChar));
    }
}


// Attempts to connect if return is pressed after entering a password.
void Control::ControlListener::textEditorReturnKeyPressed
(juce::TextEditor& editor)
{
    controlComponent.connect();
}
