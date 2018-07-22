#pragma once
#include "PageComponent.h"
#include "LayoutManager.h"
#include "PagedList.h"

/**
 * @file FocusingListPage
 * 
 * @brief Shows a scrolling list of items.  List items can be selected, causing
 *        the page to focus on them and potentially display additional content
 *        relevant to the selected item.
 * 
 *  FocusingListPage is an abstract PageComponent that contains a single 
 * PagedList.  When a list item is clicked, it expands to fill the page, hiding
 * all other list items and the PagedList's navigation buttons.  While a list
 * item is selected, the page's back button will be set to de-select the focused
 * list item instead of closing the page.
 * 
 *  Implementations of FocusingListPage are responsible for providing all list
 * items and setting the layout of list item components.  
 */

class FocusingListPage : public PageComponent
{
public:
    FocusingListPage();

    virtual ~FocusingListPage() { }

protected:
    /**
     * Determines the total number of items that should be in the list.
     * 
     * @return   The number of items in the list.
     */
    virtual unsigned int getListSize() = 0;
    
    /**
     * Called to handle button click events for any buttons other than
     * navigation buttons and list items.  Subclasses should override this if
     * they need to handle additional button events.
     * 
     * @param button  A button component that was clicked.
     */
    virtual void listPageButtonClicked(Button* button) { }

    /**
     * Gets the index of the selected list item.
     * 
     * @return  The selected index, or -1 if no list item is selected.
     */
    int getSelectedIndex() const;

    /**
     * Sets the selected list index.
     * 
     * @param index  The index of the list item to select.  If this value is
     *               outside of the list bounds, nothing will happen.
     */
    void setSelectedIndex(const int index);

    /**
     * If a list item is currently selected, it will be de-selected and the
     * component will be updated.
     */
    void deselect();

    /**
     * Refreshes displayed list content.
     * 
     * @param transition  Optional transition animation to apply when 
     *                    updating list content.
     * 
     * @param duration    Duration in milliseconds to run transition 
     *                    animations.
     */
    void updateList(TransitionAnimator::Transition transition
            = TransitionAnimator::none, unsigned int duration = 0);

    /**
     * Handles list item selection.
     * 
     * @button  A list item button that was clicked by the user.
     */
    virtual void pageButtonClicked(Button* button) final override;


    /**
     * When a list item is selected, override the back button to de-select
     * the list item instead of closing the page.
     * 
     * @return   True if a list item was selected when the back button was
     *           clicked.
     */
    bool overrideBackButton() override;

private:

    /**
     * Loads or updates the Component layout for each item in the list.
     * 
     * @param layout  The layout of a single list item.  If empty, appropriate
     *                components should be added to the layout.  Otherwise, any 
     *                existing components in the layout should be updated to fit
     *                the provided list index.  
     *                
     * @param index   The index of the list item being updated.  This should
     *                be used to select appropriate content for the layout. 
     */
    virtual void updateListItemLayout(LayoutManager::Layout& layout,
            const unsigned int index) = 0;

    /**
     * Updates the layout of the selected list item.  Subclasses should override
     * this method to add additional information or control Components that will
     * only be shown when a list item is selected.
     * 
     * @param layout  The layout used for the selected list item.  This layout
     *                will always be updated with updateListItemLayout before
     *                this method is called.
     */
    virtual void updateSelectedItemLayout(LayoutManager::Layout& layout) { }

    class ListItem : public Button
    {
    public:
        ListItem();

        virtual ~ListItem() { }

        /**
         * Gets the layout used by this list item.
         * 
         * @return  The layout containing the list item's child components.
         */
        LayoutManager::Layout getLayout() const;

        /**
         * Sets the layout used by this list item.  All components in the layout
         * will be added to the list item as child components.
         * 
         * @param layout      The layout to apply to the list item.
         * 
         * @param transition  Optional transition animation to display when 
         *                    updating the layout.
         * 
         * @param duration    Duration (in milliseconds) of any transition
         *                    animation. 
         */
        void setLayout(LayoutManager::Layout layout,
                const TransitionAnimator::Transition transition
                = TransitionAnimator::none,
                const unsigned int duration = 0);
        
        /**
         * Gets the current list index assigned to this list item.
         * 
         * @return  The index represented by the list item. 
         */
        int getIndex() const;
        
        /**
         * Sets the list index value stored by this list item.
         * 
         * @newIndex  The list index value to store.
         */
        void setIndex(const int newIndex);

    private:
        /**
         * Draws a border around the list item.
         * 
         * @param g   The Juce graphics context.
         */
        void paintButton(Graphics &g, bool isMouseOverButton,
                        bool isButtonDown) override;

        /**
         * Re-apply the list item's layout when it is resized. 
         */
        virtual void resized() final override;
        
        int index = -1;
        
        static const constexpr unsigned int borderWidth = 4;

        LayoutManager buttonLayout;
    };

    class FocusingList : public PagedList
    {
    public:
        FocusingList();

        virtual ~FocusingList() { }

        /**
         * Reloads list content, running updateListItem for each visible
         * list item.
         * 
         * @param transition  Optional transition animation to apply when 
         *                    updating list content.
         * 
         * @param duration    Duration in milliseconds to run transition 
         *                    animations.
         */
        void updateList(TransitionAnimator::Transition transition
                = TransitionAnimator::none, unsigned int duration = 0);

    protected:
        /**
         * Reads the number of list items from the parent FocusingListPage
         * 
         * @return the number of items in the list.
         */
        virtual unsigned int getListSize() override;

        /**
         * Updates a list item, loading and applying its layout from the parent
         * FocusingListPage.
         * 
         * @param listItem  A ListItem component to update, or nullptr if a new
         *                  list item should be created.
         * 
         * @param index     The index of the updated list item.
         * 
         * @return   The updated ListItem.
         */
        virtual Component* updateListItem(Component* listItem,
                unsigned int index) override;
        
        /**
         * Gets the weight value used to determine the height of a particular
         * row item.  When no list item is selected, all row heights are
         * equal.  When a list item is selected, the selected item uses the
         * entire list height.
         *
         * @param index  The index of a list item, or of an empty space after
         *               the last list item.
         *
         * @return  Zero if a row item is selected and it is not the row item
         *          at the given index, one otherwise.
         */ 
        virtual unsigned int getListItemWeight(const unsigned int index)
                override;    
    private:
        
        //Layout to update and use for any focused list item.
        LayoutManager::Layout selectedLayout;
    };
    FocusingList pageList;
    
    //Current selected list index.
    int selectedIndex = -1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FocusingListPage)
};
