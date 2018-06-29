#pragma once
#include "PageComponent.h"
#include "Localized.h"
#include "NavButton.h"
#include "ScalingLabel.h"
#include "SimpleList.h"

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
     * Gets button titles for all page buttons.
     *
     * @return  Titles for all page buttons, in order.
     */
    StringArray getButtonTitles();

    /**
     * Reloads page layout whenever the page becomes visible.
     */
    void visibilityChanged() override;

    /**
     * Handle button clicks to open menu pages
     */
    void pageButtonClicked(Button*) override;
    
    
    class SettingsList : public SimpleList
    {
    public:
        SettingsList();
        
        virtual ~SettingsList() { }
        
        /**
         * Gets the total number of page buttons.
         * 
         * @return  The number of page buttons. 
         */
        virtual unsigned int getListSize();
        
        /**
         * Updates or creates a page TextButton for a specific button index.
         * 
         * @param listItem  Either nullptr, or a recycled TextButton to update.
         * 
         * @param index     The button's index in the list.
         * 
         * @return  The updated list Component. 
         */
        virtual Component* updateListItem(Component* listItem,
                unsigned int index);
        
        /**
         * Refresh list components.
         */
        void refresh();
    private:
        static const constexpr float yPaddingFraction = 0.09;
    };
    SettingsList buttonList;

    //Number of page buttons to show on the screen at a time
    static const constexpr int buttonsPerPage = 4;

    //localized text keys;
    static const constexpr char * advanced_settings = "advanced_settings";
    static const constexpr char * personalize_homepage = "personalize_homepage";
    static const constexpr char * set_password = "set_password";
    static const constexpr char * change_password = "change_password";
    static const constexpr char * remove_password = "remove_password";
    static const constexpr char * date_and_time = "date_and_time";
    static const constexpr char * input_settings = "input_settings";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdvancedSettingsPage)
};
