#include "FocusingListPage.h"

FocusingListPage::FocusingListPage() : PageComponent("FocusingListPage") { }

/*
 * Gets the index of the selected list item.
 */
int FocusingListPage::getSelectedIndex()
{
    return selectedIndex;
}

/*
 * Sets the selected list index.
 */
void FocusingListPage::setSelectedIndex(const int index)
{
    
}

/*
 * If a list item is currently selected, it will be de-selected and the
 * component will be updated.
 */
void FocusingListPage::deselect() 
{
}

/*
 * Refreshes displayed list content.
 */
void FocusingListPage::updateList() 
{
}

/*
 * Handles list item selection.
 */
void FocusingListPage::pageButtonClicked(Button* button)
{
}

/*
 * When a list item is selected, override the back button to de-select
 * the list item instead of closing the page.
 */
bool FocusingListPage::overrideBackButton()
{
}

FocusingListPage::ListItem::ListItem()
{
}

/*
 * Gets the layout used by this list item.
 */
LayoutManager::Layout FocusingListPage::ListItem::getLayout()
{ 
    return buttonLayout;
}

/*
 * Sets the layout used by this list item.  All components in the layout
 * will be added to the list item as child components.
 */
void FocusingListPage::ListItem::setLayout(LayoutManager::Layout layout,
        const TransitionAnimator::Transition transition,
        const unsigned int duration)) 
{ 
}

/*
 * Draws a border around the list item.
 */
void FocusingListPage::ListItem::paint(Graphics &g) 
{ 
}

/*
 * Reapply the list item's layout when it is resized. 
 */
void FocusingListPage::ListItem::resized()
{
}

FocusingListPage::FocusingList::FocusingList() 
{ 
}

/*
 * Reads the number of list items from the parent FocusingListPage
 */
unsigned int FocusingListPage::FocusingList::getListSize() 
{ 
}

/*
 * Updates a list item, loading and applying its layout from the parent
 * FocusingListPage.
 */
Component* FocusingListPage::FocusingList::updateListItem(Component* listItem,
        unsigned int index) 
{ 
}