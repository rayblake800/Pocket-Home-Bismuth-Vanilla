#pragma once
/**
 * @file  Wifi_ConnectionList_ControlComponent.h
 *
 * @brief  Provides reusable control and information components for a single
 *         access point.
 */

#include "Locale/TextUser.h"
#include "Wifi_AccessPoint.h"
#include "Layout_Group_Manager.h"
#include "Widgets_BoundedLabel.h"
#include "Spinner.h"
#include "Wifi_Connection_Control_Handler.h"
#include "JuceHeader.h"

namespace Wifi { namespace ConnectionList { class ControlComponent; } }

/**
 * @brief  A collection of components that provide detailed information and
 *         controls for a single Wifi access point.
 */
class Wifi::ConnectionList::ControlComponent : public juce::Component,
    public Locale::TextUser
{
public:
    /**
     * @brief  Initializes all child components.
     */
    ControlComponent();

    virtual ~ControlComponent() { }

    /**
     * @brief  Updates all components to represent a specific Wifi access point.
     *
     * @param newAP  The access point the ControlComponent should represent.
     */
    void updateComponentsForAP(AccessPoint newAP);

private:
    /**
     * @brief  Updates the layout of all child components when the main 
     *         component is resized.
     */
    virtual void resized() override;

    /**
     * @brief  Attempts to open a connection using the selected access point.
     */
    void connect();

    /**
     * @brief  A TextButton with text that can be replaced by a Spinner to 
     *         indicate that Wifi is busy.
     */
    class ConnectionButton : public juce::TextButton
    {
    public:
        ConnectionButton();
        
        virtual ~ConnectionButton() { }
      
        /**
         * @brief  Shows or hide the spinner, ensuring the button is enabled 
         *         when text is visible and disabled when the spinner is 
         *         visible.
         * 
         * @param showSpinner  True to show the spinner, false to show the
         *                     button text.
         */
        void setSpinnerVisible(bool showSpinner);

    private:
        /**
         * @brief  Ensures the spinner bounds are updated with connection button 
         *         bounds.
         */
        virtual void resized() override;
        
        /* The loading spinner component: */
        Spinner spinner;

        /* Holds the button text while the spinner is enabled. */
        juce::String savedText;
    };

    /**
     * @brief  Listens for input from the password field and connection button.
     */
    class ControlListener : public juce::Button::Listener, 
            public juce::TextEditor::Listener
    {
    public:
        /**
         * @brief  Connects this listener to its ControlComponent.
         *
         * @param controlComponent  The ControlComponent that owns this
         *                          ControlListener.
         */
        ControlListener(ControlComponent& controlComponent);

        virtual ~ControlListener() { }

    private:
        /**
         * @brief  Attempts to connect or disconnect from the current selected 
         *         access point when the connection button is clicked.
         * 
         * @param button  This should always be the connection button.
         */
        virtual void buttonClicked(juce::Button* button) final override;

        /**
         * @brief  Attempts to connect if return is pressed after entering a 
         *         password.
         * 
         * @param editor  This should always be the password field.
         */
        void textEditorReturnKeyPressed(juce::TextEditor& editor) override;

        ControlComponent& controlComponent;
    };
    ControlListener controlListener;

    /* Handles Wifi connection actions, and ensures the Wifi::Resource remains
     * available: */
    Wifi::Connection::Control::Handler connectionController;

    /* Manages the layout of all child components: */
    Layout::Group::Manager layoutControl;

    /* The access point object represented and controlled by this component: */
    AccessPoint selectedAP;

    /* Displays an access point's last connection time if applicable. */
    Widgets::BoundedLabel lastConnectionLabel;

    /* Allows the user to delete all saved connections linked to the access
     * point. */
    juce::TextButton savedConnectionDeleteButton;

    /* Used for entering a password for a secured access point. */
    Widgets::BoundedLabel passwordLabel;
    juce::TextEditor passwordEditor;

    /* Clicked to connect or disconnect. */
    ConnectionButton connectionButton;

    /* Shows an error message if the connection fails. */
    Widgets::BoundedLabel errorLabel;
};
