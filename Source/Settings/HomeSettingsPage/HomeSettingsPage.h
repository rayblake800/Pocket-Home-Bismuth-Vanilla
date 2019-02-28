#pragma once
/**
 * @file  HomeSettingsPage.h
 * 
 * @brief  Allows the user to configure the HomePage, setting the page 
 *         background, the application menu type, and the application menu 
 *         dimensions.
 */

#include "ScalingLabel.h"
#include "Page_Component.h"
#include "AppMenu_SettingsComponents.h"
#include "Theme_Image_HomeBackgroundPicker.h"
#include "Utils.h"
#include "Locale/TextUser.h"

/**
 * @brief  The page component used to hold all HomePage and AppMenu settings
 *         control components.
 */
class HomeSettingsPage : public Page::Component, public Locale::TextUser
{
public:
    /**
     * @brief  Initializes the page layout.
     */
    HomeSettingsPage();

    /**
     * @brief  Updates AppMenu settings when the page closes.
     */
    virtual ~HomeSettingsPage();

private:
    /* Page title label: */
    ScalingLabel title;
    
    /* Used to set the HomePage's background image: */
    ScalingLabel bgLabel;
    Theme::Image::HomeBackgroundPicker homeBGPicker;

    /* Used to select AppMenu format settings: */
    AppMenu::SettingsComponents menuComponents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomeSettingsPage)
};
