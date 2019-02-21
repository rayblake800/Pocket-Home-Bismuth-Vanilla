#include "FocusingListPage.h"

/* Default number of list items per page: */
static const constexpr unsigned int defaultItemsPerPage = 5;

/* Default list padding fraction: */
static const constexpr float defaultPaddingFraction = 0.02;

/* Milliseconds to take when (un)focusing list content: */
static const constexpr unsigned int focusDuration = 300;

/* List item border width in pixels: */
static const constexpr unsigned int borderWidth = 4;

FocusingListPage::FocusingListPage() : PageComponent("FocusingListPage") 
{ 
    setBackButton(PageComponent::leftBackButton);
    using namespace Layout::Group;
    setLayout(RelativeLayout(
    {
        Row(1,
        { 
            RowItem(&pageList, 1) 
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
    if(index >= 0 && index < getListSize() && index != selectedIndex)
    {
        selectedIndex = index;
        updateList(Layout::Transition::Type::toDestination, focusDuration);
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
        updateList(Layout::Transition::Type::toDestination, focusDuration);
    }
}

/*
 * Refreshes displayed list content.
 */
void FocusingListPage::updateList(Layout::Transition::Type transition,
        unsigned int duration)
{
    pageList.updateList(transition, duration);
}

/*
 * Handles list item selection.
 */
void FocusingListPage::pageButtonClicked(juce::Button* button)
{
    ListItem* listButton = dynamic_cast<ListItem*>(button);
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
        listPageButtonClicked(button);
    }
}

/*
 * When a list item is selected, override the back button to de-select
 * the list item instead of closing the page.
 */
bool FocusingListPage::overrideBackButton()
{
    if(getSelectedIndex() < 0)
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
Layout::Group::RelativeLayout FocusingListPage::ListItem::getLayout() const
{ 
    return buttonLayoutManager.getLayout();
}

/*
 * Sets the layout used by this list item.  All components in the layout
 * will be added to the list item as child components.
 */
void FocusingListPage::ListItem::setLayout(Layout::Group::RelativeLayout layout,
        const Layout::Transition::Type transition,
        const unsigned int duration) 
{ 
    buttonLayoutManager.transitionLayout(layout, this, transition, duration);
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
void FocusingListPage::ListItem::paintButton(juce::Graphics &g,
                bool isMouseOverButton, bool isButtonDown) 
{ 
    using namespace juce;
    g.setColour(findColour(Label::ColourIds::textColourId));
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 1, borderWidth);
}

/*
 * Reapply the list item's layout when it is resized. 
 */
void FocusingListPage::ListItem::resized()
{
    buttonLayoutManager.layoutComponents(getLocalBounds());
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
        Layout::Transition::Type transition,
        unsigned int duration)
{   
    FocusingListPage* parentPage 
            = dynamic_cast<FocusingListPage*>(getParentComponent());
    if(parentPage != nullptr)
    {
        updateNavButtonVisibility(parentPage->getSelectedIndex() < 0);
        refreshListContent(transition, duration);
    }
}

/*
 * Reads the number of list items from the parent FocusingListPage
 */
unsigned int FocusingListPage::FocusingList::getListSize() const
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
juce::Component* FocusingListPage::FocusingList::updateListItem
(juce::Component* listItem, unsigned int index) 
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
       
    Layout::Group::RelativeLayout buttonLayout = pageButton->getLayout();
    Layout::Group::RelativeLayout selectedLayout 
            = selectedLayoutManager.getLayout();
    bool wasSelected = (buttonLayout == selectedLayout);
    bool isSelected  = (index == parentPage->getSelectedIndex());

    if(isSelected)
    {
        parentPage->updateListItemLayout(selectedLayout, index);
        parentPage->updateSelectedItemLayout(selectedLayout);
        buttonLayout = selectedLayout;    
    }
    else
    {
        if(wasSelected)
        {
            buttonLayout = Layout::Group::RelativeLayout();
        }
        parentPage->updateListItemLayout(buttonLayout, index);
    }
    pageButton->setLayout(buttonLayout);
    pageButton->setIndex(index);
    return pageButton;
}

/*
 * Gets the weight value used to determine the height of a particular row item.
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
    return 0;
} 
