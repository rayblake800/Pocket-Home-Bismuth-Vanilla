#pragma once
/**
 * @file  QuickSettingsPage.h
 * 
 * @brief  Provides wireless device controls, brightness and volume sliders, and 
 *         a button to open additional settings pages.
 */

#include "IconSliderComponent.h"
#include "SwitchComponent.h"
#include "Layout_Component_Manager.h"
#include "Page_Component.h"
#include "WindowFocusedTimer.h"
#include "Wifi_ControlWidget.h"
#include "ScalingTextButton.h"
#include "Theme_Image_Component.h"


class QuickSettingsPage : public Page::Component, public WindowFocusedTimer
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
     * @brief  Updates the advanced settings button when the page is resized.
     */
    void pageResized() override;

    /**
     * @brief  Handles component input events for the QuickSettingsPage.
     */
    class PageListener : public juce::Button::Listener,
            public juce::Slider::Listener
    {
    public:
        /**
         * @brief  Connects this listener to its QuickSettingsPage.
         *
         * @param settingsPage  The page component that owns this Listener.
         */
        PageListener(QuickSettingsPage& settingsPage) : 
            settingsPage(settingsPage) { }

    private:
        /**
         * @brief  Opens the settings list page when its button is clicked.
         * 
         * @param button  This should always be the settingsListBtn.
         */
        void buttonClicked(juce::Button* button) override;

        /**
         * @brief  Slider::Listener requires this method to be implemented, but 
         *         its not actually used.  
         *
         *  Brightness and volume do not need to be updated as frequently as 
         * this method would be called, so a timer is used instead to update the
         * sliders while they're being dragged.
         */
        virtual void sliderValueChanged(juce::Slider* slider) override { };
        
        /**
         * @brief  Starts a timer to update the slider values as its being 
         *         dragged.
         * 
         * @param slider  The slider being dragged, either the brightness or 
         *                volume slider.
         */
        virtual void sliderDragStarted(juce::Slider* slider) override;
        
        /**
         * @brief  Stops the timer and immediately updates slider values.
         * 
         * @param slider  The slider component that was being dragged, either 
         *                the brightness or volume slider.
         */
        virtual void sliderDragEnded(juce::Slider* slider) override;

        QuickSettingsPage& settingsPage;
    };
    PageListener pageListener;
    
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
    Theme::Image::Component<DrawableImageButton> settingsListBtn;
    /* Sets the bounds of the settings list button: */
    Layout::Component::Manager listButtonManager; 
};
