#pragma once
#include "JuceHeader.h"
#include "SwitchComponent.h"
#include "Spinner.h"
#include "DrawableImageComponent.h"

/**
 * @file  ConnectionSettingsComponent.h
 * 
 * @brief  A widget that provides a connection status icon, a toggle switch for 
 *         enabling/disabling the connection, and a button that shows a 
 *         connection status string. The button will also open a connection page
 *         when clicked while the switch is enabled.
 */

class ConnectionSettingsComponent : public juce::Component,
        private juce::Button::Listener
{
public:
    /**
     * @param openConnectionPage  A callback to run when the button is clicked.
     *                            This should be a function that opens a 
     *                            connection settings page.
     * 
     * @param name                The internal component name.
     */
    ConnectionSettingsComponent(
            std::function<void()> openConnectionPage,
            const juce::String& name = juce::String());

    virtual ~ConnectionSettingsComponent() { }

protected:
    /**
     * @brief  Updates the icon, switch, and connection button based on current
     *         connection status.
     */
    void refresh();

private:
    /**
     * @brief  Checks if the connection is enabled.  
     *
     * This will be used to determine what position the switch should be in when
     * the component is created or refreshed.
     * 
     * @return  True if connections are enabled, false if disabled.
     */
    virtual bool connectionEnabled() = 0;
    
    /**
     * @brief  Checks if the loading spinner should be visible.
     * 
     * @return  True if the loading spinner should be visible, false if it 
     *          should be hidden.
     */
    virtual bool shouldShowSpinner() = 0;

    /**
     * @brief  Determines if the connection switch should be enabled.
     * 
     * @return  True iff the connection state can be changed.
     */
    virtual bool allowConnectionToggle() = 0;

    
    /**
     * @brief  Determines if the connection page should be accessible.
     * 
     * @return  True iff the connection button should open the connection page.
     */
    virtual bool connectionPageAvailable() = 0;
    
    /**
     * @brief  Gets the icon asset path that should be used for the connection
     *         status icon.
     *
     * @return  An appropriate asset file name for the connection type and
     *          status.
     */
    virtual juce::String getIconAsset() = 0;

    /**
     * @brief  Enables or disables the connection when the connection toggle
     *         switch is clicked.
     * 
     * @param enabled  The new state of the connection toggle switch.
     */
    virtual void enabledStateChanged(bool enabled) = 0;


    /**
     * @brief  Return connection status text for the current connection state.
     *
     * @return  Text describing the connection state.
     */
    virtual juce::String updateButtonText() = 0;

    /**
     * @brief  Arranges child components to fit within the component bounds.
     */
    void resized() override;

    /**
     * @brief  If text color changes, updates the icon color to match.
     */
    void colourChanged() override;

    /**
     * @brief  Handles click events from the connection page button or the
     *         connection toggle switch.
     *
     * If the connection button is clicked this will run openConnectionPage(). 
     * If the switch is clicked, this will call enabledStateChanged(), passing
     * it the switch toggle state.
     * 
     * @param button  The button that was clicked.
     */
    void buttonClicked(juce::Button* button) override;

    /**
     * @brief  Run refresh() when the component regains visibility.
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
         * @param name    The internal component name.
         */
        ConnectionButton(const juce::String name = juce::String());

        virtual ~ConnectionButton() { }

        /**
         * @brief  Sets the text that will be printed on the button.
         *
         * @param text  The new button text.
         */
        void setText(const juce::String text);

    private:
        /**
         * @brief  Draws the connection button outline and prints the button 
         *         text.
         * 
         * @param g                  The juce Graphics object.
         *
         * @param isMouseOverButton  True iff the mouse is over the connection
         *                           button.
         *
         * @param isButtonDown       True iff the connection button is held
         *                           down.
         */
        void paintButton
        (juce::Graphics &g, bool isMouseOverButton, bool isButtonDown) override;

        /**
         * @brief  Calculates button text height based on button size.
         */
        void resized() override;

        /* Last calculated text height */
        int textHeight;
        /* Button border width in pixels */
        int borderSize;
        /* Connection information text */
        juce::String displayText;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionButton)
    };
    /* Icon indicating the connection type and/or status */
    DrawableImageComponent icon;
    /* Shown when the connection is busy updating */
    Spinner spinner;
    /* Switch to enable/disable the connection */
    SwitchComponent toggle;
    /* Can be clicked to open the connection page */
    ConnectionButton pageButton;
    /* Callback function that opens the connection page */
    std::function<void()> openConnectionPage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionSettingsComponent)
};
