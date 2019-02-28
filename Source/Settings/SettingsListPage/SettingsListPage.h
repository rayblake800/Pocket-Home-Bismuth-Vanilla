#pragma once
/**
 * @file SettingsListPage.h
 * 
 * @brief  Provides an interface for accessing all other settings pages.
 */

#include "Page_Component.h"
#include "Locale/TextUser.h"
#include "NavButton.h"
#include "PagedList.h"

/**
 * @brief  A navigation page containing buttons that open other settings pages.
 */
class SettingsListPage : public Page::Component, public Locale::TextUser
{
public:
    SettingsListPage();

    virtual ~SettingsListPage() { }

private:
    /**
     * @brief  Gets button titles for all page buttons.
     *
     * @return  Titles for all page buttons, in order.
     */
    juce::StringArray getButtonTitles();

    /**
     * @brief  Reloads page layout whenever the page becomes visible.
     */
    void visibilityChanged() override;

    /**
     * @brief  Handles button clicks within the page.
     */
    class PageListener : public juce::Button::Listener
    {
    public:
        /**
         * @brief  Connects this listener to its page component.
         *
         * @param listPage  The SettingsListPage that owns this listener.
         */
        PageListener(SettingsListPage& listPage) : listPage(listPage) { }

    private:
        /**
         * @brief  Handles button clicks to open menu pages.
         */
        void buttonClicked(juce::Button* button) override;

        SettingsListPage& listPage;
    };
    PageListener pageListener;
    
    /**
     * @brief  The list component providing all settings page buttons. 
     */
    class SettingsList : public PagedList
    {
    public:
        SettingsList();
        
        virtual ~SettingsList() { }
        
        /**
         * @brief Gets the total number of page buttons.
         * 
         * @return  The number of page buttons. 
         */
        virtual unsigned int getListSize() const;
        
        /**
         * @brief  Updates or creates a page TextButton for a specific button 
         *         index.
         * 
         * @param listItem  Either nullptr, or a recycled TextButton to update.
         * 
         * @param index     The button's index in the list.
         * 
         * @return  The updated list Component. 
         */
        virtual Component* updateListItem(Component* listItem,
                const unsigned int index);
        
        /**
         * @brief  Refreshes all list components.
         */
        void refresh();

    private:
    };
    SettingsList buttonList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsListPage)
};
