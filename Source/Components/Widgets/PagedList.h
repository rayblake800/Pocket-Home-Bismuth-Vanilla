#pragma once
#include "NavButton.h"
#include "LayoutManager.h"
/**
 * @file PagedList.h
 * 
 * @brief  Abstract interface for classes arranging components in a vertical 
 *         scrolling list.
 * 
 * PagedList evenly spaces Components in a vertical list, using up and down
 * NavButtons to scroll the list one page at a time.  PagedList takes ownership
 * of all Components in the list.  When scrolling through the list, existing
 * Components are re-used and updated.
 */
class PagedList : public Component, private Button::Listener
{
public:
    PagedList();

    virtual ~PagedList() { }
    
    /**
     * Return the number of items in the list.
     * 
     * @return the number of Components in the list.
     */
    virtual unsigned int getListSize() = 0;
    
protected:
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
            const unsigned int index) = 0;
    
    /**
     * Provides the weight value used to set each list item's relative
     * height. All list items have vertical weight 1 by default.  Subclasses
     * should override this method if they need to have rows of varying
     * heights.
     *
     * @param index  A list index.  This may be an index greater than the list
     *               bounds, if determining the weight of an empty list row
     *               after the last list item.
     *
     * @return  The weight value that should be used to determine the relative
     *          height of this index.
     */
    virtual unsigned int getListItemWeight(const unsigned int index);    
    
    /**
     * Sets the number of list items that are displayed at one time.
     * 
     * @param perPage  The number of Components to display per list page.
     */
    void setItemsPerPage(int perPage);
    
    /**
     * Gets the number of list items shown on the page at one time.
     *
     * @return  The number of Components to display per list page.
     */
    unsigned int getItemsPerPage() const;

    /**
     * Sets the fraction of the list height that should be placed between list
     * items.
     * 
     * @param paddingFraction  The fraction of the height used to determine
     *                         padding size/
     */
    void setYPaddingFraction(float paddingFraction);
    
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
        
    /**
     * Shows or hides the list page navigation buttons. By default, navigation
     * buttons will be visible.  Even if navigation buttons are currently
     * set as visible, the up navigation button will still be hidden on the
     * first list page, and the down navigation button will still be hidden on
     * the last list page.
     *
     * @param buttonsVisible  If true, navigation buttons will be shown when
     *                        relevant.  If false, navigation buttons will
     *                        always be hidden.
     */
    void setNavButtonsVisible(bool buttonsVisible);

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
    
    //Tracks if navigation buttons should be shown.
    bool showNavButtons = true;

    //All Components displayed in the list.
    OwnedArray<Component> listComponents;
    
    //number of list items to display per page
    unsigned int itemsPerPage = 5;
    
    //index of the current list page
    unsigned int pageIndex = 0;
    
    //Fraction of the list vertical space to place between each list item pair
    float yPaddingFraction = 0.05;
    
    //animation duration(milliseconds) when scrolling between list pages
    static const constexpr unsigned int animDuration = 300;
};
