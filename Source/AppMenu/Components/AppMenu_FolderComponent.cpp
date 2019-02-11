#define APPMENU_IMPLEMENTATION
#include "AppMenu_FolderComponent.h"
#include "DesktopEntry_Loader.h"
#include "ScopeTimer.h"

#ifdef JUCE_DEBUG
/* Print full class name before debug output: */
static const constexpr char* dbgPrefix = "AppMenu::FolderComponent::";
#endif

/*
 * Creates a new folder component.
 */
AppMenu::FolderComponent::FolderComponent(MenuItem folderItem) :
folderItem(folderItem) 
{
    ScopeTimer t("AppMenu::FolderComponent::FolderComponent");
    DBG(dbgPrefix << __func__ << ": Creating folder component holding "
            << folderItem.getFolderSize() << " folder items.");
    folderItem.addListener(this);
    setWantsKeyboardFocus(false);
}

/*
 * Creates, adds, and shows the folder's menu buttons if they have not yet been 
 * initialized.
 */
void AppMenu::FolderComponent::initMenuItems()
{
    ScopeTimer t("AppMenu::FolderComponent::initMenuItems");
    const int itemCount = folderItem.getFolderSize();
    for(int i = 0; i < itemCount; i++)
    {
        MenuItem buttonItem = folderItem.getFolderItem(i);
        jassert(!buttonItem.isNull());
        MenuButton* newButton = createMenuButton(buttonItem);
        folderButtons.add(newButton);
        addAndMakeVisible(newButton);
    }
}

/*
 * Gets the number of menu items in this folder.
 */
int AppMenu::FolderComponent::getFolderSize() const
{
    return folderButtons.size();
}

/*
 * Gets the folder's menu item data.
 */
AppMenu::MenuItem AppMenu::FolderComponent::getFolderMenuItem() const
{
    return folderItem;
}

/*
 * Gets the current selected folder item.
 */
AppMenu::MenuItem 
AppMenu::FolderComponent::getSelectedItem() const
{
    if(selectedIndex < 0 || selectedIndex > folderButtons.size())
    {
        return MenuItem();
    }
    return folderButtons[selectedIndex]->getMenuItem();
}

/*
 * Gets the index of the current menu selection.
 */
int AppMenu::FolderComponent::getSelectedIndex() const
{
    return selectedIndex;
}

/*
 * Sets the current selected menu index.
 */
void AppMenu::FolderComponent::setSelectedIndex(const int newSelection)
{
    ScopeTimer t("AppMenu::FolderComponent::setSelectedIndex");
    if(selectedIndex == newSelection)
    {
        return;
    }
    if(selectedIndex >= 0 && selectedIndex < folderButtons.size())
    {
        getButtonComponent(selectedIndex)->setSelected(false);
    }
    if(newSelection >= 0 && newSelection < folderButtons.size())
    {
        selectedIndex = newSelection;
        getButtonComponent(selectedIndex)->setSelected(true);
    }
    else
    {
        selectedIndex = -1;
    }
}

/*
 * Gets the button component of a child menu item in this folder.
 */
AppMenu::MenuButton* 
AppMenu::FolderComponent::getButtonComponent(const int index) const
{
    if(index < 0 || index >= folderButtons.size())
    {
        return nullptr;
    }
    return folderButtons[index];
}


/*
 * Creates and inserts a new ItemButton when a new child menu button is created.
 */
void AppMenu::FolderComponent::childAdded(const int childIndex)
{
    MenuItem newChild = folderItem.getFolderItem(childIndex);
    folderButtons.insert(childIndex, createMenuButton(newChild));
    addAndMakeVisible(folderButtons[childIndex]);
    if(selectedIndex >= childIndex)
    {
        selectedIndex++;
    }
    // Trigger the parent's resized() method to force a layout update
    juce::Component* parent = getParentComponent();
    if(parent != nullptr)
    {
        parent->resized();
    }
}

/*
 * Removes the corresponding ItemButton when a child menu item is removed.
 */
void AppMenu::FolderComponent::childRemoved(const int removedIndex)
{
    removeChildComponent(folderButtons[removedIndex]);
    folderButtons.remove(removedIndex);
    if(removedIndex == folderButtons.size())
    {
        setSelectedIndex(selectedIndex - 1);
    }
    else if(selectedIndex == removedIndex)
    {
        getButtonComponent(selectedIndex)->setSelected(true);
    }
    else if(selectedIndex > removedIndex)
    {
        selectedIndex--;
    }
    // Trigger the parent's resized() method to force a layout update
    juce::Component* parent = getParentComponent();
    if(parent != nullptr)
    {
        parent->resized();
    }
}

/*
 * Swaps the positions of two of the folder's ItemButtons when their 
 * corresponding MenuItems are swapped.
 */
void AppMenu::FolderComponent::childrenSwapped
(const int swapIndex1, const int swapIndex2)
{
    juce::Rectangle<int> firstBounds = folderButtons[swapIndex1]->getBounds();
    folderButtons[swapIndex1]->setBounds
        (folderButtons[swapIndex2]->getBounds());
    folderButtons[swapIndex2]->setBounds(firstBounds);
    folderButtons.swap(swapIndex1, swapIndex2);
    if(selectedIndex == swapIndex1)
    {
        selectedIndex = swapIndex2;
    }
    else if(selectedIndex == swapIndex2)
    {
        selectedIndex = swapIndex1;
    }
}

/*
 * Recalculates menu button layout when the folder bounds change.
 */
void AppMenu::FolderComponent::resized()
{
    ScopeTimer t("AppMenu::FolderComponent::resized");
    updateButtonLayout();
}
