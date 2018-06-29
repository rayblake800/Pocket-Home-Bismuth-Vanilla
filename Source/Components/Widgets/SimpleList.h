#pragma once
#include "NavButton.h"
#include "LayoutManager.h"
/**
 * @file SimpleList.h
 * 
 * @brief  Abstract interface for classes arranging components in a vertical 
 *         scrolling list.
 * 
 * SimpleList evenly spaces Components in a vertical list, using up and down
 * NavButtons to scroll the list one page at a time.  SimpleList takes ownership
 * of all Components in the list.  When scrolling through the list, existing
 * Components are re-used and updated.
 */
class SimpleList : public Component, private Button::Listener
{
public:
    SimpleList();

    virtual ~SimpleList() { }
    
    /**
     * Return the number of items in the list.
     * 
     * @return the number of Components in the list.
     */
    virtual unsigned int getListSize() = 0;
    
    /**
     * Updates a component so it can be used as a specific list item.
     * 
     * @param listItem  A list item Component to update.  This parameter may be
     *                  null, in which case a new Component should be created.
     * 
     * @param index     The index where the list item will be used.  Subclasses
     *                  should use this to select the appropriate properties or 
     *                  content to apply to the Component.
     * 
     * @return  The updated list Component. 
     */
    virtual Component* updateListItem(Component* listItem,
            unsigned int index) = 0;
    
protected:
    
    /**
     * Sets the number of list items that are displayed at one time.
     * 
     * @param perPage  The number of Components to display per list page.
     */
    void setItemsPerPage(int perPage);
    
    /**
     * Reloads list content, running updateListItem for each visible
     * list item.
     * 
     * @param transition  Optional transition animation to apply when updating
     *                    list content.
     * 
     * @param duration    Duration in milliseconds to run transition animations.
     */
    void refreshListContent(TransitionAnimator::Transition transition
            = TransitionAnimator::none, unsigned int duration = 0);
        
private:
    /**
     * Scrolls the list when the navigation buttons are clicked.  
     *
     * @param button  One of the two page navigation buttons.
     */
    virtual void buttonClicked(Button* button) final override;
    
    /**
     * Re-positions list items and navigation buttons when the list is resized.
     */
    virtual void resized() override;
    
    //Handles list item positioning
    LayoutManager layoutManager;
    
    //List navigation buttons scroll the list by one page.
    NavButton upButton;
    NavButton downButton;
    
    //All Components displayed in the list.
    OwnedArray<Component> listComponents;
    
    //number of list items to display per page
    unsigned int itemsPerPage = 5;
    
    //index of the current list page
    unsigned int pageIndex = 0;
    
    //Fraction of the list vertical space to place between each list item pair
    static const constexpr float yPaddingFraction = 0.03;
    
    //animation duration(milliseconds) when scrolling between list pages
    static const constexpr unsigned int animDuration = 1000;
};