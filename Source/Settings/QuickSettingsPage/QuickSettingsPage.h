#pragma once
/**
 * @file  QuickSettingsPage.h
 * 
 * @brief  Provides wireless device controls, brightness and volume sliders, and 
 *         a button to open additional settings pages.
 */

#include "Widgets_Switch.h"
#include "Layout_Component_Manager.h"
#include "Page_Component.h"
#include "Wifi_ControlWidget.h"
#include "Theme_Image_Component.h"
#include "Settings_BrightnessSlider.h"
#include "Settings_VolumeSlider.h"


class QuickSettingsPage : public Page::Component
{
public:
    QuickSettingsPage();

    virtual ~QuickSettingsPage() { }

private:    
    /**
     * @brief  Updates the advanced settings button when the page is resized.
     */
    void pageResized() override;

    /**
     * @brief  Controls how the settings list button acts when clicked.
     */
    class PageListener : public juce::Button::Listener
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

        QuickSettingsPage& settingsPage;
    };
    PageListener pageListener;

    /* Turns wifi on or off, shows connection state, and opens the wifi page. */
    Wifi::ControlWidget wifiComponent;

    /* Sets the display brightness */
    Settings::BrightnessSlider brightnessSlider;

    /* Sets system volume levels */
    Settings::VolumeSlider volumeSlider;

    /* Opens the settings list page */
    Theme::Image::Component<Widgets::DrawableImageButton> settingsListBtn;

    /* Sets the bounds of the settings list button: */
    Layout::Component::Manager listButtonManager; 
};
