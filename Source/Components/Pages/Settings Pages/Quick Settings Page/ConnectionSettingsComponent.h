#pragma once

#include "JuceHeader.h"
#include "PageComponent.h"
#include "LayoutManager.h"
#include "SwitchComponent.h"
#include "Spinner.h"
#include "DrawableImageComponent.h"

/**
 * @file ConnectionSettingsComponent.h
 * 
 * ConnectionSettingsComponent is a widget that provides a connection status
 * icon, a toggle switch for enabling/disabling the connection, and a button
 * that shows a connection status string. The button will also open a 
 * connection page when clicked while the switch is enabled.
 */

class ConnectionSettingsComponent : public juce::Component,
        private juce::Button::Listener
{
public:
    /**
     * @param openConnectionPage  A callback to run when the button is clicked.
     *                             This should be a function that opens a 
     *                             connection settings page.
     * 
     * @param name                The internal component name.
     */
    ConnectionSettingsComponent(
            std::function<void() > openConnectionPage,
            const juce::String& name = juce::String());

    virtual ~ConnectionSettingsComponent() { }

protected:
    /**
     * Updates the icon, switch, and connection button based on current
     * connection status.
     */
    void refresh();

private:
    /**
     * Check the connections enabled status directly.  This will be used to
     * determine what position the switch should be in when the component
     * is created or refreshed.
     * 
     * @return true if connections are enabled, false if disabled.
     */
    virtual bool connectionEnabled() = 0;
    
    /**
     * This method is used by the component to determine if it should show the
     * loading spinner.
     * 
     * @return true if the loading spinner should be visible, false if it should
     *         be hidden.
     */
    virtual bool shouldShowSpinner() = 0;

    /**
     * This method is used by the component to determine if the connection 
     * switch should be enabled.
     * 
     * @return true iff the connection state can be changed.
     */
    virtual bool allowConnectionToggle() = 0;

    
    /**
     * This method is used by the component to determine if the connection 
     * page should be accessible.
     * 
     * @return true iff the connection button should open the connection page.
     */
    virtual bool connectionPageAvailable() = 0;
    
    /**
     * @return an appropriate asset file name for the connection type and
     *          status.
     */
    virtual juce::String getIconAsset() = 0;

    /**
     * This will be called whenever the enable/disable switch is flipped.
     * Subclasses should implement this to enable/disable their connections.
     * 
     * @param enabled  Passes on the new switch state.
     */
    virtual void enabledStateChanged(bool enabled) = 0;


    /**
     * Return connection status text for the current connection state.
     * Implementations of this should check their connections and/or the 
     * state of the enabled/disabled switch to determine an appropriate short
     * text value to print on the connection button.
     */
    virtual juce::String updateButtonText() = 0;

    /**
     * Arranges child components to fit within bounds.
     */
    void resized() override;

    /**
     * Update the icon color if text color changes.
     */
    void colourChanged() override;

    /**
     * If the connection button is clicked this will run openConnectionPage(). 
     * If the switch is clicked, this will call enabledStateChanged(), passing
     * it the switch toggle state.
     * 
     * @param b
     */
    void buttonClicked(juce::Button *b) override;

    /**
     * Run refresh() when the component regains visibility.
     */
    void visibilityChanged() override;

    /**
     * Connection button displays status text and opens the connection page
     * when possible.
     */
    class ConnectionButton : public juce::Button
    {
    public:
        /**
         * @param name    The internal component name
         */
        ConnectionButton(const juce::String& name = juce::String());

        virtual ~ConnectionButton() { }

        /**
         * Sets the text that will be printed on the button.
         * @param text
         */
        void setText(const juce::String &text);

    private:
        /**
         * Draws the connection button outline and prints the button text.
         * 
         * @param g
         * @param isMouseOverButton
         * @param isButtonDown
         */
        void paintButton
        (juce::Graphics &g, bool isMouseOverButton, bool isButtonDown) override;

        /**
         * Calculates button text height based on button size.
         */
        void resized() override;

        //last calculated text height
        int textHeight;
        //calculated when the component is resized, set to a tenth of component
        //height
        int borderSize;
        //Connection information text
        juce::String displayText;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionButton)
    };
    //Icon indicating the connection type and/or status
    DrawableImageComponent icon;
    //Shown when the connection is busy updating
    Spinner spinner;
    //Switch to enable/disable the connection
    SwitchComponent toggle;
    //Can be clicked to open the connection page
    ConnectionButton pageButton;
    //Callback function that opens the connection page
    std::function<void() > openConnectionPage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionSettingsComponent)
};