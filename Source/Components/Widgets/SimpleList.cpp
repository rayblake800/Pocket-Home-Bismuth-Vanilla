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

/**
 * Provides the weight value used to set each list item's relative
 * height. All list items have vertical weight 1 by default.  Subclasses
 * should override this method if they need to have rows of varying
 * heights.
 */
unsigned int SimpleList::getListItemWeight(const unsigned int index)
{
    return 1;
}

/*
 * Sets the number of list items that are displayed at one time.
 */
void SimpleList::setItemsPerPage(int perPage)
{
    if(perPage != itemsPerPage)
    {
        itemsPerPage = perPage;
        if(getParentComponent() != nullptr && !getBounds().isEmpty())
        {
            refreshListContent();
        }
        //if items per page decreased, remove unneeded list components
        while(listComponents.size() > itemsPerPage)
        {
            listComponents.removeLast();
        }
    }
}


    
/*
 * Sets the fraction of the list height that should be placed between list
 * items.
 */
void SimpleList::setYPaddingFraction(float paddingFraction)
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
        LayoutManager::Row row(1);
        int itemIndex = i + pageIndex * itemsPerPage;
        if(itemIndex < listSize)
        {
            if(componentsSaved < i || listComponents[i] == nullptr)
            {
                listComponents.set(i, updateListItem(nullptr, itemIndex));
            }
            row.addRowItem(LayoutManager::RowItem(listComponents[i], 1));
        }
        layout.addRow(row);
    }
    layout.setYPaddingFraction(yPaddingFraction);
    DBG("yPadding = " << yPaddingFraction);
    layout.setYMarginFraction(std::max(upButton.yMarginFractionNeeded(),
            downButton.yMarginFractionNeeded()));
    layoutManager.transitionLayout(layout, this, transition, duration);
    layoutManager.printLayout();
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
    upButton.setBounds(getLocalArea(getParentComponent(),
            upButton.getBounds()));
    downButton.setBounds(getLocalArea(getParentComponent(),
            downButton.getBounds()));
}
