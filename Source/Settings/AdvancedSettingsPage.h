/**
 * @file AdvancedSettingsPage.h
 * 
 * AdvancedSettingsPage is a navigation page containing buttons that open all
 * other settings pages.
 */
#pragma once
#include "../Configuration/PersonalizePageComponent.h"
#include "../Configuration/DateTimePage.h"
#include "../Configuration/Configurables/ConfigurableImageButton.h"
#include "../Basic Components/ScalingLabel.h"
#include "InputSettingsPage.h"
#include "LoginSettingsPage.h"

class AdvancedSettingsPage : public Component, public Button::Listener {
public:
    AdvancedSettingsPage(AppConfigFile& appConfig);
    ~AdvancedSettingsPage();

private:
    /**
     * Get pointers to all buttons on the page
     * @param includeAll if false, only get pointers to page buttons
     * and exclude ones not relevant to the current program state.
     * @return pointers to all page buttons, in order.
     */
    std::vector<Button*> getButtonList(bool includeAll = false);

    /**
     * Reloads the page layout settings.
     */
    void reloadLayout();

    /**
     * Reloads page layout whenever the page becomes visible.
     */
    void visibilityChanged() override;

    /**
     * Handle button clicks to open menu pages, close this page, or 
     * scroll the list of page buttons.
     */
    void buttonClicked(Button*) override;

    /**
     * Fills in the background color
     */
    void paint(Graphics&) override;

    /**
     * Layout child components to fit within the page bounds
     */
    void resized() override;

    GridLayoutManager layoutManager;
    //Title of the page: "Advanced Settings"
    ScalingLabel titleLabel;
    //Closes this page
    ConfigurableImageButton backButton;
    //Page background color: currently fixed at 0xffd23c6d
    Colour bg_color;

    //Pages with associated buttons
    TextButton personalizeButton;
    TextButton setPasswordButton;
    TextButton removePasswordButton;
    TextButton dateTimeButton;
    TextButton inputOptionsButton;
    LoginSettingsPage loginSettingPage;
    PersonalizePageComponent personalizePage;
    DateTimePage dateTimePage;
    InputSettingsPage inputPage;

    //Next and previous buttons for scrolling page buttons
    DrawableImageButton prevArrow;
    DrawableImageButton nextArrow;

    //Number of page buttons to show on the screen at a time
    static const int buttonsPerPage = 4;
    //Current topmost page button index, corresponding to button index
    //in the vector returned by getButtonList() 
    int buttonIndex = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdvancedSettingsPage)
};