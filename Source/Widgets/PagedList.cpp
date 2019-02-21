#include "PagedList.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "PagedList::";
#endif

    
/* Default animation duration(milliseconds) when scrolling between list 
 * pages: */
static const constexpr unsigned int defaultAnimDuration = 300;

PagedList::PagedList() : 
upButton(NavButton::up),
downButton(NavButton::down)        
{
    addAndMakeVisible(upButton);
    addAndMakeVisible(downButton);
    upButton.addListener(this);
    downButton.addListener(this);
    setInterceptsMouseClicks(false, true);
}

/*
 * Changes the current selected list page, updating the list layout to show the 
 * selected page.
 */
void PagedList::setPageIndex
(const unsigned int newIndex, const bool animate, const unsigned int duration)
{
    using Transition= Layout::Transition::Type;
    if(newIndex < 0 || newIndex >= getPageCount())
    {
        DBG(dbgPrefix << __func__ << ": can't set page to invalid index "
                << (int) newIndex);
        return;
    }
    else if(newIndex == pageIndex)
    {
        DBG(dbgPrefix << __func__ << ": tried to move to page " 
                << (int) newIndex << " but that's already the selected page.");
        return;
    }
    const Transition animationType = animate ?
        ((newIndex > pageIndex) ? Transition::moveUp : Transition::moveDown)
        : Transition::none;
    const unsigned int animationDuration = animate ? duration : 0;
    pageIndex = newIndex;
    refreshListContent(animationType, animationDuration);
    pageSelectionChanged();
}

/*
 * Provides the weight value used to set each list item's relative height. 
 */
unsigned int PagedList::getListItemWeight(const unsigned int index)
{
    return 1;
}

/*
 * Sets the number of list items that are displayed at one time.
 */
void PagedList::setItemsPerPage(const unsigned int perPage)
{
    if(perPage != itemsPerPage)
    {
        itemsPerPage = perPage;
        if(getParentComponent() != nullptr && !getBounds().isEmpty())
        {
            refreshListContent();
        }
    }
}

/*
 * Gets the number of list items shown on the page at one time.
 */
unsigned int PagedList::getItemsPerPage() const
{
    return itemsPerPage;
}

/*
 * Gets the number of pages in the list.
 */
unsigned int PagedList::getPageCount() const
{
    const unsigned int listSize = getListSize();
    unsigned int pageCount = listSize / itemsPerPage;
    if((listSize % itemsPerPage) > 0)
    {
        pageCount++;
    }
    return pageCount;
}
    
/*
 * Sets the fraction of the list height that should be placed between list
 * items.
 */
void PagedList::setYPaddingFraction(const float paddingFraction)
{
    yPaddingFraction = paddingFraction;
    if(getParentComponent() != nullptr && !getBounds().isEmpty())
    {
        refreshListContent();
    }
}
  
/*
 * Reloads list content, running updateListItem for each visible
 * list item.
 */
void PagedList::refreshListContent(const Layout::Transition::Type transition,
        const unsigned int duration)
{
    Layout::Group::RelativeLayout layout;
    unsigned int listSize = getListSize();
    pageIndex = std::min(pageIndex, getPageCount() - 1);
    int componentsSaved = listComponents.size();
    // Update the list layout:
    for(int i = 0; i < itemsPerPage; i++)
    {
        int itemIndex = i + pageIndex * itemsPerPage;
        Layout::Group::Row row(getListItemWeight(itemIndex));
        if(itemIndex < listSize)
        {
            if(componentsSaved < i || listComponents[i] == nullptr)
            {
                listComponents.set(i, updateListItem(nullptr, itemIndex));
            }
            row.addRowItem(Layout::Group::RowItem(listComponents[i], 1));
        }
        layout.addRow(row);
    }
    layout.setYPaddingFraction(yPaddingFraction);
    layout.setYMarginFraction(std::max(upButton.yMarginFractionNeeded(),
            downButton.yMarginFractionNeeded()));
    layoutManager.transitionLayout(layout, this, transition, duration);
    // Update individual list components:
    for(int i = 0; i < itemsPerPage; i++)
    {
        int itemIndex = i + pageIndex * itemsPerPage;
        if(itemIndex >= listSize)
        {
            break;
        }
        updateListItem(listComponents[i], itemIndex);
    }
    updateNavButtonVisibility(showNavButtons);
}

/*
 * Updates the list page navigation button visibility.
 */
void PagedList::updateNavButtonVisibility(const bool buttonsVisible)
{
    showNavButtons = buttonsVisible;
    upButton.setVisible(showNavButtons && pageIndex > 0);
    downButton.setVisible(showNavButtons && pageIndex < (getPageCount() - 1));
}

/*
 * Scrolls the list when the navigation buttons are clicked.  
 */
void PagedList::buttonClicked(juce::Button* button)
{
    int newIndex = pageIndex;
    if(button == &upButton)
    {
        newIndex--;
    }
    else if(button == &downButton)
    {
        newIndex++;
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Unexpected button \""
                << button->getName() << "\" captured by PagedList!"); 
        return;
    }
    if(newIndex >= 0 && newIndex < getPageCount())
    {
        setPageIndex(newIndex, true, defaultAnimDuration);
    }
}

/*
 * Re-positions list items and navigation buttons when the list is resized.
 */
void PagedList::resized()
{
    layoutManager.layoutComponents(getLocalBounds());
    upButton.applyConfigBounds();
    downButton.applyConfigBounds();
    upButton.setBounds(getLocalArea(getParentComponent(),
            upButton.getBounds()));
    downButton.setBounds(getLocalArea(getParentComponent(),
            downButton.getBounds()));
}
