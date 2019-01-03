#pragma once
#include "IconSliderComponent.h"
#include "SwitchComponent.h"
#include "ConfigurableImageButton.h"
#include "PageComponent.h"
#include "WindowFocusedTimer.h"
#include "Wifi/Component/ControlWidget.h"
#include "ScalingTextButton.h"
//#include "BluetoothSettingsComponent.h"

/**
 * @file  QuickSettingsPage.h
 * 
 * @brief  Provides wireless device controls, brightness and volume sliders, and 
 *         a button to open additional settings pages.
 */

class QuickSettingsPage : public PageComponent, public WindowFocusedTimer,
        private juce::Slider::Listener
{
public:
    QuickSettingsPage();

    virtual ~QuickSettingsPage() { }

    private:    
    /**
     * @brief  Cancels the slider timer when visibility is lost.
     */
    virtual void visibilityChanged() override;
    
    /**
     * @brief  Updates the sliders while they're being dragged.
     */
    virtual void timerCallback() override;
    
    /**
     * @brief  Opens the settings list page when its button is clicked.
     * 
     * @param button  This should always be the settingsListBtn.
     */
    void pageButtonClicked(juce::Button *b) override;

    /**
     * @brief  Updates the advanced settings button when the page is resized.
     */
    void pageResized() override;

    /**
     * @brief  Slider::Listener requires this method to be implemented, but it's
     *         not actually used.  
     *
     * Brightness and volume do not need to be updated as frequently as this
     * method would be called, so a timer is used instead to update the sliders
     * while they're being dragged.
     */
    virtual void sliderValueChanged(juce::Slider* slider) override { };
    
    /**
     * @brief  Starts a timer to update the slider values as its being dragged.
     * 
     * @param slider  The slider being dragged, either the brightness or volume
     *                slider.
     */
    virtual void sliderDragStarted(juce::Slider* slider) override;
    
    /**
     * @brief  Stops the timer and immediately updates slider values.
     * 
     * @param slider  The slider component that was being dragged, either the
     *                brightness or volume slider.
     */
    virtual void sliderDragEnded(juce::Slider* slider) override;
    
    /* Tracks the slider currently being dragged so the timer callback knows
       whether it should update brightness or volume. */
    juce::Slider* changingSlider;

    /* Turns wifi on or off, shows connection state, and opens the wifi page. */
    Wifi::ControlWidget wifiComponent;
    /* Turns bluetooth on or off, shows connection state, and opens the 
       bluetooth page. */
    //BluetoothSettingsComponent bluetoothComponent;
    /* Sets the display brightness */
    IconSliderComponent screenBrightnessSlider;
    /* Sets system volume levels */
    IconSliderComponent volumeSlider;
    /* Opens the settings list page */
    ConfigurableImageButton settingsListBtn;
};
