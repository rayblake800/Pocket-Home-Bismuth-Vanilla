#pragma once
#include "PageComponent.h"
#include "LayoutManager.h"
#include "SimpleList.h"

/**
 * @file FocusingListPage
 * 
 * @brief Shows a scrolling list of items, which can be selected to show
 *        additional information or controls.
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
     * @return the number of items in the list.
     */
    virtual unsigned int getListSize() = 0;

    /**
     * Gets the index of the selected list item.
     * 
     * @return  The selected index, or -1 if no list item is selected.
     */
    int getSelectedIndex();
    
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
     */
    void updateList();

    /**
     * Handles list item selection.
     * 
     * @button  A list item button that was clicked by the user.
     */
    void pageButtonClicked(Button* button) override;


    /**
     * When a list item is selected, override the back button to de-select
     * the list item instead of closing the page.
     * 
     * @return true if a list item was selected when the back button was
     *         clicked.
     */
    bool overrideBackButton() override;

private:
    
    /**
     * Used to load or update content for each item in the list.
     * 
     * @param layout   The layout of a single list item.  If empty, appropriate
     *                components should be added to the layout.  Otherwise, any 
     *                existing components in the layout should be updated as 
     *                necessary.  FocusingListPage will not take ownership of
     *                Components added to the layout. 
     *                
     * @param index    The index of the list item being updated.  This should
     *                be used to select appropriate content for the layout. 
     */
    virtual void updateListItemLayout(LayoutManager::Layout& layout,
            const unsigned int index) = 0;
    
    /**
     * Used to load or update additional content for the selected list item.
     * 
     * @param layout   The layout used for a list item when it is selected. If 
     *                empty, new components should be added. Otherwise, existing
     *                components in the layout should be updated as necessary.  
     *                FocusingListPage will not take ownership of components 
     *                added to the layout. 
     */
    virtual void updateSelectedItemLayout(LayoutManager::Layout& layout) = 0;
    
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
        LayoutManager::Layout getLayout();
        
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
            const unsigned int duration = 0));
        
    private:
        /**
         * Draws a border around the list item.
         * 
         * @param g The Juce graphics context.
         */
        void paint(Graphics &g) override;
        
        /**
         * Reapply the list item's layout when it is resized. 
         */
        void resized() override;
        
        LayoutManager::Layout buttonLayout;
    };


    class FocusingList : public SimpleList
    {
    public:
        FocusingList();

        virtual ~FocusingList() { }
        
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
    };

    FocusingList pageList;
    int selectedIndex = -1;
};