/**
 * @file AdvancedSettingsPage.h
 * 
 * AdvancedSettingsPage is a navigation page containing buttons that open
 * other settings pages.
 */
#pragma once
#include "PageComponent.h"
#include "ConfigurableImageButton.h"
#include "ScalingLabel.h"

class AdvancedSettingsPage : public PageComponent{
public:
    AdvancedSettingsPage(PageComponent::PageFactoryInterface* pageFactory,
            ComponentConfigFile& config);
    
    ~AdvancedSettingsPage() {}

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
     * Handle button clicks to open menu pages or scroll the list of page 
     * buttons.
     */
    void pageButtonClicked(Button*) override;
    
    /**
     * Updates the up/down navigation buttons to fit when the page changes
     * size.
     */
    void pageResized();


    //Title of the page: "Advanced Settings"
    ScalingLabel titleLabel;

    //Pages with associated buttons
    TextButton personalizeButton;
    TextButton setPasswordButton;
    TextButton removePasswordButton;
    TextButton dateTimeButton;
    TextButton inputOptionsButton;

    //Next and previous buttons for scrolling page buttons
    ConfigurableImageButton prevArrow;
    ConfigurableImageButton nextArrow;

    //Number of page buttons to show on the screen at a time
    static const int buttonsPerPage = 4;
    //Current topmost page button index, corresponding to button index
    //in the vector returned by getButtonList() 
    int buttonIndex = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdvancedSettingsPage)
};
