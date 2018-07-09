#include "FocusingListPage.h"

FocusingListPage::FocusingListPage() : PageComponent("FocusingListPage") 
{ 
    setBackButton(PageComponent::leftBackButton);
    setLayout(LayoutManager::Layout(
    {
        LayoutManager::Row(1,
        { 
            LayoutManager::RowItem(&pageList, 1) 
        })
    }));
}

/*
 * Gets the index of the selected list item.
 */
int FocusingListPage::getSelectedIndex() const
{
    return selectedIndex;
}

/*
 * Sets the selected list index.
 */
void FocusingListPage::setSelectedIndex(const int index)
{
    if(index >= 0 && index < getListSize())
    {
        selectedIndex = index;
        updateList(TransitionAnimator::toDestination, focusDuration);
    }
    
}

/*
 * If a list item is currently selected, it will be de-selected and the
 * component will be updated.
 */
void FocusingListPage::deselect() 
{
    if(selectedIndex >= 0)
    {
        selectedIndex = -1;
        updateList(TransitionAnimator::toDestination, focusDuration);
    }
}

/*
 * Refreshes displayed list content.
 */
void FocusingListPage::updateList(TransitionAnimator::Transition transition,
        unsigned int duration)
{
    pageList.updateList(transition, duration);
}

/*
 * Handles list item selection.
 */
void FocusingListPage::pageButtonClicked(Button* button)
{
    ListItem* listButton = dynamic_cast<ListItem*>(button);
    DBG("Button " << button->getName() << " Clicked");
    if(listButton != nullptr)
    {
        if(listButton->getIndex() != getSelectedIndex())
        {
            setSelectedIndex(listButton->getIndex());
        }
        else
        {
            deselect();
        }
    }
    else
    {
        DBG("NULL list button!");
    }
}

/*
 * When a list item is selected, override the back button to de-select
 * the list item instead of closing the page.
 */
bool FocusingListPage::overrideBackButton()
{
    if(getSelectedIndex() > 0)
    {
        return false;
    }
    deselect();
    return true;
}

FocusingListPage::ListItem::ListItem() : Button("FocusingListItem") 
{ 
    setWantsKeyboardFocus(false);
}

/*
 * Gets the layout used by this list item.
 */
LayoutManager::Layout FocusingListPage::ListItem::getLayout() const
{ 
    return buttonLayout.getLayout();
}

/*
 * Sets the layout used by this list item.  All components in the layout
 * will be added to the list item as child components.
 */
void FocusingListPage::ListItem::setLayout(LayoutManager::Layout layout,
        const TransitionAnimator::Transition transition,
        const unsigned int duration) 
{ 
    buttonLayout.transitionLayout(layout, this, transition, duration);
}
        
/*
 * Gets the current list index assigned to this list item.
 */
int FocusingListPage::ListItem::getIndex() const
{
    return index;
}

/**
 * Sets the list index value stored by this list item.
 */
void FocusingListPage::ListItem::setIndex(const int newIndex)
{
    index = newIndex;
}

/*
 * Draws a border around the list item.
 */
void FocusingListPage::ListItem::paintButton(Graphics &g,
                bool isMouseOverButton, bool isButtonDown) 
{ 
    g.setColour(findColour(Label::ColourIds::textColourId));
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 1, borderWidth);
}

/*
 * Reapply the list item's layout when it is resized. 
 */
void FocusingListPage::ListItem::resized()
{
    buttonLayout.layoutComponents(getLocalBounds());
}

FocusingListPage::FocusingList::FocusingList() 
{
    setItemsPerPage(defaultItemsPerPage);
    setYPaddingFraction(defaultPaddingFraction);
}
      
/*
* Reloads list content, running updateListItem for each visible
* list item.
*/
void FocusingListPage::FocusingList::updateList(
        TransitionAnimator::Transition transition,
        unsigned int duration)
{   
    FocusingListPage* parentPage 
            = dynamic_cast<FocusingListPage*>(getParentComponent());
    if(parentPage != nullptr)
    {
        setNavButtonsVisible(parentPage->getSelectedIndex() < 0);
        refreshListContent(transition, duration);
    }
}

/*
 * Reads the number of list items from the parent FocusingListPage
 */
unsigned int FocusingListPage::FocusingList::getListSize() 
{ 
    FocusingListPage* parentPage 
            = dynamic_cast<FocusingListPage*>(getParentComponent());
    if(parentPage != nullptr)
    {
        return parentPage->getListSize();
    }
    return 0;
}

/*
 * Updates a list item, loading and applying its layout from the parent
 * FocusingListPage.
 */
Component* FocusingListPage::FocusingList::updateListItem(Component* listItem,
        unsigned int index) 
{
    FocusingListPage* parentPage 
            = dynamic_cast<FocusingListPage*>(getParentComponent());
    if(parentPage == nullptr)
    {
        DBG("FocusingListPage::" << __func__ << ": Parent is null!");
        return nullptr;
    }
    ListItem * pageButton;
    if(listItem == nullptr)
    {
        pageButton = new ListItem();
        pageButton->addListener(parentPage);
    }
    else
    {
        pageButton = static_cast<ListItem*>(listItem);
    }
       
    LayoutManager::Layout buttonLayout = pageButton->getLayout();
    bool wasSelected = (buttonLayout == selectedLayout);
    bool  isSelected = (index == parentPage->getSelectedIndex());
    bool animateTransition = (wasSelected != isSelected);

    if(isSelected)
    {
        parentPage->updateSelectedItemLayout(selectedLayout);
        buttonLayout = selectedLayout;    
    }
    else
    {
        if(wasSelected)
        {
            buttonLayout = LayoutManager::Layout();
        }
        parentPage->updateListItemLayout(buttonLayout, index);
    }
    animateTransition = (animateTransition
            && (pageButton->getIndex() == index));

    pageButton->setLayout(buttonLayout, (animateTransition ?
            TransitionAnimator::toDestination
            : TransitionAnimator::none), parentPage->focusDuration);
    pageButton->setIndex(index);
    return pageButton;
}

/**
 * Gets the weight value used to determine the height of a particular
 * row item.  When no list item is selected, all row heights are
 * equal.  When a list item is selected, the selected item uses the
 * entire list height.
 */ 
unsigned int FocusingListPage::FocusingList::getListItemWeight
(const unsigned int index)
{
    FocusingListPage* parentPage 
            = static_cast<FocusingListPage*>(getParentComponent());
    int selected = parentPage->getSelectedIndex();
    if(selected == index || selected < 0)
    {
        return 1;
    }
    DBG("selected = " << selected << ", index=" <<(int) index);
    return 0;
} 
