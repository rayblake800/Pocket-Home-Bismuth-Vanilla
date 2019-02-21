#pragma once
#include "PageComponent.h"
#include "Locale/TextUser.h"
#include "NavButton.h"
#include "ScalingLabel.h"
#include "PagedList.h"

/**
 * @file SettingsListPage.h
 * 
 * @brief A navigation page containing buttons that open other settings pages.
 */

class SettingsListPage : public PageComponent, public Locale::TextUser
{
public:
    SettingsListPage();

    virtual ~SettingsListPage() { }

private:
    
    /**
     * Gets button titles for all page buttons.
     *
     * @return  Titles for all page buttons, in order.
     */
    juce::StringArray getButtonTitles();

    /**
     * Reloads page layout whenever the page becomes visible.
     */
    void visibilityChanged() override;

    /**
     * Handle button clicks to open menu pages
     */
    void pageButtonClicked(juce::Button* button) override;
    
    
    class SettingsList : public PagedList
    {
    public:
        SettingsList();
        
        virtual ~SettingsList() { }
        
        /**
         * Gets the total number of page buttons.
         * 
         * @return  The number of page buttons. 
         */
        virtual unsigned int getListSize() const;
        
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
        static const constexpr float yPaddingFraction = 0.04;
    };
    SettingsList buttonList;

    //Number of page buttons to show on the screen at a time
    static const constexpr int buttonsPerPage = 5;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsListPage)
};
