#include "SimpleList.h"

SimpleList::SimpleList() : 
upButton(NavButton::up),
downButton(NavButton::down)        
{
    addAndMakeVisible(upButton);
    addAndMakeVisible(downButton);
    upButton.addListener(this);
    downButton.addListener(this);
}

/*
 * Sets the number of list items that are displayed at one time.
 */
void SimpleList::setItemsPerPage(int perPage)
{
    if(perPage != itemsPerPage)
    {
        itemsPerPage = perPage;
        refreshListContent();
        //if items per page decreased, remove unneeded list components
        while(listComponents.size() > itemsPerPage)
        {
            listComponents.removeLast();
        }
    }
}
  
/*
 * Reloads list content, running updateListItem for each visible
 * list item.
 */
void SimpleList::refreshListContent(TransitionAnimator::Transition transition,
        unsigned int duration)
{
    LayoutManager::Layout layout;
    unsigned int listSize = getListSize();
    while((pageIndex * itemsPerPage) > listSize)
    {
        pageIndex--;
    }
    int componentsSaved = listComponents.size();
    for(int i = 0; i < itemsPerPage; i++)
    {
        LayoutManager::Row row;
        int itemIndex = i + pageIndex * itemsPerPage;
        if(itemIndex < listSize)
        {
            if(componentsSaved < i)
            {
                listComponents.add(updateListItem(nullptr, itemIndex));
            }
            row.addRowItem(LayoutManager::RowItem(listComponents[i]));
        }
        layout.addRow(row);
    }
    layout.setYPaddingFraction(yPaddingFraction);
    layoutManager.transitionLayout(layout, this, transition, duration);
    //update recycled components
    for(int i = 0; i < componentsSaved; i++)
    {
        int itemIndex = i + pageIndex * itemsPerPage;
        if(itemIndex > listSize)
        {
            break;
        }
        updateListItem(listComponents[i], itemIndex);
    }
    upButton.setVisible(pageIndex > 0);
    downButton.setVisible(listSize > ((pageIndex + 1) * itemsPerPage));
}

/*
 * Scrolls the list when the navigation buttons are clicked.  
 */
void SimpleList::buttonClicked(Button* button)
{
    if(button == &upButton)
    {
        if(pageIndex > 0)
        {
            pageIndex--;
        }
        refreshListContent(TransitionAnimator::moveDown, animDuration);
    }
    else if(button == &downButton)
    {
        if(getListSize() > ((pageIndex + 1) * itemsPerPage))
        {
            pageIndex++;
        }
        refreshListContent(TransitionAnimator::moveUp, animDuration);
    }
}

/*
 * Re-positions list items and navigation buttons when the list is resized.
 */
void SimpleList::resized()
{
    layoutManager.layoutComponents(getLocalBounds());
    upButton.applyConfigBounds();
    downButton.applyConfigBounds();
}
