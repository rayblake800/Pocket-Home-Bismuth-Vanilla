#define APPMENU_IMPLEMENTATION_ONLY
#include "MenuItem.h"

/*
 * Creates a menu item from some source of menu data. 
 */
AppMenu::MenuItem::MenuItem(ItemData::Ptr dataSource) :
Nullable(dataSource) { }

/*
 * Creates a menu item copying data from another menu item. 
 */
AppMenu::MenuItem::MenuItem(const MenuItem& toCopy) :
Nullable(toCopy.getData()) { }

/*
 * Gets the menu item's displayed title.
 */
juce::String 
AppMenu::MenuItem::getTitle() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getTitle();
}

/*
 * Gets the menu item's icon name.
 */
juce::String 
AppMenu::MenuItem::getIconName() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getIconName();
}

/*
 * Gets any launch command associated with this menu item.
 */
juce::String 
AppMenu::MenuItem::getCommand() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getCommand();
}

/*
 * Checks if this menu item is a terminal application.
 */
bool AppMenu::MenuItem::getLaunchedInTerm() const
{
    if(isNull())
    {
        return false;
    }
    return getData()->getLaunchedInTerm();
}

/*
 * Gets all application categories associated with this menu item.
 */
juce::StringArray 
AppMenu::MenuItem::getCategories() const
{
    if(isNull())
    {
        return juce::StringArray();
    }
    return getData()->getCategories();
}

/*
 * Gets the folder menu item that contains this menu item.
 */
AppMenu::MenuItem 
AppMenu::MenuItem::getParentFolder() const
{
    if(isNull())
    {
        return MenuItem();
    }
    return MenuItem(getData()->getParentFolder());
}

/*
 * Gets the menu item's index within its folder.
 */
const int AppMenu::MenuItem::getIndex() const
{
    if(isNull())
    {
        return -1;
    }
    return getData()->getIndex();
}

/*
 * Checks if this menu item represents a folder within the menu.
 */
bool AppMenu::MenuItem::isFolder() const
{
    if(isNull())
    {
        return false; 
    }
    return getData()->isFolder();
}

/*
 * Gets the number of menu items in the folder opened by this menu item.
 */
int AppMenu::MenuItem::getFolderSize() const
{
    if(isNull())
    {
        return 0; 
    }
    return getData()->getFolderSize();
}

/*
 * Gets the number of folder items held by this menu item that can be reordered.
 */
int AppMenu::MenuItem::getMovableChildCount() const
{
    if(isNull())
    {
        return 0; 
    }
    return getData()->getMovableChildCount();
} 

/*
 * Gets a menu item held within this menu item.
 */
AppMenu::MenuItem 
AppMenu::MenuItem::getFolderItem(const int folderIndex) const
{
    if(isNull())
    {
        return MenuItem(); 
    }
    return MenuItem(getData()->getChild(folderIndex));
}

/*
 * Gets all menu items held within this menu item.
 */
juce::Array<AppMenu::MenuItem> 
AppMenu::MenuItem::getFolderItems() const
{
    if(isNull())
    {
        return {}; 
    }
    juce::Array<MenuItem> folderItems;
    juce::Array<ItemData::Ptr> childData = getData()->getChildren();
    for(const ItemData::Ptr& item : childData)
    {
        folderItems.add(MenuItem(item));
    }
    return folderItems;
}

/*
 * Gets an appropriate title to use for a deletion confirmation window.
 */
juce::String 
AppMenu::MenuItem::getConfirmDeleteTitle() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getConfirmDeleteTitle();
}

/*
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
juce::String 
AppMenu::MenuItem::getConfirmDeleteMessage() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getConfirmDeleteMessage();
}

/*
 * Gets a title to use when editing this menu item.
 */
juce::String 
AppMenu::MenuItem::getEditorTitle() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getEditorTitle();
}

/*
 * Compares this menu item with another.
 */
bool AppMenu::MenuItem::operator==(const MenuItem& toCompare) const
{
    return getData() == toCompare.getData();
}

/*
 * Compares this menu item with another.
 */
bool AppMenu::MenuItem::operator<(const MenuItem& toCompare) const
{
    return getTitle() < toCompare.getTitle();
}

/*
 * Calls the childAdded method that passes in a MenuItem instead of an 
 * ItemData::Ptr.
 */
void AppMenu::MenuItem::Listener::childAdded(ItemData::Ptr folderItem, 
        const int childIndex)
{
    childAdded(MenuItem(folderItem), childIndex);
}

/*
 * Calls the childRemoved method that passes in a MenuItem instead of an 
 * ItemData::Ptr.
 */
void AppMenu::MenuItem::Listener::childRemoved(ItemData::Ptr folderItem,
        const int removedIndex)
{
    childRemoved(MenuItem(folderItem), removedIndex);
}

/*
 * Calls the childrenSwapped method that passes in a MenuItem instead of an 
 * ItemData::Ptr.
 */
void AppMenu::MenuItem::Listener::childrenSwapped(ItemData::Ptr folderItem,
        const int swapIndex1, const int swapIndex2)
{
    childrenSwapped(MenuItem(folderItem), swapIndex1, swapIndex2);
}

/*
 * Calls the removedFromMenu method that passes in a MenuItem instead of an 
 * ItemData::Ptr.
 */
void AppMenu::MenuItem::Listener::removedFromMenu(ItemData::Ptr removedItem)
{
    removedFromMenu(MenuItem(removedItem));
}

/*
 * Calls the dataChanged method that passes in a MenuItem instead of an 
 * ItemData::Ptr.
 */
void AppMenu::MenuItem::Listener::dataChanged(ItemData::Ptr changedItem)
{
    dataChanged(MenuItem(changedItem));
}

/*
 * If this MenuItem is not null, add a Listener object to its menu data.
 */
void AppMenu::MenuItem::addListener(Listener* toAdd)
{
    if(!isNull())
    {
        getData()->addListener(toAdd);
    }
}

/*
 * If this menuItem is not null, make a Listener object stop tracking its menu 
 * data.
 */
void AppMenu::MenuItem::removeListener(Listener* toRemove)
{
    if(!isNull())
    {
        getData()->removeListener(toRemove);
    }
}

/*
 * Checks if a data field within a menu item can be edited.
 */
bool AppMenu::MenuItem::Editor::isEditable
(MenuItem menuItem, const DataField dataField) const
{
    if(menuItem.isNull())
    {
        return false;
    }
    return menuItem.getData()->isEditable(dataField);
}

/*
 * Sets a menu item's displayed title.
 */
void AppMenu::MenuItem::Editor::setTitle
(MenuItem menuItem, const juce::String& title) const
{
    if(!menuItem.isNull() && isEditable(menuItem, DataField::title))
    {
        menuItem.getData()->setTitle(title);
    }
}

/*
 * Sets the name or path used to load a menu item's icon file.
 */
void AppMenu::MenuItem::Editor::setIconName
(MenuItem menuItem, const juce::String& iconName) const
{
    if(!menuItem.isNull() && isEditable(menuItem, DataField::icon))
    {
        menuItem.getData()->setIconName(iconName);
    }
}

/*
 * Sets a menu item's application launch command.
 */
void AppMenu::MenuItem::Editor::setCommand
(MenuItem menuItem, const juce::String command) const
{
    if(!menuItem.isNull() && isEditable(menuItem, DataField::command))
    {
        menuItem.getData()->setCommand(command);
    }
}

/*
 * Sets if a menu item runs its command in a new terminal window.
 */
void AppMenu::MenuItem::Editor::setLaunchedInTerm
(MenuItem menuItem, const bool launchInTerm) const
{
    if(!menuItem.isNull() && isEditable(menuItem, DataField::termLaunchOption))
    {
        menuItem.getData()->setLaunchedInTerm(launchInTerm);
    }
}

/*
 * Sets the application categories connected to a menu item.
 */
void AppMenu::MenuItem::Editor::setCategories
(MenuItem menuItem, const juce::StringArray categories) const
{
    if(!menuItem.isNull() && isEditable(menuItem, DataField::categories))
    {
        menuItem.getData()->setCategories(categories);
    }
}

/*
 * Attempts to insert a menu item into a folder menu item, saving the change to
 * the folder menu item's data source.
 */
bool AppMenu::MenuItem::Editor::insertChild
(MenuItem folderItem, MenuItem childItem, const int index) const
{
    if(!folderItem.isNull() && !childItem.isNull())
    {
        return folderItem.getData()->insertChild(childItem.getData(), index);
    }
    return false;
}

/*
 * Attempts to remove a menu item from the menu, saving the change to the menu
 * item's data source.
 */
bool AppMenu::MenuItem::Editor::remove(MenuItem toRemove) const
{
    if(!toRemove.isNull())
    {
        return toRemove.getData()->remove();
    }
    return false;
}

/*
 * Swaps the positions of two folder items, saving the change to the folder menu
 * item's data source.
 */
bool AppMenu::MenuItem::Editor::swapChildren
(MenuItem folderItem, const int childIdx1, const int childIdx2)
{
    if(!folderItem.isNull())
    {
        return folderItem.getData()->swapChildren(childIdx1, childIdx2);
    }
    return false;
}

/*
 * Saves all changes made to a menu item back to the menu item's data source.
 */
void AppMenu::MenuItem::Editor::saveChanges(MenuItem menuItem) const
{
    if(!menuItem.isNull())
    {
        menuItem.getData()->saveChanges();
    }
}
