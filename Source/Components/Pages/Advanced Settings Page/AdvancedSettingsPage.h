#pragma once
#include "PageComponent.h"
#include "Localized.h"
#include "NavButton.h"
#include "ScalingLabel.h"

/**
 * @file AdvancedSettingsPage.h
 * 
 * @brief AdvancedSettingsPage is a navigation page containing buttons that open
 * other settings pages.
 */

class AdvancedSettingsPage : public PageComponent, private Localized
{
public:
    AdvancedSettingsPage();

    virtual ~AdvancedSettingsPage() { }

private:
    /**
     * Get pointers to all buttons on the page
     * 
     * @param includeAll  If false, only get pointers to page buttons
     *                     and exclude ones not relevant to the current program 
     *                     state.
     
     * @return            pointers to all page buttons, in order.
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

    //Pages with associated buttons
    TextButton personalizeButton;
    TextButton setPasswordButton;
    TextButton removePasswordButton;
    TextButton dateTimeButton;
    TextButton inputOptionsButton;

    //Next and previous buttons for scrolling page buttons
    NavButton prevArrow;
    NavButton nextArrow;

    //Number of page buttons to show on the screen at a time
    static const int buttonsPerPage = 4;
    //Current topmost page button index, corresponding to button index
    //in the vector returned by getButtonList() 
    int buttonIndex = 0;

    //localized text keys;
    static const constexpr char * advanced_settings = "advanced_settings";
    static const constexpr char * set_password = "set_password";
    static const constexpr char * change_password = "change_password";
    static const constexpr char * remove_password = "remove_password";
    static const constexpr char * personalize_homepage = "personalize_homepage";
    static const constexpr char * date_and_time = "date_and_time";
    static const constexpr char * input_settings = "input_settings";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdvancedSettingsPage)
};
