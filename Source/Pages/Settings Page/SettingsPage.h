/**
 * @file SettingsPage.h
 * 
 * The settings page provides wireless device controls, brightness and volume
 * sliders, and a button to open additional settings pages.
 */
#pragma once

#include "IconSliderComponent.h"
#include "Localized.h"
#include "SwitchComponent.h"
#include "ConfigurableImageButton.h"
#include "PageComponent.h"
#include "WindowFocusedTimer.h"
#include "WifiSettingsComponent.h"
//#include "BluetoothSettingsComponent.h"

class SettingsPage : public PageComponent, public WindowFocusedTimer,
private Slider::Listener, private Localized
{
public:
    SettingsPage();

    virtual ~SettingsPage() { }

    private:    
    /**
     * Cancels the slider timer when visibility is lost.
     */
    virtual void visibilityChanged() override;
    
    /**
     * Used to update the sliders while they're being dragged.
     */
    virtual void timerCallback() override;
    
    /**
     * Opens the advanced settings page when its button is clicked.
     * 
     * @param b
     */
    void pageButtonClicked(Button *b) override;

    /**
     * Slider::Listener requires this method to be implemented, but it's not 
     * actually needed.
     */
    void sliderValueChanged(Slider* slider) { };
    
    /**
     * Starts a timer to update the slider values as its being dragged.
     * 
     * @param slider
     */
    void sliderDragStarted(Slider* slider);
    
    /**
     * Stops the timer and immediately updates slider values.
     * 
     * @param slider
     */
    void sliderDragEnded(Slider* slider);
    
    //Tracks the slider currently being dragged so the timer callback knows
    //whether it should update brightness or volume
    Slider* changingSlider;

    //Turns wifi on or off, shows connection state, and opens the wifi page.
    WifiSettingsComponent wifiComponent;
    //Turns bluetooth on or of, shows connection state, and opens the bluetooth
    //page.
    //BluetoothSettingsComponent bluetoothComponent;
    //sets the display brightness
    IconSliderComponent screenBrightnessSlider;
    //sets system volume levels
    IconSliderComponent volumeSlider;
    //opens the advanced settings page
    TextButton advancedPageButton;
    
    //localized text keys
    static const constexpr char * advanced_settings = "advanced_settings";
};
