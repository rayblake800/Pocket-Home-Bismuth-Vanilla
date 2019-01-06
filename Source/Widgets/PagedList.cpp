#include "PagedList.h"

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

/**
 * Provides the weight value used to set each list item's relative
 * height. All list items have vertical weight 1 by default.  Subclasses
 * should override this method if they need to have rows of varying
 * heights.
 */
unsigned int PagedList::getListItemWeight(const unsigned int index)
{
    return 1;
}

/*
 * Sets the number of list items that are displayed at one time.
 */
void PagedList::setItemsPerPage(int perPage)
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
 * Sets the fraction of the list height that should be placed between list
 * items.
 */
void PagedList::setYPaddingFraction(float paddingFraction)
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
void PagedList::refreshListContent(Layout::Transition::Type transition,
        unsigned int duration)
{
    Layout::Group::RelativeLayout layout;
    unsigned int listSize = getListSize();
    while((pageIndex * itemsPerPage) > listSize)
    {
        pageIndex--;
    }
    int componentsSaved = listComponents.size();
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
    upButton.setVisible(showNavButtons && pageIndex > 0);
    downButton.setVisible(showNavButtons
            && listSize > ((pageIndex + 1) * itemsPerPage));
    layout.setYPaddingFraction(yPaddingFraction);
    layout.setYMarginFraction(std::max(upButton.yMarginFractionNeeded(),
            downButton.yMarginFractionNeeded()));
    layoutManager.transitionLayout(layout, this, transition, duration);
    //update recycled components
    for(int i = 0; i < itemsPerPage; i++)
    {
        int itemIndex = i + pageIndex * itemsPerPage;
        if(itemIndex >= listSize)
        {
            break;
        }
        updateListItem(listComponents[i], itemIndex);
    }
}

/*
 * Shows or hides the list page navigation buttons.
 */
void PagedList::setNavButtonsVisible(bool buttonsVisible)
{
    if(buttonsVisible != showNavButtons)
    {
        showNavButtons = buttonsVisible;
        upButton.setVisible(showNavButtons && pageIndex > 0);
        downButton.setVisible(showNavButtons
               && getListSize() > ((pageIndex + 1) * itemsPerPage));
    }
}


/*
 * Scrolls the list when the navigation buttons are clicked.  
 */
void PagedList::buttonClicked(juce::Button* button)
{
    if(button == &upButton)
    {
        if(pageIndex > 0)
        {
            pageIndex--;
        }
        refreshListContent(Layout::Transition::Type::moveDown, animDuration);
    }
    else if(button == &downButton)
    {
        if(getListSize() > ((pageIndex + 1) * itemsPerPage))
        {
            pageIndex++;
        }
        refreshListContent(Layout::Transition::Type::moveUp, animDuration);
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
