#pragma once
/**
 * @file  Page_SettingsList.h
 *
 * @brief  Provides an interface for accessing all other settings pages.
 */

#include "Page_Component.h"
#include "Locale_TextUser.h"
#include "Widgets_NavButton.h"
#include "Widgets_PagedList.h"

namespace Page { class SettingsList; }

/**
 * @brief  A navigation page containing buttons that open other settings pages.
 */
class Page::SettingsList : public Page::Component, public Locale::TextUser
{
public:
    /**
     * @brief  Sets the page layout on construction.
     */
    SettingsList();

    virtual ~SettingsList() { }

private:
    /**
     * @brief  Updates the list of button titles, refreshing the list if needed.
     */
    void updateButtonTitles();

    /**
     * @brief  Checks if the list should be updated when the page is revealed.
     */
    virtual void pageRevealedOnStack() override;

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
        PageListener(SettingsList& listPage) : listPage(listPage) { }

    private:
        /**
         * @brief  Opens new pages when their buttons are clicked.
         */
        void buttonClicked(juce::Button* button) override;

        SettingsList& listPage;
    };
    PageListener pageListener;

    /**
     * @brief  The list component providing all settings page buttons.
     */
    class PageList : public Widgets::PagedList
    {
    public:
        /**
         * @brief  Connects the list to the page that contains it.
         *
         * @param page  The settings list page that holds this list.
         */
        PageList(SettingsList& page);

        virtual ~PageList() { }

        /**
         * @brief  Gets the total number of page buttons.
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
         * @return          The updated list Component.
         */
        virtual Component* updateListItem(Component* listItem,
                const unsigned int index);

        /**
         * @brief  Refreshes all list components.
         */
        void refresh();

    private:
        // The page that holds this list component:
        SettingsList& page;
    };
    PageList buttonList;

    // Holds all page titles that should currently have list buttons:
    juce::StringArray pageTitles;

    // Tracks if the application password was set the last time the list was
    // updated:
    bool passwordSet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsList)
};
